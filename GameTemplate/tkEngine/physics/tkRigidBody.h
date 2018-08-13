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
		}
		CVector3 pos;				//座標。
		CQuaternion rot;			//回転。
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
	};
}
