/*!
 * @brief	フォント
 */

#pragma once

namespace tkEngine{
	/*!
	 * @brief	フォント。
	 */
	class CFont final{
	public:
		CFont();
		~CFont();
		/*!
		* @brief	フォントを設定。
		* @details	デフォルトフォントから変更したい場合は
		*  DirectX::SpriteFontを使用して、フォントをロードして差し替えを行ってください。
		*/
		void SetFont(DirectX::SpriteFont* font)
		{
			m_spriteFont = font;
		}
		/*!
		 * @brief	描画開始。
		 */
		void Begin(CRenderContext& rc);
		/*!
		 * @brief	描画終了。
		 */
		void End(CRenderContext& rc);
		/*!
		 * @brief	描画。
		 *@param[in]	text		表示したいテキスト。
		 *@param[in]	trans		平行移動。
		 *@param[in]	color		カラー。
		 *@param[in]	rot			回転。
		 *@param[in]	scale		拡大。
		 *@param[in]	pivot		基点。
		 *						0.5, 0.5で画像の中心が基点。
		 *						0.0, 0.0で画像の左下。
		 *						1.0, 1.0で画像の右上。
		 *						UnityのuGUIに準拠。
		 */
		void Draw(
			wchar_t const* text, 
			const CVector2& position, 
			const CVector4& color = CVector4::White,
			float rotation = 0.0f, 
			float scale = 1.0f,
			CVector2 pivot = {0.5f, 0.5f}
		);
		/*!
		 *@brief	影のパラメータを設定。
		 *@param[in]	isDrawShadow		影を描く？
		 *@param[in]	shadowOffset		影を描くときのピクセルのオフセット量。
		 *@param[in]	shadowColor			影の色。
		 */
		void SetShadowParam(bool isDrawShadow, float shadowOffset, const CVector4& shadowColor)
		{
			m_isDrawShadow = isDrawShadow;
			m_shadowOffset = shadowOffset;
			m_shadowColor = shadowColor;
		}
	private:
		DirectX::SpriteBatch*	m_spriteBatch = nullptr;	//!<スプライトバッチ。
		DirectX::SpriteFont*	m_spriteFont = nullptr;		//!<スプライトフォント。
		bool m_isDrawShadow = false;						//!<影を書く？
		float m_shadowOffset = 0.0f;						//!<影を書くときのピクセルのオフセット量。
		CVector4 m_shadowColor = CVector4::Black;			//!<影の色。
		CMatrix m_scaleMat;
	};
}