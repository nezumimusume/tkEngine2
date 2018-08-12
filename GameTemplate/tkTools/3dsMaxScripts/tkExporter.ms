/*!
 * @brief	スケルトンエクスポーター。
 */
 
pluginManager.loadClass FBXIMPORTER 
pluginManager.loadClass FbxExporter

--オブジェクトパラメータ。
struct ObjectParam(
	objName,
	bindPose,
	invBindPose,
	parentId,
	n
)
--アニメーションキーフレーム
struct KeyFrame(
	boneId,
	t,
	localTransform
)
--アニメーションクリップ
struct AnimationClip(
	animClipName,		--アニメーションクリップ名
	keyFrames = #()		--キーフレームのリスト
)
--オブジェクトパラメータの配列。
g_objectParams = #()

startFrame = 0
endFrame = 0

rollout tkExporter "tkExporter" width:194 height:248
(
	editText 'animClipStartFrame' "" pos:[90,427] width:58 height:21 align:#left
	label 'lbl1' "start frame" pos:[22,430] width:60 height:16 align:#left
	groupBox 'grp1' "Animation clip" pos:[14,401] width:147 height:134 align:#left
	button 'btn_saveAnimationClip' "Save" pos:[27,496] width:120 height:28 align:#left
	label 'lbl2' "end frame" pos:[22,461] width:60 height:16 align:#left
	editText 'animClipEndFrame' "" pos:[90,459] width:58 height:21 align:#left
	button 'btn_saveSkeleton' "Save Skeleton" pos:[22,364] width:120 height:28 align:#left
	label 'lbl3' "Label" pos:[33,423] width:0 height:0 align:#left
	groupBox 'grp5' "FBX" pos:[13,13] width:145 height:109 align:#left
	button 'btn_save_fbx' "Save" pos:[27,33] width:120 height:28 align:#left
	groupBox 'grp10' "Level" pos:[13,135] width:145 height:189 align:#left
	button 'btn_load_locObject' "Load Object" pos:[26,215] width:120 height:28 align:#left
	editText 'levelName' "" pos:[23,181] width:120 height:24 align:#left
	label 'lbl4' "edit level name" pos:[23,160] width:121 height:16 align:#left
	button 'SaveLoc' "Save Level" pos:[26,285] width:120 height:28 align:#left
	groupBox 'grp6' "Animation" pos:[3,338] width:164 height:200 align:#left
	button 'btn_copy_locObject' "Copy Object" pos:[26,249] width:120 height:28 align:#left
	button 'btn_save_selected_fbx' "Save Selected" pos:[27,79] width:120 height:28 align:#left
	
	on  tkExporter open do
	(
		levelName.text = "level00"
	)
	
	/*
	 *  出力するノードの情報を構築。
	 */
	fn BuildOutputNodeInfo n parentId= 
	(
		objParam = ObjectParam "" Matrix3 Matrix3 -1 Node
		objParam.objName = n.name
		objParam.bindPose = n.transform
		objParam.invBindPose = inverse n.transform
		objParam.n = n
		if n.parent == undefined then(
			--親がいない
			objParam.parentId = -1
		)else(
			--親がいる
			objParam.parentId = parentId
		)

		append g_objectParams objParam
		parentId = g_objectParams.count-1
		--子供も。
		for child in n.children do(
			BuildOutputNodeInfo child parentId
		)
	)
	/*
	 *  スケルトンの保存。
	*@param[in]  filepath		保存するファイルパス。
	 */
	fn SaveSkeletonFilePath filepath = 
	(
		file = fopen filepath "wb"
			
		try(
			--ヘッダーを出力
			WriteLong file g_objectParams.count
			
			for o in g_objectParams do (
				
				WriteByte file o.objName.count
				WriteString file o.objName
				WriteLong file o.parentId
				--バインドポーズを出力。
				for i = 1 to 4 do(
					WriteFloat file o.bindPose[i].x
					WriteFloat file o.bindPose[i].y
					WriteFloat file o.bindPose[i].z
				)
				--バインドポーズの逆数を出力。
				for i = 1 to 4 do(
					WriteFloat file o.invBindPose[i].x
					WriteFloat file o.invBindPose[i].y
					WriteFloat file o.invBindPose[i].z
				)
			)
		)catch(
			Messagebox "ファイルの保存に失敗しました"
		)
		FClose file
	)
	/*
	 *  スケルトンの保存。
	 */
	fn SaveSkeleton = 
	(
		filepath = getSaveFileName caption:"" types: "skeleton (*tks)|*.tks|All Files (*.*)|*.*|"
		
		if filepath != undefined then
		(
			SaveSkeletonFilePath filepath
		)
	)
	/*
	 * レベルデータを保存。
	 */
	fn SaveLevel = 
	(
		filepath = getSaveFileName caption:"" types: "level (*.tkl)|*.tkl|All Files (*.*)|*.*|"
		
		if filepath != undefined then
		(
			
			file = fopen filepath "wb"
			
			try(
				--ヘッダーを出力
				WriteLong file g_objectParams.count
				
				for o in g_objectParams do (
					
					WriteByte file o.objName.count
					WriteString file o.objName
					WriteLong file o.parentId
					--バインドポーズを出力。
					for i = 1 to 4 do(
						WriteFloat file o.bindPose[i].x
						WriteFloat file o.bindPose[i].y
						WriteFloat file o.bindPose[i].z
					)
					--バインドポーズの逆数を出力。
					for i = 1 to 4 do(
						WriteFloat file o.invBindPose[i].x
						WriteFloat file o.invBindPose[i].y
						WriteFloat file o.invBindPose[i].z
					)
				)
			)catch(
				Messagebox "ファイルの保存に失敗しました"
			)
			FClose file
		)
	)
	/*
	 *  アニメーションクリップの保存。
	 */
	fn SaveAnimationClip=
	(
		filepath = getSaveFileName caption:"" types: "animation clip (*tka)|*.tka|All Files (*.*)|*.*|"

		if filepath != undefined then
		(
			
			numKey = endFrame - startFrame + 1
			if numKey <= 0 then(
				--1フレームだけ出す。
				endFrame = startFrame
				numKey = 1
			)
			deltaTime = 1.0/30
			keyTime = 0.0
			
			numKey *= g_objectParams.Count
			matTransList = #()
			matRotList = #()

			file = fopen filepath "wb"
			--ヘッダーを出力
			WriteLong file numKey
			--アニメーションイベントの数を出力
			numNoteTrack = numNoteTracks selection[1]

			if numNoteTrack > 0 then(
				--ノートキーの数を出力
				nt = getNoteTrack selection[1] 1
				WriteLong file nt.keys.count
				for k in nt.keys do(
					--イベントが発生する時間を出力
					if k.time >= numKey then(
						exit
					)
					invokeTime = k.time * deltaTime
					
					WriteFloat file invokeTime
					WriteLong file k.value.count
					WriteString file k.value
				)
			)else(
				WriteLong file 0
			)
			
			fclose file
			--アニメーションの情報を出力。
			slidertime=startFrame

			file = fopen filepath "ab"
			
			for i=startFrame to endFrame do(
				boneId = 0
				keyTime = (i - startFrame) * deltaTime
				
				for o in g_objectParams do (
					n = o.n

					--出力
					WriteLong file boneId -- 骨ID
					WriteFloat file keyTime -- 時間
					
					--ボーンローカル行列に変換する。
					
					transformMatrix = Matrix3 1
					if n.parent == undefined then(
						transformMatrix =n.transform
					)else(
						transformMatrix = n.transform * inverse n.parent.transform
					)
					for i = 1 to 4 do(
						
						WriteFloat file transformMatrix[i].x
						WriteFloat file transformMatrix[i].y
						WriteFloat file transformMatrix[i].z
					)
					boneId +=1
					
				)
				slidertime += 1
			)
			
			fclose file

		)
	)
	/*
	 *  スケルトンの出力のボタンが押されたときの処理。
	 */
	on btn_saveSkeleton pressed do
	(
		if selection[1] == undefined then(
			Messagebox "出力するスケルトンのルートのオブジェクトを選択して下さい。。。"
		)else(
			g_objectParams = #()
			--アニメーション時間を0にする。
			animationRange = interval 0 animationRange.end
			slidertime = 0
			at time 0(
				--アニメーション進行中の処理。
				BuildOutputNodeInfo selection[1] -1
				SaveSkeleton()
			)
		)
	)
	/*
	* Levelを保存。
	*/
	on SaveLoc pressed do
	(
		--現在編集中のルートのレベルを取得。
		editLevelObj = getNodeByName levelName.text
		if editLevelObj == undefined then(
			Messagebox "保存するレベルが存在しません。"
		)else(
			select editLevelObj
		
			--スケルトンとして保存。
			g_objectParams = #()
			--アニメーション時間を0にする。
			animationRange = interval 0 animationRange.end
			slidertime = 0
			at time 0(
				--アニメーション進行中の処理。
				BuildOutputNodeInfo selection[1] -1
				SaveLevel()
			)
		)
	)
		/*
	 *  スキンモディファイアが設定されているノードがあったら、そのノードに割り当てられているスケルトンのルートを取得。
	 */
	fn GetSkeletonRoot nodes =
	(
		for p in nodes do
		(
			--スキンモディファイアを取得。
			skinMod = p.modifiers[skin]
			
			if skinMod != undefined then(
				--スキンを見つけた。
				clearSelection()
				max modify mode
				select p
				modPanel.setCurrentObject skinMod
				numBone = skinOps.GetNumberBones( skinMod )
				if numBone > 0 then(
					--bone発見したぞいぞい
					boneName = skinOps.GetBoneName skinMod 1 1 
					--名前からノードを選択
					boneNode = getNodebyName(boneName)
					if boneNode != undefined  then (
						--スケルトンのルートを検索するぞ
						while boneNode.parent != undefined  do(
							boneNode = boneNode.parent
						)
						--ルートを返すん。
						return boneNode
					)
				)
			)
		)
		return undefined
	)
	/*
	 *  アニメーションクリップの保存のボタンが押されたときの処理。
	 */
	on btn_saveAnimationClip pressed do
	(
		--とりあえず全クリップ出力する。
		skeletonRoot = GetSkeletonRoot objects
		
		if skeletonRoot == undefined  then(
			--シーンにスキンモディファイアが見つからなかった。
			if selection[1] == undefined  then(
				Messagebox "スケルトンのルートのオブジェクトの自動判別ができませんでした。\nスケルトンのルートのオブジェクトを選択してアニメーションクリップを保存してください。\n"
				return 0
			)
			skeletonRoot = selection[1]
		)
		g_objectParams = #()
		--アニメーション時間を0にする。
		startFrame = animClipStartFrame.text as integer
		endFrame = animClipEndFrame.text as integer
		animationRange = interval startFrame endFrame
		slidertimer = 0
		animate on(
			--アニメーション進行中の処理。
			BuildOutputNodeInfo skeletonRoot -1
			try(
				SaveAnimationClip()
			)catch(
				messageBox "ファイルの保存に失敗しました"
			)
		)
	)
	
	function selectHierarchy =
    (
		for p in selection do
		(
			if p.children != undefined do
			(
				selectmore p.children
			)
		)
    )
    
	/*
	 *　配置するオブジェクトをコピー
	 */
	on btn_copy_locObject pressed do
	(
		if selection.count == 0  then(
			Messagebox "コピーするオブジェクトを選択してください。"
		)else(
			selectHierarchy()
			parentObj = copy selection[1]
			parentObj.name = selection[1].name
			for obj in 2 to selection.count do (
				cloneObj = copy selection[obj]
				cloneObj.name = selection[obj].name
				cloneObj.parent = parentObj
				parentObj = merge( parentObj cloneObj )
				deselect cloneObj
			)
			select parentObj
		)
	)
	/*
	 * 配置するオブジェクトのロード。
	 */
	on btn_load_locObject pressed do
	(
		
		if  levelName.text == "" then(
			Messagebox "編集するレベルの名前を入力して下さい。"
		)else(
			filepath = getOpenFileName caption:"" types: "fbx (*fbx)|*.fbx|All Files (*.*)|*.*|"
			if filepath != undefined then(
				editLevelObj = getNodeByName levelName.text
				if editLevelObj == undefined then(
					--ダミーを作る。
					editLevelObj = Dummy()
					editLevelObj.name = levelName.text
				)
				
				FBXImporterSetParam "ScaleConversion" false
				FBXImporterSetParam "UpAxis" "Z"
				ImportFile filepath #noPrompt
				importObj = $
		
				filename = getFilenameFile filepath
				
				--全てを編集可能ポリゴンに変換する。
				delObjArray = #()
				for i=1 to $selection.count do
				(
					p = convertToMesh($selection[i]);
					--編集可能ポリゴンに変換できなかった場合は、削除リストに積む。
					if p == undefined then(
						append delObjArray $selection[i]
					)
	
				)
				--編集可能ポリゴンに変換できなかったオブジェクトを削除。
				for i=1 to delObjArray.count do
				(
					delete delObjArray[i]
				)
				parentBox = box length:1 width:1 height:1
				p = convertToMesh(parentBox)
				importObj.parent = parentBox
				select p
				selectMore $.children
				--結合。
				while  $selection.count>1 do
				(
					attach p $selection[$selection.count];
				)

				--ダミーをレベルの下にぽいっとな。
				$selection[1].pivot = Point3 0 0 0
				selection[1].name = filename
				$selection[1].parent = editLevelObj
			)
		)
	)
	/*
	 *  FBX出力用の設定を行う。
	 */
	fn SetupFbxExportSettings = 
	(
		--他にもオプションが必要なら追加していってください。
		FBXExporterSetParam "ResetExport"
		FBXExporterSetParam "ScaleFactor" 1.0
		FBXExporterSetParam "UpAxis" "Z"
		FBXExporterSetParam "TangentSpaceExport" true
		FBXExporterSetParam "Triangulate" true
		FBXExporterSetParam "ASCII" true
	)
	/*
	 *  マテリアルが貼られていないノードがないかチェックを行う。
	 */
	fn CheckMaterial nodes =
	(
		for p in nodes do
		(
			meshCount = GetTriMeshFaceCount(p)
			if classof( p ) == BoneGeometry do(
				--こいつはボーン
				meshCount[1] = 0
				meshCount[2] = 0
			)
			if meshCount[1] > 0 do
			(
				--メッシュがある。
				if p.material == undefined do
				(
					--マテリアルが張られていない。
					errorMessage = "エラー\n " + p.name + "にマテリアルが割り当てられていないため出力できません。\n"+ "マテリアルを割り当ててください。\n"
					messagebox errorMessage
					return false				
				)
			)
		)
		return true
	)

	/*
	 *  スキンモディファイアが設定されているノードがあったら、スケルトンを出力する。
	 */
	fn TrySaveSkeleton nodes filePath = 
	(
		--スケルトンを選択出来たら取得する。
		skeletonRoot = GetSkeletonRoot nodes
		if skeletonRoot != undefined  then(
			
			--スケルトンを保存する。
			g_objectParams = #()
			--アニメーション時間を0にする。
			animationRange = interval 0 animationRange.end
			slidertime = 0
			at time 0(
				--アニメーション進行中の処理。
				BuildOutputNodeInfo skeletonRoot -1
				--ファイルパスの拡張子をtksに変更する。
				extPos = findString filepath "."
				filepath = replace filepath extPos 4 ".tks"
				--保存だ―。
				SaveSkeletonFilePath filepath
			)
		)
	)
	/*!
	 * @brief	FBXのSaveのボタンが押されたときの処理。
	 */
	on btn_save_fbx pressed do
	(
		if CheckMaterial(objects) then (
			SetupFbxExportSettings()
			
			filepath = getSaveFileName caption:"" types: " (*.fbx)|*.fbx|All Files (*.*)|*.*|"
			if filepath != undefined then(
				exportFile(filepath) #noPrompt
			)
			--スキンモディファイアがある場合にスケルトンデータを保存する。
			TrySaveSkeleton objects filepath
		)
	)
	/*!
	 * @brief	FBXのSave Selectedのボタンが押されたときの処理。
	 */
	on btn_save_selected_fbx pressed do
	(
		
		if CheckMaterial(selection) then (
			SetupFbxExportSettings()
			filepath = getSaveFileName caption:"" types: " (*.fbx)|*.fbx|All Files (*.*)|*.*|"
			if filepath != undefined then(
				exportFile(filepath) #noPrompt selectedOnly:true
			)
			--スキンモディファイアがある場合にスケルトンデータを保存する。
			TrySaveSkeleton selection filepath
		)
	)
)
addrollout tkExporter style:#(#style_titlebar, #style_sysmenu)