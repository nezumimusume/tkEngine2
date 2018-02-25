/*!
 * @brief	プレイヤーの旋回処理。
 */

#include "stdafx.h"
#include "Player/PlayerTurn.h"
#include "Player/Player.h"

namespace {
	const float ROT_SPEED = 8.0f;	//回転速度。単位(ラジアン/秒)。
}
/*!
 * @brief	更新。
 */
void CPlayerTurn::Update()
{
	CVector3 moveSpeed = m_player->GetMoveSpeed();
	moveSpeed.y = 0.0f;	//yは不要。
	if (moveSpeed.LengthSq() < 0.01f) {
		//移動していない。
		return;
	}
	CVector3 forwardXZ = m_player->GetForwardXZ();
	float targetAngle = atan2f(moveSpeed.x, moveSpeed.z);
	float currentAngle = atan2f(forwardXZ.x, forwardXZ.z);

	float diff = targetAngle - currentAngle;
	float a_diff = fabsf(diff);
	if (a_diff > CMath::PI) {
		//180°を超えて大回りしているので、-180.0f～180.0fにラップする。
		diff = (CMath::PI2 - a_diff) * (a_diff / -diff);
		a_diff = fabsf(diff);
	}
	float rotSpeed = ROT_SPEED * GameTime().GetFrameDeltaTime();
	float addAngle = 0.0f;
	if (a_diff > rotSpeed) {
		addAngle = (diff/a_diff) * rotSpeed;
	}
	else {
		addAngle = diff;
	}

	CQuaternion qAddRot;
	qAddRot.SetRotation(CVector3::AxisY, addAngle);
	CQuaternion qRot = m_player->GetRotation();
	qRot.Multiply(qRot, qAddRot);
	m_player->SetRotation(qRot);
}
