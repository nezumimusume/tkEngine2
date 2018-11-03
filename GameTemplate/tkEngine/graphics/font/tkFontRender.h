/*!
 * @brief	フォント
 */

#pragma once

#include "tkEngine/graphics/font/tkFont.h"

namespace tkEngine{
namespace prefab {
	/*!
	 * @brief	フォントレンダー。
	 */
	class CFontRender final : public IGameObject{
	public:
		/*!
		* @brief	フォントを設定。
		* @details	デフォルトフォントから変更したい場合は
		*  DirectX::SpriteFontを使用して、フォントをロードして差し替えを行ってください。
		*/
		void SetFont(DirectX::SpriteFont* font)
		{
			m_font.SetFont(font);
		}
		
		/*!
		 *@brief	影のパラメータを設定。
		 *@param[in]	isDrawShadow		影を描く？
		 *@param[in]	shadowOffset		影を描くときのピクセルのオフセット量。
		 *@param[in]	shadowColor			影の色。
		 */
		void SetShadowParam(bool isDrawShadow, float shadowOffset, const CVector4& shadowColor)
		{
			m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		}
		/*!
		 * @brief	テキストを設定。
		 *@details
		 * 毎フレーム呼ぶと重いぞ！！！\n
		 * 重いのが問題になったら相談してね\n
		 * Unsafe版があります。
		 *@param[in]	text		テキスト
		 */
		void SetText(const wchar_t* text)
		{
			if (wcscmp(m_text.c_str(), text) != 0) {
				//文字列が変わった。
				m_text = text;
			}
			
		}
		/*!
		 * @brief	テキストを設定。Unsafe版。
		 *@details
		 *　危険だよ！！！\n
		 *  素人は使っちゃダメ！\n
		 *  使いたい人はちゃんとこの先を読んでね！！！\n
		 * 
		 *
		 *
		 *
		 *
		 *  SetTextは呼び出すたびに文字列のコピーが発生するため、\n
		 *  毎回メモリアロケートが走ります。\n
		 *  しかし、内部でメモリ確保を行っているため、\n
		 *  引数で指定されたテキストが無効になっても問題なく動作します。\n
		 *  しかし、SetTextUnsafe版は引数で渡されたアドレスをそのまま使用するため、\n
		 *  フォントの描画が完了するまで、そのアドレスは有効である必要があります。\n
		 *  ようは、ローカル変数に格納しているテキストとか渡したら死ぬよ( ´艸｀)！ってことです。\n
		 *  この説明を読んで、意味が分からない人は、SetTextUnsafeは使わないこと！！！！
		 * 
		 @param[in]	text		テキスト
		 */
		void SetTextUnsafe(const wchar_t* text)
		{
			m_textUnsafe = text;
		}
		/*!
		* @brief	フォントを表示する座標を設定。
		*@details
		* フォントの座標系は2次元となっており、画面の中央を0, 0とします。\n
		* 右方向がX+、上方向がY+の座標系です。
		*@param[in]	pos		座標。
		*/
		void SetPosition(const CVector2& pos)
		{
			m_position = pos;
		}
		/*!
		* @brief	フォントのカラーを設定。
		*@details
		* フォントのカラーは光の三原色で指定します。\n
		* 引数のcolor.xが赤、color.yが緑、color.zが青となります。\n
		* 各成分に設定できる値は0.0～1.0です。\n
		* 例えば、真っ赤を指定したい場合は、\n
		* color.x = 1.0f、color.y = 0.0f、color.z = 0.0fとなります。
		* 
		*@param[in]	color	色。
		*/
		void SetColor(const CVector3& color)
		{
			m_color = color;
		}
		/*!
		 *@brief	回転を設定。
		 *@param[in]	rotation	回転角度。単位はラジアン。
		 */
		void SetRotation(float rotation)
		{
			m_rotation = rotation;
		}
		/*!
		 *@brief	拡大率を設定。
		 *@param[in]	scale	拡大率。
		 */
		void SetScale(float scale)
		{
			m_scale = scale;
		}
		/*!
		 *@brief	ピボットを設定。
		 *@details
		 *　フォントを表示する際の基点を指定します。\n
		 *　0.5, 0.5で画像の中心が基点。
		 *	0.0, 0.0で画像の左下。
		 *	1.0, 1.0で画像の右上。
		 *  UnityのuGUIに準拠。
		 *@param[in]	pivot	基点。
		 */
		void SetPivot(const CVector2& pivot)
		{
			m_pivot = pivot;
		}
	private:
		void PostRender(CRenderContext& rc);
	private:
		CFont m_font;
		std::wstring m_text;					//!<テキスト。
		const wchar_t* m_textUnsafe = nullptr;	//!<アンセーフ版のテキスト。
		CVector2 m_position = CVector2::Zero;	//!<座標。x = 0.0f, y = 0.0fで画面の中心。
		CVector4 m_color = CVector4::White;		//!<カラー。
		float m_rotation = 0.0f;				//!<回転。
		float m_scale = 1.0f;					//!<拡大率。
		CVector2 m_pivot;						//!<ピボット。
	};
}
}