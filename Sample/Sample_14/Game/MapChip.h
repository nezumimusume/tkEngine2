#pragma once

#include "tkEngine/physics/tkMeshCollider.h"
#include "tkEngine/physics/tkRigidBody.h"

class MapChip : public IGameObject
{
public:
	MapChip();
	~MapChip();
	void Init(
		const wchar_t* modelFilePath, 
		CVector3 pos,
		CVector3 scale,
		CQuaternion rotation
	);
	bool Start();
	void Update();
	void Render(CRenderContext& rc);
	//////////////////////////////////////////////////
	// ここからメンバ変数。
	//////////////////////////////////////////////////
	CSkinModelData m_skinModelData;		//!<スキンモデルデータ。
	CSkinModel m_skinModel;				//!<スキンモデル。
	CVector3 m_position;				//!<座標。
	CVector3 m_scale;					//!<拡大。
	CQuaternion m_rotation;				//!<回転。
	CMeshCollider m_meshCollider;		//!<メッシュコライダー。
	CRigidBody m_rigidBody;				//!<剛体。
};

