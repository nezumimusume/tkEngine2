#pragma once
class Timer : public IGameObject
{
public:
	void PostRender(CRenderContext& rc)override final;
private:
	CFont m_font;
	float m_timer = 0.0f;
};

