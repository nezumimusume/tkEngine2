/*!
 * @brief	静的オブジェクト。
 */


#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/physics/tkPhysicsGhostObject.h"
#include "tkEngine/physics/tkBoxCollider.h"

namespace tkEngine{
	CPhysicsGhostObject::CPhysicsGhostObject()
	{
	}
	CPhysicsGhostObject::~CPhysicsGhostObject()
	{
		Release();
	}
	
	void CPhysicsGhostObject::Release()
	{
		if (m_isRegistPhysicsWorld == true) {
			PhysicsWorld().RemoveCollisionObject(m_ghostObject);
			m_isRegistPhysicsWorld = false;
		}
	}
	void CPhysicsGhostObject::Create(CVector3 pos, CQuaternion rot, CVector3 size)
	{
		Release();
		auto boxCollider = std::make_unique<CBoxCollider>();
		boxCollider->Create(size);
		m_collider = std::move(boxCollider);
		m_ghostObject.setCollisionShape(m_collider->GetBody());
		btTransform btTrans;
		btTrans.setOrigin({ pos.x, pos.y, pos.z });
		btTrans.setRotation({ rot.x, rot.y, rot.z, rot.w });
		m_ghostObject.setWorldTransform(btTrans);

		//物理エンジンに登録。
		PhysicsWorld().AddCollisionObject(m_ghostObject);
		m_isRegistPhysicsWorld = true;
	}
}
