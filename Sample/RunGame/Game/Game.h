#pragma once
#include "GameCamera.h"
#include "Player.h"
#include "Background.h"
#include "Star.h"

class Game : public IGameObject
{
public:
	Game();
	~Game();
	
	void Update() override;

	GameCamera* m_gameCamera = nullptr;
	Player* m_player = nullptr;
	Background* m_background = nullptr;
	Star* m_star[5] = { nullptr };
	int m_numGetStarCount = 0;
	bool m_isClear = false;
	prefab::CSpriteRender* m_clearSpriteRender = nullptr;
	int m_timer = 0;
};

