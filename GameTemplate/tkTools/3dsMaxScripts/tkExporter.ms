/*!
 * @brief	スケルトンエクスポーター。
 */

pluginManager.loadClass FbxExporter

--オブジェクトパラメータ。
struct ObjectParam(
	localPos,
	localRot ,
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
	editText animClipStartFrame "" pos:[91,270] width:58 height:21
	label lbl1 "start frame" pos:[23,273] width:60 height:16
	groupBox grp1 "Animation clip" pos:[13,249] width:145 height:134
	groupBox grp2 "Skeleton" pos:[12,172] width:145 height:64
	button btn_saveAnimationClip "Save" pos:[28,339] width:120 height:28
	label lbl2 "end frame" pos:[23,304] width:60 height:16
	editText animClipEndFrame "" pos:[91,302] width:58 height:21
	button btn_saveSkeleton "Save" pos:[23,191] width:120 height:28
	label lbl3 "Label" pos:[34,291] width:0 height:0
	groupBox grp5 "FBX" pos:[13,13] width:145 height:64
	button btn_save_fbx "Save" pos:[24,33] width:120 height:28
	groupBox grp10 "FBX And Skeleton" pos:[13,92] width:145 height:64
	button btn_save_fbx_and_skeleton "Save" pos:[24,112] width:120 height:28
	
	/*
	 *  出力するノードの情報を構築。
	 */
	fn BuildOutputNodeInfo n parentId= 
	(
		objParam = ObjectParam Point3 Quat "" Matrix3 Matrix3 -1 Node
		objParam.objName = n.name
		objParam.bindPose = n.transform
		objParam.invBindPose = inverse n.transform
		objParam.n = n
		if n.parent == undefined then(
			--親がいない
			objParam.parentId = -1
			objParam.localPos = n.pos
			objParam.localRot = n.rotation
			
		)else(
			--親がいる
			objParam.parentId = parentId
			objParam.localPos = n.pos * inverse n.parent.transform
			objParam.localRot = n.rotation * inverse n.parent.rotation
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
	 */
	fn SaveSkeleton = 
	(
		filepath = getSaveFileName caption:"" types: "skeleton (*tks)|*.tks|All Files (*.*)|*.*|"
		
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
	 *  アニメーションクリップの保存のボタンが押されたときの処理。
	 */
	on btn_saveAnimationClip pressed do
	(
		if selection[1] == undefined  then(
			Messagebox "出力するアニメーションクリップを含むスケルトンのルートのオブジェクトを選択してください。"
		)else(
			--とりあえず全クリップ出力する。
			g_objectParams = #()
			--アニメーション時間を0にする。
			startFrame = animClipStartFrame.text as integer
			endFrame = animClipEndFrame.text as integer
			animationRange = interval startFrame endFrame
			slidertimer = 0
			animate on(
				--アニメーション進行中の処理。
				BuildOutputNodeInfo selection[1] -1
				try(
					SaveAnimationClip()
				)catch(
					messageBox "ファイルの保存に失敗しました"
				)
			)
		)
	)
	/*!
	 * @brief	FBXの保存のボタンが押されたときの処理。
	 */
	on btn_save_fbx pressed do
	(
		OpenFbxSetting()
	)
)
addrollout tkExporter style:#(#style_titlebar, #style_sysmenu)