/*!
 *@brief	六角形ブラー。
 */

#pragma once

namespace tkEngine{
	/// <summary>
	/// 六角形ブラー。
	/// </summary>
	/// <remarks>
	/// 参考URL
	/// https://colinbarrebrisebois.com/2017/04/18/hexagonal-bokeh-blur-revisited/
	/// http://hikita12312.hatenablog.com/entry/2017/09/26/225828
	/// </remarks>
	class CHexaBlur final : Noncopyable {
	public:
		/// <summary>
		/// 描画パス。
		/// </summary>
		enum EnRenderPass {
			enRenderPass_VerticalDiagonalBlur,			//垂直、斜めブラー
			enRenderPass_CombineVerticalDiagonalBlur,	//垂直ブラーと斜めブラーを合成する。
			enRenderPass_RhomboidBlur,					//六角形ブラーを作成する。
			enRenderPass_Num,
		};
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="srcTexture">ソースとなるテクスチャ</param>
		/// <param name="isScaleup">拡大ブラーをかける？</param>
		void Init(CShaderResourceView& srcTexture, bool isScaleup = false);
		/// <summary>
		/// ブラーを実行。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Execute(CRenderContext& rc, std::function<void(CRenderContext& rc, EnRenderPass)> onPreDraw);
		/// <summary>
		/// ソーステクスチャを変更。
		/// </summary>
		/// <param name="srcTexture">ソーステクスチャ</param>
		void ChangeSrcTexture(CShaderResourceView& srcTexture)
		{
			m_srcTexture = &srcTexture;
		}
		/// <summary>
		/// ブラーの半径を設定。
		/// </summary>
		/// <param name="radius">ブラーの半径(単位：テクセル)</param>
		void SetRadius(float radius)
		{
			m_radius = radius;
		}
		/// <summary>
		/// ブラーの半径を取得。
		/// </summary>
		float GetRadius() const
		{
			return m_radius;
		}
		/// <summary>
		/// 結果を取得。
		/// </summary>
		/// <returns></returns>
		CShaderResourceView& GetResultSRV()
		{
			return m_renderTarget[enRenderTarget_RhomboidBlur].GetRenderTargetSRV();
		}
	private:
		
		enum EnRenderTarget {
			enRenderTarget_VerticalBlur,			//垂直ブラー。
			enRenderTarget_DiagonalBlur,			//斜めブラー。
			enRenderTarget_VerticalDiagonalBlur,	//垂直、斜めブラー。
			enRenderTarget_RhomboidBlur,			//六角形ブラー。
			enRenderTarget_Num,
		};
		/// <summary>
		/// この構造体の中身を変更したら、hexaBlur.fxのcbParamも変更するように。
		/// </summary>
		struct SCBParam {
			CVector2 texSize;		//ブラーをかけるテクスチャのサイズ。
			float radius;			//ブラーの半径。
		};
		CShaderResourceView* m_srcTexture = nullptr;		//ブラー元のテクスチャ。
		CRenderTarget m_renderTarget[enRenderTarget_Num];	//レンダリングターゲット。
		CShader m_vs;										//頂点シェーダ。
		CShader m_psVerticalDiagonalBlur;					//垂直、斜めブラーのピクセルシェーダー。
		CShader m_psCombineVerticalDiagonalBlur;			//垂直、斜めブラーの合成用のピクセルシェーダー。
		CShader m_psRhomboidBlur;							//六角形ブラーを作成する。
		CConstantBuffer m_cb;								//定数バッファ。
		float m_radius = 4.0f;								//ブラー半径。
	};
}
