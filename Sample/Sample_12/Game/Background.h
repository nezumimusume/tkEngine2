#pragma once

//CMeshColliderとCRigidBodyを使用したい場合は下記のヘッダーファイルをインクルードする必要がある。
#include "tkEngine/physics/tkPhysicsStaticObject.h"

class Background : public IGameObject
{
public:
	bool Start() override;
	void OnDestroy() override;
private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//スキンモデルレンダラー。
	CPhysicsStaticObject m_physicsStaticObject;		//静的物理オブジェクト。
	CVector3 m_position = CVector3::Zero;			//座標。
};

