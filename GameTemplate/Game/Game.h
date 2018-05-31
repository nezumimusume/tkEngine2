#pragma once

#include "tkEngine/graphics/effect/tkEffect.h"

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(CRenderContext& rc);
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//スキンモデルレンダラー。
};

