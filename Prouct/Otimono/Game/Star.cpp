#include "stdafx.h"
#include "Star.h"
#include "Player.h"
#include "Game.h"
#include "tkEngine/light/tkPointLight.h"

Star::Star()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/star/star.cmo");
	
	//星のx座標の最大座標と最小座標。
	const float STAR_X_POS_MAX = 1000.0f;
	const float STAR_X_POS_MIN = -1000.0f;
	//星のz座標の最大座標と最小座標。
	const float STAR_Z_POS_MAX = 1000.0f;
	const float STAR_Z_POS_MIN = -1000.0f;
	//星の開始時のY座標。
	const float STAR_START_Y_POS = 1000.0f;
	//座標をランダムに初期化する。
	
	//yは固定。
	m_position.y = STAR_START_Y_POS;
	//xとzをランダムに初期化する。
	auto& rnd = Random();
	m_position.x = CMath::Lerp(rnd.GetRandDouble(), STAR_X_POS_MIN, STAR_X_POS_MAX);
	m_position.z = CMath::Lerp(rnd.GetRandDouble(), STAR_Z_POS_MIN, STAR_Z_POS_MAX);
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale({ 40.0f, 40.0f, 40.0f });
	m_skinModelRender->SetShadowCasterFlag(true);
	m_skinModelRender->SetEmissionColor({ 2.9f, 2.9f, 0.0f });
	m_player = FindGO<Player>(PLAYER_NAME);
	m_game = FindGO<Game>(GAME_NAME);
	m_pointLight = NewGO<prefab::CPointLight>(0);
	m_pointLight->SetColor({ 20.3f, 20.3f, 20.0f});
	m_pointLight->SetAttn({ 200.0f, 2.0f, 0.0f });
}


Star::~Star()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_pointLight);
}

void Star::Update()
{
	//星が落下する速度。単位(cm/sec)
	const float STAR_FALL_SPEED = 200.0f;
	m_position.y -= STAR_FALL_SPEED * GameTime().GetFrameDeltaTime();
	m_skinModelRender->SetPosition(m_position);
	CQuaternion qAddRot;
	qAddRot.SetRotation(CVector3::AxisY, CMath::PI * GameTime().GetFrameDeltaTime());
	m_rotation *= qAddRot;
	m_skinModelRender->SetRotation(m_rotation);
	m_pointLight->SetPosition(m_position);
	//距離判定
	auto diff = m_player->GetPosition() - m_position;
	if (diff.Length() < 100.0f) {
		m_game->NotifyGetStar();
		//星を取得した音を鳴らす。
		auto ss = NewGO<prefab::CSoundSource>(0);
		ss->Init(L"sound/coinGet.wav");
		ss->Play(false);
		DeleteGO(this);
	}
	if (m_position.y < -10.0f) {
		//Y座標が一定値以下になった。
		DeleteGO(this);
	}
}
