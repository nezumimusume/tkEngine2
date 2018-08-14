/*!
 * @brief	静的オブジェクト。
 */


#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/physics/tkPhysicsStaticObject.h"

namespace tkEngine{
	CPhysicsStaticObject::CPhysicsStaticObject()
	{
	}
	CPhysicsStaticObject::~CPhysicsStaticObject()
	{
		PhysicsWorld().RemoveRigidBody(m_rigidBody);
	}
	void CPhysicsStaticObject::CreateMeshObject(prefab::CSkinModelRender* skinModelRender, CVector3 pos, CQuaternion rot, CVector3 scale)
	{
		CreateMeshObject(skinModelRender->GetSkinModel(), pos, rot, scale);
	}
	void CPhysicsStaticObject::CreateMeshObject(CSkinModel& skinModel, CVector3 pos, CQuaternion rot, CVector3 scale)
	{
		CMatrix mScale;
		mScale.MakeScaling(scale);
		m_meshCollider.CreateFromSkinModel(skinModel, &mScale);
		RigidBodyInfo rbInfo;
		rbInfo.collider = &m_meshCollider;
		rbInfo.mass = 0.0f;
		rbInfo.pos = pos;
		rbInfo.rot = rot;
		m_rigidBody.Create(rbInfo);
		PhysicsWorld().AddRigidBody(m_rigidBody);
	}
}