#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}


Player::~Player()
{
}
bool Player::Start()
{
	//アクターを検索。
	m_gameActor = FindGO<GameActor>("暗殺者");
	return true;
}
void Player::Update()
{
	if (m_gameActor->IsPlayRecord()) {
		//記録を再生中ならリターン。
		return;
	}
	//実習１　キーボードのAが押されたら左に旋回、
	//　　　　キーボードのDが押されたら右に旋回出来るようにしなさい。
	if (GetAsyncKeyState('W')) {
		//前に進む。
		m_gameActor->AddCommand(&m_commandWalk);
	}
	else {
		//何も押されていないので待機。
		m_gameActor->AddCommand(&m_commandIdle);
	}
}