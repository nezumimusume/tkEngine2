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
		/*!
		* @brief	フォントを設定。
		* @details	使用するフォントを設定できます。
		*  デフォルトのフォントを使用する場合は、この関数を使用する必要はありません。
		*  フォントを変更したい場合に使用してください。
		*@param[in]	font	フォント。
		*/
		void SetFont(DirectX::SpriteFont* font)
		{
			m_font->SetFont(font);
		}
	private:
		std::unique_ptr<CFont>	m_font;
		std::wstring m_text;
		CVector2 m_position = CVector2::Zero;
	};
}