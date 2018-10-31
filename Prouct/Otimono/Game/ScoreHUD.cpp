#include "stdafx.h"
#include "ScoreHUD.h"
#include "GameObjectName.h"
#include "Game.h"

ScoreHUD::ScoreHUD()
{
}


ScoreHUD::~ScoreHUD()
{
	DeleteGO(m_fontRender);
}
bool ScoreHUD::Start()
{
	m_fontRender = NewGO<prefab::CFontRender>(0);
	m_fontRender->SetShadowParam(true, 2.0f, CVector4::Black);
	m_game = FindGO<Game>(GAME_NAME);
	return true;
}
void ScoreHUD::Update()
{
	static const CVector2 normalPos = { 320.0f, 300.0f };
	switch (m_game->GetState()) {
	case Game::enState_Playing: {
		wchar_t text[256];
		swprintf(text, L"Žæ‚Á‚½¯‚Ì” \n"
			"          %02d", m_game->GetStarCount());
		m_fontRender->SetText(text);
		m_fontRender->SetPosition({ 320.0f, 300.0f });
		m_fontRender->SetPivot({ 0.0f, 0.0f });
	}break;
	case Game::enState_Timeup: {
		m_timer = min( 1.0f, m_timer + 1.0f * GameTime().GetFrameDeltaTime() );
		auto pos = CMath::Lerp(m_timer, normalPos, { 0.0f, 0.0f });
		auto pivot = CMath::Lerp<CVector2>(m_timer, { 0.0f, 0.0f }, { 0.5f, 0.5f });
		m_fontRender->SetPosition(pos);
		m_fontRender->SetPivot(pivot);
	}break;
	}
}