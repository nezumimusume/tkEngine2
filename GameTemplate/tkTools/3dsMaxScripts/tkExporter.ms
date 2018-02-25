/*!
 * @brief	スケルトンエクスポーター。
 */

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

rollout tkExporter "tkExporter" width:194 height:248
(
	groupBox grp1 "Animation clip" pos:[10,97] width:171 height:135
	groupBox grp2 "Skeleton" pos:[10,8] width:167 height:74
	button btn_saveAnimationClip "Save" pos:[31,187] width:120 height:28
	label lbl1 "start frame" pos:[29,119] width:60 height:16
	editText animClipStartFrame "" pos:[88,116] width:62 height:21
	label lbl2 "end frame" pos:[29,146] width:60 height:16
	editText animClipEndFrame "" pos:[88,145] width:62 height:21
	button btn_saveSkeleton "Save" pos:[31,35] width:120 height:28
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
		AnimationClip animClip string #()
		filepath = getSaveFileName caption:"" types: "animation clip (*tka)|*.tka|All Files (*.*)|*.*|"

		if filepath != undefined then
		(
			startFrame = animClipStartFrame.text as integer + 1
			endFrame = animClipEndFrame.text as integer + 1
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
			
			--アニメーションの情報を出力。
			for i=startFrame to endFrame do(
				boneId = 0
				keyTime = (i - startFrame) * deltaTime
				for o in g_objectParams do (
					
					n = o.n
					transCtr = n.pos.track
					rotCtr = n.rotation.track
					
					matTrans = matrix3 1
					pos = Point3 0.0 0.0 0.0
					if matTransList.count <= boneId then(
						pos.x = o.localPos.x
						pos.y = o.localPos.y
						pos.z = o.localPos.z
						translate matTrans pos
						append matTransList matTrans
					)
					if matRotList.count <= boneId then(
						matRot = o.localRot as matrix3
						append matRotList matRot
					)
					if transCtr.x_position.keys.count >= i  then(
						pos.x = transCtr.x_position.keys[i].value
						pos.y = transCtr.y_position.keys[i].value 
						pos.z = transCtr.z_position.keys[i].value 
						 matTransList[boneId+1] = transMatrix(pos)
					)
					
					if rotCtr.x_rotation.keys.count >= i then(
						xRot = rotCtr.x_rotation.keys[i].value
						yRot = rotCtr.y_rotation.keys[i].value
						zRot = rotCtr.z_rotation.keys[i].value
						matRotList[boneId+1] = rotateXMatrix (xRot) * rotateYMatrix (yRot) * rotateZMatrix (zRot)
					)

					transformMatrix = matRotList[boneId+1] * matTransList[boneId+1]
					--出力
					WriteLong file boneId -- 骨ID
					WriteFloat file keyTime -- 時間
					for i = 1 to 4 do(
						WriteFloat file transformMatrix[i].x
						WriteFloat file transformMatrix[i].y
						WriteFloat file transformMatrix[i].z
					)
					boneId +=1
				)
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
			BuildOutputNodeInfo selection[1] -1
			SaveSkeleton()
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
			BuildOutputNodeInfo selection[1] -1
			try(
				SaveAnimationClip()
			)catch(
				messageBox "ファイルの保存に失敗しました"
			)
		)
	)
)
createDialog tkExporter style:#(#style_titlebar, #style_sysmenu)