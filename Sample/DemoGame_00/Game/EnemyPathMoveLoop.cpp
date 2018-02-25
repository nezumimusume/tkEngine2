/*!
 * @brief	敵のパス移動(ループ)思考。
 */

#include "stdafx.h"
#include "EnemyPathMoveLoop.h"
#include "Path.h"
#include "Enemy.h"

EnemyPathMoveLoop::EnemyPathMoveLoop(Enemy* enemy, Path& path) :
	m_enemy(enemy),
	m_path(&path)
{
}
EnemyPathMoveLoop::~EnemyPathMoveLoop()
{
}
void EnemyPathMoveLoop::ChangeTurnState(EnState stateWhenTurnEnd, const CVector3& targetDir)
{
	//エネミーの前方方向を取得。
	const CVector3& forward = m_enemy->GetForward();
	m_angleWhenStartTurn = atan2f(forward.x, forward.z);
	m_angleWhenEndTurn = atan2f(targetDir.x, targetDir.z);
	m_stateWhenTurnEnd = stateWhenTurnEnd;
	//角度の差が180.0以上ないか調べる。
	float diff = m_angleWhenEndTurn - m_angleWhenStartTurn;
	float diff_abs = fabsf(diff);
	if (diff_abs > CMath::PI) {
		//差が180.0f以上なので大回りしている。
		float angle = 2.0f * CMath::PI - fabsf(diff);
		angle *= -diff / diff_abs;
		m_angleWhenEndTurn = m_angleWhenStartTurn + angle;
	}
	m_angle = m_angleWhenStartTurn;
	ChangeState(enState_Turn);
}
void EnemyPathMoveLoop::Update()
{
	float deltaTime = GameTime().GetFrameDeltaTime();
	switch (m_state) {
	case enState_FindNearEdge: {
		//近いエッジを検索する。
		auto* edge = m_path->GetNearEdge(m_enemy->GetPosition());
		if (edge != nullptr) {
			
			m_enemy->PlayAnimation(Enemy::enAnimationClip_run);
			m_currentEdge = edge;
			//ターン。
			CVector3 toEdgeVector = m_currentEdge->CalcVectorToEdge(m_enemy->GetPosition());
			toEdgeVector.y = 0.0f;
			CVector3 toEdgeVectorDir = toEdgeVector;
			toEdgeVectorDir.Normalize();
			ChangeTurnState(enState_MoveNearEdge, toEdgeVectorDir);
			
		}
	}break;
	case enState_MoveNearEdge: { //最も近いエッジに移動する。
		
		CVector3 toEdgeVector = m_currentEdge->CalcVectorToEdge(m_enemy->GetPosition());
		toEdgeVector.y = 0.0f;
		CVector3 toEdgeVectorDir = toEdgeVector;
		toEdgeVectorDir.Normalize();
		float move = m_enemy->MOVE_SPEED * deltaTime;
		if (toEdgeVector.Length() < m_enemy->RADIUS) {
			move = toEdgeVector.Length();
			//順方向への移動に切り替え。
			//まずターンさせる。
			ChangeTurnState(enState_MovePath, m_currentEdge->directionXZ);
		}
		m_enemy->Move(deltaTime, toEdgeVectorDir * move / deltaTime);
		
	}break;
	case enState_MovePath: {	//順方向のパス移動。
		const PathEdge* edge = m_currentEdge;
		//移動量を計算。
		float move = m_enemy->MOVE_SPEED * deltaTime;
		CVector3 moveSpeed = edge->directionXZ;
		moveSpeed *= move;
		//次の移動先を計算。
		CVector3 nextPos = m_enemy->GetPosition() + moveSpeed;

		//終点までの距離を計算。
		CVector3 toEndPoint = m_currentEdge->endPos - nextPos;
		toEndPoint.y = 0.0f; //Yはいらない。
		if (toEndPoint.Dot(m_currentEdge->directionXZ) < 0.0f) {
			//終点を超えた。
			nextPos = m_currentEdge->endPos;
			nextPos.y = m_enemy->GetPosition().y;
			//次のエッジへゴー。
			int nextEdgeNo = (m_currentEdge->no + 1);
			if (nextEdgeNo == m_path->GetNumEdge()) {
				//逆方向への移動に切り替え。
				ChangeTurnState(enState_MovePath_Reverse, m_currentEdge->directionXZ * -1.0f);
			}
			else {
				m_currentEdge = m_path->GetEdge(nextEdgeNo);
				//ターン。
				ChangeTurnState(m_state, m_currentEdge->directionXZ);
			}
		}
		m_enemy->Move(deltaTime, (nextPos-m_enemy->GetPosition()) / deltaTime);
	}break;
	case enState_MovePath_Reverse: {	//逆方向のパス移動。
		const PathEdge* edge = m_currentEdge;
		//移動量を計算。
		float move = m_enemy->MOVE_SPEED * GameTime().GetFrameDeltaTime();
		CVector3 moveSpeed = edge->directionXZ;
		moveSpeed *= -move;
		//次の移動先を計算。
		CVector3 nextPos = m_enemy->GetPosition() + moveSpeed;

		//始点までの距離を計算。
		CVector3 toStartPoint = m_currentEdge->startPos - nextPos;
		toStartPoint.y = 0.0f; //Yはいらない。
		if (toStartPoint.Dot(m_currentEdge->directionXZ) > 0.0f) {
			//始点を超えた。
			
			CVector3 pos = m_currentEdge->startPos;
			nextPos.y = m_enemy->GetPosition().y;

			//次のエッジへゴー。
			int nextEdgeNo = (m_currentEdge->no - 1);
			if (nextEdgeNo < 0) {
				//順方向への移動に切り替え。
				//まずターンさせる。
				ChangeTurnState(enState_MovePath, m_currentEdge->directionXZ);
			}
			else {
				m_currentEdge = m_path->GetEdge(nextEdgeNo);
				//ターン。
				ChangeTurnState(m_state, m_currentEdge->directionXZ * -1.0f);
			}
		}
	
		m_enemy->Move(deltaTime, (nextPos - m_enemy->GetPosition())/ deltaTime);
	}break;
	case enState_Turn: {
		
		if (m_angleWhenStartTurn < m_angleWhenEndTurn) {
			//順方向に回す。
			m_angle = m_angle + 0.1f;
			if (m_angle > m_angleWhenEndTurn) {
				m_angle = m_angleWhenEndTurn;
				//旋回終わり。
				ChangeState(m_stateWhenTurnEnd);
			}
		}
		else {
			//逆方向に回す。
			m_angle = m_angle - 0.1f;
			if (m_angle < m_angleWhenEndTurn) {
				m_angle = m_angleWhenEndTurn;
				//旋回終わり。
				ChangeState(m_stateWhenTurnEnd);
			}
		}
		CQuaternion qRot;
		qRot.SetRotation(CVector3::AxisY, m_angle);
		m_enemy->SetRotation(qRot);
		
	}break;
	}
	
}