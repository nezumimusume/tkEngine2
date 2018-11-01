#pragma once

#include "tkEngine/Physics/tkICollider.h"

namespace tkEngine{
	/*!
	 * @brief	ボックスコライダー。
	 */
	class CBoxCollider : public ICollider
	{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CBoxCollider();
		/*!
		 * @brief	デストラクタ。
		 */
		~CBoxCollider();
		/*!
		 * @brief	ボックスコライダーを作成。
		 */
		void Create( const CVector3& size );
		btCollisionShape* GetBody() const override
		{
			return shape;
		}
	private:
		btBoxShape*		shape;
	};
}