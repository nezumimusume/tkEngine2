/*!
 * @brief	静的オブジェクト。
 */

#pragma once

#include "tkEngine/physics/tkMeshCollider.h"
#include "tkEngine/physics/tkRigidBody.h"

namespace tkEngine{
	/*!
	 * @brief	静的物理オブジェクト
	 */
	class CPhysicsStaticObject{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CPhysicsStaticObject();
		/*!
		 * @brief	デストラクタ。
		 */
		~CPhysicsStaticObject();
		/*!
		 * @brief	メッシュの静的オブジェクトを作成。
		 *@param[in]	skinModel	スキンモデル。
		 *@param[in]	pos			座標。
		 *@param[in]	rot			回転。
		 */
		void CreateMeshObject(CSkinModel& skinModel, CVector3 pos, CQuaternion rot);
		void CreateMeshObject(prefab::CSkinModelRender* skinModelRender, CVector3 pos, CQuaternion rot);
	private:
		CMeshCollider m_meshCollider;		//!<メッシュコライダー。
		CRigidBody m_rigidBody;				//!<剛体。
	};
}