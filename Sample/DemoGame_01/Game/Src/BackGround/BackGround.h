/*!
 * @brief	背景
 */

#pragma once

#include "tkEngine/physics/tkPhysicsStaticObject.h"

class CBackGround : public IGameObject{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	CBackGround();
	/*!
	 * @brief	デストラクタ。
	 */
	~CBackGround();
	/*!
	 * @brief	スタート。
	 */
	bool Start() override;
private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CPhysicsStaticObject m_phyStaticObject;	//!<静的物理オブジェクト。
};