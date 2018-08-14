/*!
 * @brief	動的物理オブジェクト。
 */

#pragma once

#include "tkEngine/physics/tkMeshCollider.h"
#include "tkEngine/physics/tkRigidBody.h"

namespace tkEngine{
	/*!
	 * @brief	動的物理オブジェクト
	 *@details
	 * 動くぜー
	 */
	class CPhysicsDynamicObject{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CPhysicsDynamicObject();
		/*!
		 * @brief	デストラクタ。
		 */
		~CPhysicsDynamicObject();
		/*!
		 * @brief	メッシュの静的オブジェクトを作成。
		 *@param[in]	skinModel	スキンモデル。
		 *@param[in]	pos			座標。
		 *@param[in]	rot			回転。
		 *@param[in]	scale		拡大率。
		 */
		void CreateMeshObject(CSkinModel& skinModel, CVector3 pos, CQuaternion rot, CVector3 scale);
		void CreateMeshObject(prefab::CSkinModelRender* skinModelRender, CVector3 pos, CQuaternion rot, CVector3 scale);
	private:
		CMeshCollider m_meshCollider;		//!<メッシュコライダー。
		CRigidBody m_rigidBody;				//!<剛体。
	};
}