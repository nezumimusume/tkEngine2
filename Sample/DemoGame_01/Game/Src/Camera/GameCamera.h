/*!
 * @brief	ゲームカメラ。
 */
 
#pragma once

#include "tkEngine/camera/tkSpringCamera.h"

class CPlayer;

/*!
 * @brief	ゲームカメラ。
 */
class CGameCamera : public IGameObject{
public:
	bool Start() override final;
	void Update() override final;
	/*!
	 *@brief	視点を取得。
	 */
	CVector3 GetPosition() const
	{
		return m_springCamera.GetPosition();
	}
	/*!
	 *@brief	注視点を取得。
	 */
	CVector3 GetTarget() const
	{
		return m_springCamera.GetTarget();
	}
private:
	
	
private:
	CSpringCamera m_springCamera;				//!<バネカメラ。
	CPlayer* m_player = nullptr;				//!<プレイヤ。
	float m_toCameraPosDist;					//!<注視点から視点までの距離。
};