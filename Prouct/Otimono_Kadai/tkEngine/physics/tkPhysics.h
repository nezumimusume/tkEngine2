#pragma once

#include "tkEngine/Physics/tkCollisionAttr.h"
#include "tkEngine/Physics/tkPhysicsDebugDraw.h"
#include "tkEngine/Physics/tkRigidBody.h"

namespace tkEngine{
	class CCharacterController;

	class CPhysicsWorld
	{
		std::unique_ptr<btDefaultCollisionConfiguration> 	 m_collisionConfig;
		std::unique_ptr<btCollisionDispatcher>				 m_collisionDispatcher;	//!<衝突解決処理。
		std::unique_ptr<btBroadphaseInterface>				 m_overlappingPairCache;	//!<ブロードフェーズ。衝突判定の枝切り。
		std::unique_ptr<btSequentialImpulseConstraintSolver> m_constraintSolver;		//!<コンストレイントソルバー。拘束条件の解決処理。
		std::unique_ptr<btDiscreteDynamicsWorld>			 m_dynamicWorld;			//!<ワールド。
#if BUILD_LEVEL!=BUILD_LEVEL_MASTER
		CPhysicsDebugDraw									 m_debugDraw;
#endif
	public:
		~CPhysicsWorld();
		void Init();
		void Update();
		void DebubDrawWorld(CRenderContext& rc);
		void Release();
		/*!
		* @brief	重力を設定。。
		*/
		void SetGravity(CVector3 g)
		{
			m_dynamicWorld->setGravity(btVector3(g.x, g.y, g.z));
		}
		/*!
		* @brief	ダイナミックワールドを取得。
		*/
		btDiscreteDynamicsWorld* GetDynamicWorld()
		{
			return m_dynamicWorld.get();
		}
		/*!
		* @brief	剛体を登録。
		*/
		void AddRigidBody(CRigidBody& rb)
		{
			if (rb.IsAddPhysicsWorld() == false) {
				m_dynamicWorld->addRigidBody(rb.GetBody());
				rb.SetMarkAddPhysicsWorld();
			}
		}
		/*!
		* @brief	デバッグ表示のモードを設定する。
		*@param[in]	debugMode		enum btIDebugDraw::DebugDrawModesを参照してください。
		*/
		void SetDebugDrawMode(int debugMode)
		{
#if BUILD_LEVEL!=BUILD_LEVEL_MASTER
			//ワイヤーフレーム描画のみ。
			m_debugDraw.setDebugMode(debugMode);
#endif
		}
		/*!
		* @brief	剛体を破棄。
		*/
		void RemoveRigidBody(CRigidBody& rb)
		{
			if (rb.IsAddPhysicsWorld() == true) {
				m_dynamicWorld->removeRigidBody(rb.GetBody());
				rb.SetUnmarkAddPhysicsWorld();
			}
		}
		/*!
		* @brief	コリジョンオブジェクトをワールドに登録。
		*@param[in]	colliObj	コリジョンオブジェクト。
		*/
		void AddCollisionObject(btCollisionObject& colliObj)
		{
			m_dynamicWorld->addCollisionObject(&colliObj);
		}
		/*!
		* @brief	コリジョンオブジェクトをワールドから削除。
		*@param[in]	colliObj	コリジョンオブジェクト。
		*/
		void RemoveCollisionObject(btCollisionObject& colliObj)
		{
			m_dynamicWorld->removeCollisionObject(&colliObj);
		}
		void ConvexSweepTest(
			const btConvexShape* castShape,
			const btTransform& convexFromWorld,
			const btTransform& convexToWorld,
			btCollisionWorld::ConvexResultCallback& resultCallback,
			btScalar allowedCcdPenetration = 0.0f
		)
		{
			m_dynamicWorld->convexSweepTest(castShape, convexFromWorld, convexToWorld, resultCallback, allowedCcdPenetration);
		}
		void ContactTest(
			btCollisionObject* colObj,
			std::function<void(const btCollisionObject& contactCollisionObject)> cb
		);
		void ContactTest(
			CRigidBody& rb,
			std::function<void(const btCollisionObject& contactCollisionObject)> cb
		);
		
		void ContactTest(
			CCharacterController& charaCon,
			std::function<void(const btCollisionObject& contactCollisionObject)> cb
		);
		
	};
	
}
