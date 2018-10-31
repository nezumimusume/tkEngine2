#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/Physics/tkRigidBody.h"
#include "tkEngine/Physics/tkICollider.h"

using namespace std;

namespace tkEngine{


	CRigidBody::~CRigidBody()
	{
		Release();
	}
	void CRigidBody::Release()
	{
	}
	void CRigidBody::Create(RigidBodyInfo& rbInfo)
	{
		Release();
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(rbInfo.pos.x, rbInfo.pos.y, rbInfo.pos.z));
		transform.setRotation(btQuaternion(rbInfo.rot.x, rbInfo.rot.y, rbInfo.rot.z, rbInfo.rot.w));
		m_myMotionState = make_unique<btDefaultMotionState>();
		m_myMotionState->setWorldTransform(transform);
		btVector3 btLocalInteria;
		rbInfo.localInteria.CopyTo(btLocalInteria);
		btRigidBody::btRigidBodyConstructionInfo btRbInfo(rbInfo.mass, m_myMotionState.get(), rbInfo.collider->GetBody(), btLocalInteria);
		//çÑëÃÇçÏê¨ÅB
		m_rigidBody = make_unique<btRigidBody>(btRbInfo);
	}
}