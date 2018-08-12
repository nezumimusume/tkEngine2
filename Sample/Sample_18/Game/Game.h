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
	void PostRender(CRenderContext& rc);
	CFont m_font;
	CVector2 m_position = CVector2::Zero;
	CVector4 m_color = CVector4::Gray;
};

