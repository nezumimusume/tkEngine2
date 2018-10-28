/*!
 * @brief	静的オブジェクト。
 */

#pragma once

#include "tkEngine/physics/tkMeshCollider.h"
#include "tkEngine/physics/tkRigidBody.h"
#include "tkEngine/physics/tkPhysicsObjectBase.h"


namespace tkEngine{
	/*!
	 * @brief	静的物理オブジェクト
	 */
	class CPhysicsStaticObject : public CPhysicsObjectBase{
	public:
		~CPhysicsStaticObject()
		{
			Release();
		}
		/*!
		* @brief	解放。
		*/
		void Release() override final;
		/*!
		* @brief	座標と回転を設定。
		*/
		void SetPositionAndRotation(const CVector3& pos, const CQuaternion& rot)
		{
			m_rigidBody.SetPositionAndRotation(pos, rot);
		}
		CRigidBody& GetRigidBody()
		{
			return m_rigidBody;
		}
	private:
		
		/*!
		* @brief	静的物理オブジェクト作成処理の共通処理。
		*/
		void CreateCommon(CVector3 pos, CQuaternion rot) override final;
	private:
		CRigidBody m_rigidBody;				//!<剛体。
	};
}