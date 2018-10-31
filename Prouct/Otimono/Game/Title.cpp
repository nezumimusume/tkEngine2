#include "stdafx.h"
#include "Title.h"
#include "Game.h"

Title::Title()
{
}


Title::~Title()
{
	DeleteGO(m_spr);
}
bool Title::Start()
{
	m_spr = NewGO<prefab::CSpriteRender>(0);
	m_spr->Init(L"sprite/title.dds", 1280.0f, 720.0f);
	return true;
}
void Title::Update()
{
	if (Pad(0).IsPressAnyKey() == true) {
		NewGO<Game>(0, GAME_NAME);
		DeleteGO(this);
	}
}
