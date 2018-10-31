/*!
 * @brief	静的オブジェクト。
 */


#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/physics/tkPhysicsStaticObject.h"
#include "tkEngine/physics/tkBoxCollider.h"

using namespace std;
namespace tkEngine{
	
	void CPhysicsStaticObject::Release()
	{
		PhysicsWorld().RemoveRigidBody(m_rigidBody);
	}
	void CPhysicsStaticObject::CreateCommon(CVector3 pos, CQuaternion rot)
	{
		RigidBodyInfo rbInfo;
		rbInfo.collider = m_collider.get();
		rbInfo.mass = 0.0f;
		rbInfo.pos = pos;
		rbInfo.rot = rot;
		m_rigidBody.Create(rbInfo);
		PhysicsWorld().AddRigidBody(m_rigidBody);
	}
}