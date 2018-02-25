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
	/*!
	 * @brief	更新。
	 */
	void Update() override;
	/*!
	 * @brief	描画。
	 */
	void Render(CRenderContext& rc) override;
private:
	CSkinModel	m_skinModel;				//!<モデル。
	CSkinModelData m_skinModelData;			//!<モデルデータ。
	CPhysicsStaticObject m_phyStaticObject;	//!<静的物理オブジェクト。
};