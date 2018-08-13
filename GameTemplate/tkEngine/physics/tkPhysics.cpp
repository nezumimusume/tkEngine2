#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/Physics/tkPhysics.h"
#include "tkEngine/Physics/tkRigidBody.h"
#include "tkEngine/character/tkCharacterController.h"

namespace tkEngine{
	namespace {
		struct MyContactResultCallback : public btCollisionWorld::ContactResultCallback {
			using ContantTestCallback = std::function<void(const btCollisionObject& contactCollisionObject)>;
			ContantTestCallback  m_cb;
			btCollisionObject* m_me = nullptr;
			virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
			{
				if (m_me == colObj0Wrap->getCollisionObject()) {
					m_cb(*colObj1Wrap->getCollisionObject());
				}
				return 0.0f;
			}
		};
	}
	CPhysicsWorld::CPhysicsWorld()
	{
		collisionConfig = NULL;
		collisionDispatcher = NULL;
		overlappingPairCache = NULL;
		constraintSolver = NULL;
		dynamicWorld = NULL;
	}


	CPhysicsWorld::~CPhysicsWorld()
	{
		Release();
	}
	void CPhysicsWorld::Release()
	{
		delete dynamicWorld;
		delete constraintSolver;
		delete overlappingPairCache;
		delete collisionDispatcher;
		delete collisionConfig;

		dynamicWorld = nullptr;
		constraintSolver = nullptr;
		overlappingPairCache = nullptr;
		collisionDispatcher = nullptr;
		collisionConfig = nullptr;
	}
	void CPhysicsWorld::Init()
	{
		Release();
		//•¨—ƒGƒ“ƒWƒ“‚ð‰Šú‰»B
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		collisionConfig = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		collisionDispatcher = new	btCollisionDispatcher(collisionConfig);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		overlappingPairCache = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		constraintSolver = new btSequentialImpulseConstraintSolver;

		dynamicWorld = new btDiscreteDynamicsWorld(
			collisionDispatcher,
			overlappingPairCache,
			constraintSolver,
			collisionConfig
			);

		dynamicWorld->setGravity(btVector3(0, -10, 0));
	}
	void CPhysicsWorld::Update()
	{
		dynamicWorld->stepSimulation(GameTime().GetFrameDeltaTime());
	}
	void CPhysicsWorld::AddRigidBody(CRigidBody& rb)
	{
		dynamicWorld->addRigidBody(rb.GetBody());
	}
	void CPhysicsWorld::RemoveRigidBody(CRigidBody& rb)
	{
		dynamicWorld->removeRigidBody(rb.GetBody());
	}
	void CPhysicsWorld::ContactTest(
		btCollisionObject* colObj,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	) {
		MyContactResultCallback myContactResultCallback;
		myContactResultCallback.m_cb = cb;
		myContactResultCallback.m_me = colObj;
		dynamicWorld->contactTest(colObj, myContactResultCallback);
	}

	void CPhysicsWorld::ContactTest(
		CRigidBody& rb,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	)
	{
		ContactTest(rb.GetBody(), cb);
	}
	void CPhysicsWorld::ContactTest(
		CCharacterController& charaCon,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	)
	{
		ContactTest(*charaCon.GetRigidBody(), cb);
	}
}