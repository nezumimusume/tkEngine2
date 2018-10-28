/*!
 * @brief	フォントレンダラー。
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/font/tkFontRender.h"

namespace tkEngine {
namespace prefab {
	void CFontRender::PostRender(CRenderContext& rc)
	{
		m_font.Begin(rc);
		const wchar_t* text = nullptr;
		if (m_textUnsafe != nullptr) {
			text = m_textUnsafe;
		}
		else if (m_text.c_str() != nullptr){
			text = m_text.c_str();
		}
		m_font.Draw(
			text,
			m_position,
			m_color,
			m_rotation,
			m_scale,
			m_pivot);
		m_font.End(rc);
	}
}
}