#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"
#include "tkEngine/sound/tkSoundSource.h"
Title::Title()
{
}


Title::~Title()
{
}
void Title::OnDestroy()
{
	DeleteGO(m_bgm);
}
bool Title::Start()
{
	m_bgm = NewGO<prefab::CSoundSource>(0);
	m_bgm->Init(L"sound/title.wav");
	m_bgm->Play(true);
	m_intiBgmVolume = m_bgm->GetVolume();
	m_texture.CreateFromDDSTextureFromFile(L"sprite/title.dds");
	m_sprite.Init(m_texture, GraphicsEngine().Get2DSpaceScreenWidth(), GraphicsEngine().Get2DSpaceScreenHeight());
	m_fade = FindGO<Fade>("Fade");
	
	m_font.SetShadowParam(true, 1.0f, {26.0f/255.0f, 49/255.0f, 103/255.0f, 1.0f});
	m_fade->StartFadeIn();
	return true;
}
void Title::Update()
{
	if (m_isWaitFadeout) {
	//	m_bgm->SetVolume(m_intiBgmVolume*(1.0f-m_fade->GetCurrentAlpha()));
		if (!m_fade->IsFade()) {
			NewGO<Game>(0, "Game");
			DeleteGO(this);
		}
	}
	else {
		if (Pad(0).IsPressAnyKey()) {
			prefab::CSoundSource* s = NewGO<prefab::CSoundSource>(0);
			s->Init(L"sound/press_key.wav");
			s->SetVolume(2.0f);
			s->Play(false);
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
		}
	}
}
void Title::PostRender(CRenderContext& rc)
{
	m_sprite.Draw(rc, MainCamera2D().GetViewMatrix(), MainCamera2D().GetProjectionMatrix());
	//press any keyÇï\é¶ÅB
	m_font.Begin(rc);
	m_font.Draw(L"PRESS ANY KEY", { 0.0f, -270.0f }, { 234.0f/255.0f, 69.0f/255.0f, 21.0f/255.0f, 1.0f}, 0.0f, 1.0f);
	m_font.End(rc);
}
