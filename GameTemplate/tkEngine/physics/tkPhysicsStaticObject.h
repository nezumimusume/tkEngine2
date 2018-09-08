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
	private:
		
		/*!
		* @brief	静的物理オブジェクト作成処理の共通処理。
		*/
		void CreateCommon(CVector3 pos, CQuaternion rot) override final;
	private:
		CRigidBody m_rigidBody;				//!<剛体。
	};
}