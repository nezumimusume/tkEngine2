#pragma once

#include "tkEngine/physics/tkPhysicsStaticObject.h"

class MapChip : public IGameObject
{
public:
	void OnDestroy() override;
	void Init(
		const wchar_t* modelFilePath, 
		CVector3 pos,
		CVector3 scale,
		CQuaternion rotation
	);

	
	//////////////////////////////////////////////////
	// ここからメンバ変数。
	//////////////////////////////////////////////////
	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CPhysicsStaticObject m_physicsStaticObject;	//!<静的物理オブジェクト。
	CVector3 m_position;				//!<座標。
	CQuaternion m_rotation;				//!<回転。
};

