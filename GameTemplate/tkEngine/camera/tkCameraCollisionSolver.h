/*!
 *@brief	三人称視点のゲームカメラのコリジョンソルバー
 */

#pragma once

#include "tkEngine/Physics/tkSphereCollider.h"

namespace tkEngine{
	/*!
	 *@brief	カメラのコリジョンソルバー。
	 *@details
	 * 一般的な三人称視点のゲームカメラのコリジョンソルバーを提供します。
	 */
	class CCameraCollisionSolver{
	public:
		/*!
		 *@brief	コンストラクタ。
		 */
		CCameraCollisionSolver();
		/*!
		 *@brief	デストラクタ。
		 */
		~CCameraCollisionSolver();
		/*!
		 *@brief	初期化。
		 *@param[in]	radius		カメラのバウンディングスフィアの半径。
		 */
		void Init(float radius);
		/*!
		 *@brief	コリジョン解決の実行。
		 *@param[out]	result		コリジョン解決を行った結果のカメラの視点の座標が格納される。
		 *@param[in]	position	カメラの視点。
		 *@param[in]	target		カメラの注視点。
		 *@return	コリジョン解決を行った場合はtrueが返ってくる。
		 */
		bool Execute(CVector3& result, const CVector3& position, const CVector3& target);
	private:
		CSphereCollider		m_collider;		 //コライダー。
		float				m_radius = 0.0f; //半径。
	};
}