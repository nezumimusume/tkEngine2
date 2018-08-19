#include "stdafx.h"
#include "Title.h"
#include "Game.h"

Title::Title()
{
}


Title::~Title()
{
	DeleteGO(m_spriteRender);
}
bool Title::Start()
{
	m_spriteRender = NewGO<prefab::CSpriteRender>(0);
	m_spriteRender->Init(L"sprite/title.dds", 1280.0f, 720.0f);
	return true;
}
void Title::Update()
{
	if (Pad(0).IsPress(enButtonA) == true) {
		//‚à‚µ‚àAƒ{ƒ^ƒ“‚ª‰Ÿ‚³‚ê‚½‚çB
		NewGO<Game>(0, "Game");
		DeleteGO(this);
	}
}