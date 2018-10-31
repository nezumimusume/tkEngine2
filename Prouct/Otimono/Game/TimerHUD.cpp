#include "stdafx.h"
#include "TimerHUD.h"
#include "Game.h"

TimerHUD::TimerHUD()
{
}


TimerHUD::~TimerHUD()
{
	DeleteGO(m_fontRender);
}
bool TimerHUD::Start()
{
	m_fontRender = NewGO<prefab::CFontRender>(0);
	m_fontRender->SetShadowParam(true, 2.0f, CVector4::Black);
	m_game = FindGO<Game>(GAME_NAME);
	return true;
}
void TimerHUD::Update()
{
	float restTime = m_game->GetRestTime();
	//restTime‚ð•ª‚Æ•b‚É•ª‚¯‚éBB
	float min, sec;
	min = restTime / 60.0f;
	sec = fmod(restTime, 60.0f);

	wchar_t text[256];
	swprintf(text, L"Žc‚èŽžŠÔ %.f:%02.f", min, sec);
	m_fontRender->SetPosition({ -600.0f, 300.0f });
	m_fontRender->SetText(text);
}