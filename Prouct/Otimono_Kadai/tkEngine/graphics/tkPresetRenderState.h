/*!
 * @brief	良く使うレンダリングステートをまとめたもの。
 */

#pragma once

namespace tkEngine{
	/*!
	* @brief	αブレンドステート。
	*/
	struct AlphaBlendState {
		static ID3D11BlendState*	disable;	//!<アルファブレンディングが無効。
		static ID3D11BlendState*	trans;		//!<半透明合成。
		static ID3D11BlendState*	add;		//!<加算合成。
		/*!
		* @brief	初期化。
		*@param[in]	ge		グラフィックエンジン
		*/
		static void Init(CGraphicsEngine& ge);
	};
	/*!
	* @brief	深度ステンシルステート。。
	*/
	struct DepthStencilState
	{
		static ID3D11DepthStencilState* disable;			//!<すべて無効。
		static ID3D11DepthStencilState* SceneRender;		//!<3Dモデルを描画する時の基本的な深度ステンシルステート。
															//!<深度テスト、深度書き込みともに有効なステートです。
		static ID3D11DepthStencilState* spriteRender;		//!<2D描画する時の基本的な深度ステンシルステート。
															//!<深度テスト、深度書き込みともに無効なステートです。
		static ID3D11DepthStencilState* defferedRender;		//!<ディファードレンダリングを行うときの深度ステンシルステート。
															//!<深度テスト無効、深度書き込み有効なステートです。
		/*!
		* @brief	初期化。
		*@param[in]	ge		グラフィックエンジン
		*/
		static void Init(CGraphicsEngine& ge);
	};
	/*!
	* @brief	ラスタライザステート。
	*/
	struct RasterizerState
	{
		static ID3D11RasterizerState*	sceneRender;		//!<3Dモデルを描画する時の基本的なラスタライザステート。
		static ID3D11RasterizerState*	spriteRender;		//!<2D描画する時の基本的なラスタライザステート。
		/*!
		 * @brief	初期化。
		 *@param[in]	ge		グラフィックエンジン
		 */
		static void Init(CGraphicsEngine& ge);
	};
}