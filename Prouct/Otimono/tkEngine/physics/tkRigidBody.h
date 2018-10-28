#pragma once

namespace tkEngine{

	class ICollider;

	//剛体情報。
	struct RigidBodyInfo {
		RigidBodyInfo() :
			collider(NULL),
			mass(0.0f)
		{
			pos = CVector3::Zero;
			rot = CQuaternion::Identity;
			localInteria = CVector3::Zero;
		}
		CVector3 pos;				//座標。
		CQuaternion rot;			//回転。
		CVector3 localInteria;		//慣性テンソル。
		ICollider* collider;		//コライダー。
		float mass;					//質量。

	};
	//剛体クラス。
	class CRigidBody
	{
		std::unique_ptr<btRigidBody>			m_rigidBody;		//剛体。
		std::unique_ptr<btDefaultMotionState>	m_myMotionState;	//モーションステート。
		bool m_isAddPhysicsWorld = false;			//物理ワールドに追加されている？
	public:
		~CRigidBody();
		void Release();
		void Create(RigidBodyInfo& rbInfo);
		btRigidBody* GetBody()
		{
			return m_rigidBody.get();
		}
		//物理ワールドに登録中の印をつける。
		void SetMarkAddPhysicsWorld()
		{
			m_isAddPhysicsWorld = true;
		}
		//物理ワールドに登録中の印を外す。
		void SetUnmarkAddPhysicsWorld()
		{
			m_isAddPhysicsWorld = false;
		}
		//物理ワールドに追加されている？
		bool IsAddPhysicsWorld() const
		{
			return m_isAddPhysicsWorld;
		}
		/*!
		* @brief	物理オブジェクトの座標と回転を取得
		*@param[out]	pos			座標。
		*@param[out]	rot			回転。
		*/
		void GetPositionAndRotation(CVector3& pos, CQuaternion& rot) const
		{
			btTransform trans;
			m_myMotionState->getWorldTransform(trans);
			pos.Set(trans.getOrigin());
			rot.Set(trans.getRotation());
		}
		/*!
		* @brief	物理オブジェクトの座標と回転を設定
		*@param[in]	pos			座標。
		*@param[in]	rot			回転。
		*/
		void SetPositionAndRotation(const CVector3& pos, const CQuaternion& rot)
		{
			btTransform trans;
			btVector3 btPos;
			pos.CopyTo(btPos);
			trans.setOrigin(btPos);
			btQuaternion btRot;
			rot.CopyTo(btRot);
			trans.setRotation(btRot);
			m_rigidBody->setWorldTransform(trans);
		}
		/*!
		* @brief	力を加える。
		*@param[out]	force		力。
		*@param[out]	relPos		力を加える座標。
		*/
		void AddForce(const CVector3& force, const CVector3& relPos)
		{
			btVector3 btForce;
			btVector3 btRelPos;
			force.CopyTo(btForce);
			relPos.CopyTo(btRelPos);
			m_rigidBody->applyForce(btForce, btRelPos);
		}
	};
}
