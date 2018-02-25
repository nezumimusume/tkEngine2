/*!
 *@brief	フラスタムカリング処理。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/culling/tkObjectFrustumCulling.h"

namespace tkEngine{
	/*!
	 *@brief	コンストラクタ。
	 */
	CObjectFrustumCulling::CObjectFrustumCulling()
	{
	}
	/*!
	 *@brief	デストラクタ。
	 */
	CObjectFrustumCulling::~CObjectFrustumCulling()
	{
	}
	/*!
	 *@brief	カリングアルゴリズムの実行。
	 */
	void CObjectFrustumCulling::Execute(const CAabb& aabb)
	{
		if (m_camera != nullptr) {
			const CMatrix& viewProjMatrix = m_camera->GetViewProjectionMatrix();
			SetCullingFlag(true);
			//AABBの８頂点をスクリーン空間の正規化座標系に変換する。
			//x、yが1.0～-1.0、zが0.0～1.0の範囲内にいたら画面内にいるということになる。
			//8頂点すべてが画面の外ならカリングする。
			for (int i = 0; i < 8; i++) {
				CVector4 vertPos(aabb.GetVertexPosition(i));
				viewProjMatrix.Mul(vertPos);
				vertPos.x /= vertPos.w;
				vertPos.y /= vertPos.w;
				vertPos.z /= vertPos.w;
				if (vertPos.x >= -1.0f
					&& vertPos.x <= 1.0f
					&& vertPos.y >= -1.0f
					&& vertPos.y <= 1.0f
					&& vertPos.z >= 0.0f
					&& vertPos.z <= 1.0f
					) {
					//画面内にいる
					SetCullingFlag(false);
					break;
				}
			}
		}
	}
}
