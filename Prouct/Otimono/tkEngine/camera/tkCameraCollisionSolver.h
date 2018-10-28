/*!
 *@brief	三人称視点のゲームカメラのコリジョンソルバー
 */

#pragma once

#include "tkEngine/Physics/tkSphereCollider.h"

namespace tkEngine{
	/*!
	 *@brief	カメラのコリジョンソルバー。
	 *@details
	 * 一般的な三人称視点のゲームカメラの衝突解決を提供します。\n
	 * カメラを背景にめり込ませたくない場合などに使用してください。
	 *@code
		//典型的な使用例。
		int main()
		{
				・
				・
				省略
				ゲームの初期化などを行う。。
				・
				・
			CCameraCollisionSolver cameraCollisionSolver;
			//カメラのコリジョンソルバーを初期化する。
			cameraCollisionSolver.Init( 50.0f );
			while(true){	//ゲームループ
				・
				・
				省略
				このフレームのカメラの視点や注意点を計算したりする。
				・
				・
				CVector3 newCameraPosition;	//新しいカメラの視点の格納先。
				cameraCollisionSolver.Execute( 
					newCameraPosition,		//第一引数は衝突解決後の視点の座標の格納先。
					cameraPosition,			//第二引数は衝突解決前の視点の座標。
					cameraTarget,			//第三引数は注視点の座標。
				//衝突解決した座標をカメラの座標として設定する。
				MainCamera().SetPosition(newCameraPosition);
			}

			return 0;
		}
	 *@endcode
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
		 *@param[out]	result		コリジョン解決を行っ視点の座標が格納される。
		 *@param[in]	position	コリジョン解決を行う前の視点の座標。
		 *@param[in]	target		カメラの注視点。
		 *@return	コリジョン解決を行った場合はtrueが返ってくる。
		 */
		bool Execute(CVector3& result, const CVector3& position, const CVector3& target);
	private:
		CSphereCollider		m_collider;			 //コライダー。
		float				m_radius = 0.0f;	//半径。
		bool				m_isInited = false;	//初期化済みのフラグ。trueになっていたら初期化済み。
	};
}