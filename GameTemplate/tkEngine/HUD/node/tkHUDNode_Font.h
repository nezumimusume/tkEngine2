/*!
 *@brief	フォントノード。
 */

#pragma once

#include "tkEngine/HUD/tkIHUDNode.h"

namespace tkEngine{
	class CHUDNode_Font : public IHUDNode{
	public:
		CHUDNode_Font(CBone* bone);
		void Update() override;
		void PostRender( CRenderContext& rc ) override;
		/*!
		*@brief	テキストを設定。
		*/
		void SetText(const wchar_t* text)
		{
			m_text = text;
		}
	private:
		std::unique_ptr<CFont>	m_font;
		std::wstring m_text;
		CVector2 m_position = CVector2::Zero;
	};
}