#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/Physics/tkPhysics.h"
#include "tkEngine/character/tkCharacterController.h"

using namespace std;
namespace tkEngine{
	namespace {
		struct MyContactResultCallback : public btCollisionWorld::ContactResultCallback {
			using ContantTestCallback = function<void(const btCollisionObject& contactCollisionObject)>;
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
	
	CPhysicsWorld::~CPhysicsWorld()
	{
		Release();
	}
	void CPhysicsWorld::Release()
	{
		m_dynamicWorld.reset();
		m_constraintSolver.reset();
		m_constraintSolver.reset();
		m_overlappingPairCache.reset();
		m_collisionDispatcher.reset();
		m_collisionConfig.reset();
	}
	void CPhysicsWorld::Init()
	{
		Release();
		//ï®óùÉGÉìÉWÉìÇèâä˙âªÅB
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		m_collisionConfig = make_unique<btDefaultCollisionConfiguration>();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		m_collisionDispatcher = make_unique<btCollisionDispatcher>(m_collisionConfig.get());

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		m_overlappingPairCache = make_unique<btDbvtBroadphase>();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		m_constraintSolver = make_unique<btSequentialImpulseConstraintSolver>();

		m_dynamicWorld = make_unique<btDiscreteDynamicsWorld>(
			m_collisionDispatcher.get(),
			m_overlappingPairCache.get(),
			m_constraintSolver.get(),
			m_collisionConfig.get()
		);

		m_dynamicWorld->setGravity(btVector3(0, -10, 0));
#if BUILD_LEVEL!=BUILD_LEVEL_MASTER
		m_debugDraw.Init();
		m_dynamicWorld->setDebugDrawer(&m_debugDraw);
#endif 
	}
	void CPhysicsWorld::Update()
	{
		m_dynamicWorld->stepSimulation(GameTime().GetFrameDeltaTime());
	}
	void CPhysicsWorld::DebubDrawWorld(CRenderContext& rc)
	{
#if BUILD_LEVEL!=BUILD_LEVEL_MASTER
		m_debugDraw.BeginDraw(rc);
		m_dynamicWorld->debugDrawWorld();
		m_debugDraw.EndDraw();
#endif
	}
	void CPhysicsWorld::ContactTest(
		btCollisionObject* colObj,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	) {
		MyContactResultCallback myContactResultCallback;
		myContactResultCallback.m_cb = cb;
		myContactResultCallback.m_me = colObj;
		m_dynamicWorld->contactTest(colObj, myContactResultCallback);
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