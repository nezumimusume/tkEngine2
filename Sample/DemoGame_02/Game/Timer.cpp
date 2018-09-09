#include "stdafx.h"
#include "timer.h"


void Timer::PostRender(CRenderContext& rc)
{
	m_timer += GameTime().GetFrameDeltaTime();
	wchar_t text[256];
	float minute = min( 59.0f, m_timer / 60.0f);	//•b”‚ğ”²‚«o‚·B
	float sec = min(59.0f, fmodf(m_timer, 60.0f));
	float mill = min(99.0f, fmodf(sec*100.0f, 100.0f));
	swprintf(text, L"%02.0f:%02.0f:%02.0f", minute, sec, mill);
	m_font.Begin(rc);
	m_font.Draw(
		text,
		{ 620.0f, 340.0f }, 
		CVector4::White,
		0.0f,
		1.0,
		{1.0f, 1.0f}
	);
	m_font.End(rc);
}