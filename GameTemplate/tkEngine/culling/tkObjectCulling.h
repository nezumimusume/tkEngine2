/*!
 *@brief	オブジェクトのカリング処理のインターフェースクラス。
 */

#pragma once

#include "tkEngine/math/tkAabb.h"

namespace tkEngine{
	/*!
	 *@brief	CPUでの早期カリング処理のインターフェースクラス。
	 *@details
	 * Strategyパターンを使用して設計されています。
	 */
	class IObjectCulling{
	public:
		/*!
		 *@brief	コンストラクタ。
		 */
		IObjectCulling() {}
		/*!
		 *@brief	デストラクタ。
		 */
		virtual ~IObjectCulling() {}
		/*!
		 *@brief	カリングアルゴリズムを実行。
		 */
		virtual void Execute(const CBox& aabb) = 0;
		/*!
		 *@brief	カリングされているか判定。
		 */
		bool IsCulling() const
		{
			return m_isCulling;
		}
	protected:
		/*!
		 *@brief	カリングフラグを設定。
		 */
		void SetCullingFlag( bool flag )
		{
			m_isCulling = flag;
		}
	private:
		bool	m_isCulling = false;
	};
}