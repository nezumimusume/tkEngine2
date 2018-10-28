/*!
 * @brief	レンダリングターゲット。
 */

#pragma once

namespace tkEngine{

	class CRenderContext;
	/*!
	 * @brief	レンダリングターゲット。
	 */
	class CRenderTarget {
	public:
		CRenderTarget();
		~CRenderTarget();
		/*!
		 *@brief	レンダリングターゲットの作成。
		 *@param[in]	w					レンダリングターゲットの幅。
		 *@param[in]	h					レンダリングターゲットの高さ。
		 *@param[in]	mipLevel			ミップマップレベル。0を指定した場合はミップマップがサポートされているGPUでは1*1ピクセルまでのミップマップが作成される。
		 *@param[in]	arraySize			テクスチャ配列のサイズ。
		 *@param[in]	colorFormat			カラーバッファのフォーマット。
		 *@param[in]	depthStencilFormat	深度ステンシルバッファのフォーマット。
		 *@param[in]	multiSampleDesc		マルチサンプリングの定義。
		 *@param[in]	renderTarget		レンダリングターゲットとなるテクスチャ。nullptrが指定されていたら、内部でテクスチャを生成する。
		 *@param[in]	depthStencil		デプスステンシルとなるテクスチャ。nullptrが指定されていたら、内部でテクスチャを生成する。
		 *@return	trueが返ってきたら作成成功。
		 */
		bool Create(
			int w,
			int h,
			int mipLevel,
			int arraySize,
			DXGI_FORMAT colorFormat,
			DXGI_FORMAT depthStencilFormat,
			DXGI_SAMPLE_DESC multiSampleDesc,
			ID3D11Texture2D* renderTarget = nullptr,
			ID3D11Texture2D* depthStencil = nullptr,
			bool isResolveTexture = false
		);
		/*!
		 *@brief	レンダリングターゲットの破棄。
		 */
		void Release();
		/*!
		*@brief	レンダリングターゲットを取得。
		*/
		ID3D11Texture2D* GetRenderTarget()
		{
			return m_renderTarget;
		}
		/*!
		*@brief	レンダリングターゲットビューの取得。
		*/
		ID3D11RenderTargetView* GetRenderTargetView()
		{
			return m_renderTargetView;
		}
		/*!
		*@brief	レンダリングターゲットのSRVを取得。
		*/
		CShaderResourceView& GetRenderTargetSRV()
		{
			return m_renderTargetSRV;
		}
		/*!
		*@brief	レンダリングターゲットのUAVを取得。
		*/
		CUnorderedAccessView& GetRenderTargetUAV()
		{
			return m_renderTargetUAV;
		}
		/*!
		*@brief	デプスステンシルの取得。
		*/
		ID3D11Texture2D* GetDepthStencil()
		{
			return m_depthStencil;
		}
		/*!
		*@brief	デプスステンシルビューの取得。
		*/
		ID3D11DepthStencilView* GetDepthStencilView()
		{
			return m_depthStencilView;
		}
		/*!
		*@brief	デプスステンシルビューを設定。
		*/
		void SetDepthStencilView(ID3D11DepthStencilView* view)
		{
			if (m_depthStencilView != nullptr) {
				//現在のデプスステンシルビューをリリース。
				m_depthStencilView->Release();
			}
			m_depthStencilView = view;
			m_depthStencilView->AddRef();
		}
		/*!
		 *@brief	レンダリングターゲットの幅を取得。
		 */
		int GetWidth() const
		{
			return m_width;
		}
		int GetHeight() const
		{
			return m_height;
		}
		/*!
		 *@brief	レンダリングターゲットのテクスチャのフォーマットを取得。
		 */
		DXGI_FORMAT GetRenderTargetTextureFormat() const
		{
			return m_textureFormat;
		}
		/*!
		 *@brief	デプスステンシルのフォーマットを取得。
		 */
		DXGI_FORMAT GetDepthStencilFormat() const
		{
			return m_depthStencliFormat;
		}
		/*!
		*@brief	MSAAテクスチャのリゾルブ。
		*@details 
		* MSAAが有効になっているレンダリングターゲットのテクスチャをSRVとして使用したい場合に呼び出してください。。
		*/
		inline void ResovleMSAATexture(CRenderContext& rc);
		
	private:
		ID3D11Texture2D*			m_renderTarget = nullptr;			//!<レンダリングターゲットとなるテクスチャ。
		ID3D11RenderTargetView*		m_renderTargetView = nullptr;		//!<レンダーターゲットビュー。
		ID3D11Texture2D*			m_depthStencil = nullptr;			//!<デプスステンシルとなるテクスチャ。
		ID3D11DepthStencilView*		m_depthStencilView = nullptr;		//!<デプスステンシルビュー。
		CShaderResourceView			m_renderTargetSRV;					//!<レンダリングターゲットのSRV。
		CUnorderedAccessView		m_renderTargetUAV;					//!<レンダリングターゲットのUAV。
		ID3D11Texture2D*			m_resolveTextureMSAA = nullptr;		//!<MSAAが有効の時にリゾルブされたテクスチャ。
		bool						m_isMSAA = false;					//!<MSAAが有効？
		DXGI_FORMAT					m_textureFormat = DXGI_FORMAT_UNKNOWN;	//!<テクスチャのフォーマット。
		DXGI_FORMAT					m_depthStencliFormat = DXGI_FORMAT_UNKNOWN;	//!<デプスステンシルバッファのフォーマット。
		bool m_isResolveTexture = false;
		int m_width = 0;
		int m_height = 0;
	};
}