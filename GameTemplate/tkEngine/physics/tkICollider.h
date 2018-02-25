/*!
 * @brief	コライダー。
 */

#pragma once

class btCollisionShape;
namespace tkEngine{
	/*!
	 * @brief	コライダーのインターフェースクラス。
	 */
	class ICollider {
	public:
		virtual btCollisionShape* GetBody() const = 0;
	};
}