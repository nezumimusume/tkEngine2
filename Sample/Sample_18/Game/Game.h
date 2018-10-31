#pragma once

#include "tkEngine/graphics/effect/tkEffect.h"
#include "tkEngine/debug/math/tkVectorRender.h"

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	prefab::CFontRender* m_fontRender = nullptr;	//フォントレンダラー。
	CVector2 m_position = CVector2::Zero;
	CVector4 m_color = CVector4::Gray;
};

