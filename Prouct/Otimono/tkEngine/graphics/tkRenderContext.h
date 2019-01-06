/*!
* @brief	レンダリングコンテキスト
*/

#pragma once


#include "tkEngine/graphics/tkShader.h"
#include "tkEngine/graphics/GPUBuffer/tkVertexBuffer.h"
#include "tkEngine/graphics/GPUBuffer/tkIndexBuffer.h"
#include "tkEngine/graphics/GPUBuffer/tkConstantBuffer.h"
#include "tkEngine/graphics/GPUBuffer/tkStructuredBuffer.h"
#include "tkEngine/graphics/GPUView/tkShaderResourceView.h"
#include "tkEngine/graphics/GPUView/tkUnorderedAccessView.h"
#include "tkEngine/graphics/tkSamplerState.h"
#include "tkEngine/graphics/tkRenderTarget.h"

namespace tkEngine {
	class CVertexBuffer;
	class CRenderTarget;
	/*!
	* @brief	レンダリングステップ。
	*/
	enum EnRenderStep {
		enRenderStep_LightCulling,				//!<ライトカリング。
		enRenderStep_ZPrepass,					//!<ZPrepass。
		enRenderStep_RenderToShadowMap,			//!<シャドウマップへの書き込みステップ。
		enRenderStep_RenderGBuffer,				//!<G-Bufferの作成ステップ。
		enRenderStep_DefferdShading,			//!<ディファードシェーディング。
		enRenderStep_ForwardPreRender,			//!<フォワードプリレンダリング。(特殊な表現を行いたいオブジェクトはここで描く。)
		enRenderStep_ForwardRender,				//!<フォワードレンダリング。(特殊な表現を行いたいオブジェクトはここで描く。)
		enRenderStep_RenderTransObject,			//!<半透明オブジェクトをレンダリング。
		enRenderStep_RenderEffect,				//!<エフェクトをシーンに描画。
		enRenderStep_ScreenSpaceReflection,		//!<SSR
		enRenderStep_Bloom,						//!<ブルーム。
		enRenderStep_Toonmap,					//!<トーンマップ。
		enRenderStep_AntiAlias,					//!<アンチエイリアス。
		enRenderStep_Dof,						//!<DOF。
		enRenderStep_Render2DToScene,			//!<2Dをシーンに描画。
	};
	class CRenderContext : Noncopyable {
	public:

		CRenderContext() {}
		~CRenderContext() {}
		/*!
		* @brief	初期化。
		*@param[in]	pD3DDeviceContext	D3Dデバイスコンテキスト。開放は呼び出しもとで行ってください。
		*/
		void Init(ID3D11DeviceContext* pD3DDeviceContext, ID3D11DeviceContext* pD3DDeferredDeviceContext);
		/// <summary>
		/// 現在のレンダリングターゲットをスタックにプッシュ。
		/// </summary>
		void PushRenderTargets()
		{
			SRenderTarget renderTarget;
			OMGetRenderTargets(renderTarget.numRenderTargetViews, renderTarget.renderTargets);
			m_renderTargetStack.push(renderTarget);
		}
		/// <summary>
		/// スタックからレンダリングターゲットをポップ。
		/// </summary>
		/// <param name="isApplyRenderState">
		/// ポップしたレンダリングターゲットをレンダリングパイプラインに設定するかどうかのフラグ。
		/// </param>
		void PopRenderTargets(bool isSetRenderStateToRenderPipeline)
		{
			auto rt = m_renderTargetStack.top();
			m_renderTargetStack.pop();
			OMSetRenderTargets(rt.numRenderTargetViews, rt.renderTargets);
			if (rt.renderTargets[0] != nullptr) {
				RSSetViewport(
					0.0f, 0.0f, (float)rt.renderTargets[0]->GetWidth(), (float)rt.renderTargets[0]->GetHeight());
			}
		}
		/// <summary>
		/// 現在のレンダリングステートをスタックにプッシュ。
		/// </summary>
		void PushRenderState()
		{
			m_renderStateStack.push(m_currentRenderState);
		}
		/// <summary>
		/// スタックに退避したレンダリングステートをポップ。
		/// </summary>
		/// <param name="isApplyRenderState">
		/// ポップしたレンダリングステートをレンダリングパイプラインに設定するかどうかのフラグ。
		/// </param>
		void PopRenderState(bool isSetRenderStateToRenderPipeline)
		{
			m_currentRenderState = m_renderStateStack.top();
			m_renderStateStack.pop();
			if (isSetRenderStateToRenderPipeline == true) {
				m_pD3DDeviceContext->OMSetBlendState(m_currentRenderState.blendState, 0, 0xFFFFFFFF);
				m_pD3DDeviceContext->RSSetState(m_currentRenderState.rasterrizerState);
				m_pD3DDeviceContext->OMSetDepthStencilState(m_currentRenderState.depthStencilState, 0);
			}
		}
		/*!
		* @brief	Blendステートを設定する。
		* @details
		*  ID3D11DeviceContext::OMSetBlendStateと同じ。
		*/
		void OMSetBlendState(ID3D11BlendState *pBlendState)
		{
			m_currentRenderState.blendState = pBlendState;
			m_pD3DDeviceContext->OMSetBlendState(pBlendState, 0, 0xFFFFFFFF);
		}
		/*!
		*@brief	現在のBlendステートを取得する。
		*/
		ID3D11BlendState* GetBlendState()
		{
			return m_currentRenderState.blendState;
		}
		/*!
		* @brief	DepthStencilステートを設定する。
		* @details
		*  ID3D11DeviceContext::OMSetDepthStencilStateと同じ。
		*/
		void OMSetDepthStencilState(ID3D11DepthStencilState *pDepthStencilState)
		{
			m_pD3DDeviceContext->OMSetDepthStencilState(pDepthStencilState, 0);
			m_currentRenderState.depthStencilState = pDepthStencilState;
		}
		/*!
		* @brief	DepthStencilステートを取得する。
		*/
		ID3D11DepthStencilState* GetDepthStencilState() const
		{
			return m_currentRenderState.depthStencilState;
		}
		/*!
		* @brief	レンダリングターゲットビューを設定。
		* @details
		*  ID3D11DeviceContext::OMSetRenderTargetsと同じ。
		*@param[in]	NumViews		バインドするレンダリングターゲットの数。
		*@param[in]	renderTarget	バインドするレンダリングターゲットの配列へのポインタ。
		*/
		void OMSetRenderTargets(unsigned int NumViews, CRenderTarget* renderTarget[]);
		void OMSetRenderTargets(unsigned int NumViews, ID3D11RenderTargetView *const *ppRenderTargetViews, ID3D11DepthStencilView *pDepthStencilView)
		{
			m_pD3DDeviceContext->OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
			m_numRenderTargetView = NumViews;
		}
		/*!
		* @brief	現在バインドされているレンダリングターゲットビューを取得。
		*@param[out]	numViews		バインドされているレンダリングターゲットの数。
		*@param[out]	renderTarget	バインドするレンダリングターゲットの配列へのポインタ。
		*/
		void OMGetRenderTargets(unsigned int& numViews, CRenderTarget* renderTargets[MRT_MAX])
		{
			memcpy(renderTargets, m_renderTargetViews, sizeof(CRenderTarget*)*m_numRenderTargetView);
			numViews = m_numRenderTargetView;
		}
		/*!
		* @brief	ビューポートを設定。
		*@param[in]	topLeftX	ビューポートの左上のX座標。
		*@param[in]	topLeftY	ビューポートの左上のY座標。
		*@param[in]	width		ビューポートの幅。
		*@param[in]	height		ビューポートの高さ。
		*/
		void RSSetViewport(float topLeftX, float topLeftY, float width, float height)
		{
			m_viewport.Width = width;
			m_viewport.Height = height;
			m_viewport.TopLeftX = topLeftX;
			m_viewport.TopLeftY = topLeftY;
			m_viewport.MinDepth = 0.0f;
			m_viewport.MaxDepth = 1.0f;
			m_pD3DDeviceContext->RSSetViewports(1, &m_viewport);
		}
		/*!
		* @brief	ラスタライザのステートを設定。
		*/
		void RSSetState(ID3D11RasterizerState *pRasterizerState)
		{
			m_currentRenderState.rasterrizerState = pRasterizerState;
			m_pD3DDeviceContext->RSSetState(pRasterizerState);	
		}
		/*!
		*@brief	現在のラスタライザステートを取得。
		*/
		ID3D11RasterizerState* GetRSState() const
		{
			return m_currentRenderState.rasterrizerState;
		}
		/*!
		* @brief	レンダリングターゲットをクリア。
		*@param[in]	rtNo	レンダリングターゲットの番号。
		*@param[in]	clearColor	クリアカラー。
		*@param[in]	isClearDepthStencil	デプスステンシルバッファもクリアする？
		*/
		void ClearRenderTargetView(unsigned int rtNo, float* clearColor, bool isClearDepthStencil = true)
		{
			if (rtNo < m_numRenderTargetView
				&& m_renderTargetViews != nullptr) {
				m_pD3DDeviceContext->ClearRenderTargetView(m_renderTargetViews[rtNo]->GetRenderTargetView(), clearColor);
				if (m_renderTargetViews[0]->GetDepthStencilView() != nullptr && isClearDepthStencil) {
					m_pD3DDeviceContext->ClearDepthStencilView(m_renderTargetViews[0]->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
				}
			}
		}
		/*!
		* @brief	頂点バッファを設定。
		*/
		void IASetVertexBuffer(CVertexBuffer& vertexBuffer)
		{
			UINT offset = 0;
			UINT stride = vertexBuffer.GetStride();
			m_pD3DDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer.GetBody(), &stride, &offset);
		}
		/*!
		* @brief	インデックスバッファを設定
		*/
		void IASetIndexBuffer(CIndexBuffer& indexBuffer)
		{
			CIndexBuffer::EnIndexType type = indexBuffer.GetIndexType();
			m_pD3DDeviceContext->IASetIndexBuffer(
				indexBuffer.GetBody(),
				type == CIndexBuffer::enIndexType_16 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT,
				0
			);
		}
		/*!
		* @brief	プリミティブのトポロジーを設定。
		*@param[in]	topology	トポロジー。
		*/
		void IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
		{
			m_pD3DDeviceContext->IASetPrimitiveTopology(topology);
		}
		/*!
		* @brief	VSステージに定数バッファを設定。
		*@param[in]	slotNo		スロット番号。
		*@param[in]	cb			定数バッファ。
		*/
		void VSSetConstantBuffer(
			UINT slotNo,
			CConstantBuffer& cb
		)
		{
			m_pD3DDeviceContext->VSSetConstantBuffers(slotNo, 1, &cb.GetBody());
		}
		/*!
		* @brief	VSステージにSRVを設定。
		*@param[in]	slotNo		スロット番号。
		*@param[in]	srv			SRV。
		*/
		void VSSetShaderResource(int slotNo, CShaderResourceView& srv)
		{
			m_pD3DDeviceContext->VSSetShaderResources(slotNo, 1, &srv.GetBody());
		}
		/*!
		* @brief	VSステージにSRVを外す。
		*@param[in]	slotNo		スロット番号。
		*@param[in]	srv			SRV。
		*/
		void VSUnsetShaderResource(int slotNo)
		{
			ID3D11ShaderResourceView* view[] = {
				NULL
			};
			m_pD3DDeviceContext->VSSetShaderResources(slotNo, 1, view);
		}
		/*!
		* @brief	PSステージに定数バッファを設定。
		*@param[in]	slotNo		スロット番号。
		*@param[in]	cb			定数バッファ。
		*/
		void PSSetConstantBuffer(
			UINT slotNo,
			CConstantBuffer& cb
		)
		{
			m_pD3DDeviceContext->PSSetConstantBuffers(slotNo, 1, &cb.GetBody());
		}
		/*!
		* @brief	PSステージにSRVを設定。
		*@param[in]	slotNo		スロット番号。
		*@param[in]	srv			SRV。
		*/
		void PSSetShaderResource(int slotNo, CShaderResourceView& srv)
		{
			m_pD3DDeviceContext->PSSetShaderResources(slotNo, 1, &srv.GetBody());
		}
		/*!
		* @brief	PSステージにSRVを外す。
		*@param[in]	slotNo		スロット番号。
		*@param[in]	srv			SRV。
		*/
		void PSUnsetShaderResource(int slotNo)
		{
			ID3D11ShaderResourceView* view[] = {
				NULL
			};
			m_pD3DDeviceContext->PSSetShaderResources(slotNo, 1, view);
		}
		/*!
		* @brief	PSステージにサンプラステートを設定。
		*@param[in]	slotNo			スロット番号。
		*@param[in]	samplerState	サンプラステート。
		*/
		void PSSetSampler(int slotNo, CSamplerState& samplerState)
		{
			m_pD3DDeviceContext->PSSetSamplers(slotNo, 1, &samplerState.GetBody());
		}
		/*!
		* @brief	頂点シェーダーを設定。
		*@param[in]	shader		頂点シェーダー。
		*/
		void VSSetShader(CShader& shader)
		{
			IASetInputLayout(shader.GetInputLayout());
			m_pD3DDeviceContext->VSSetShader((ID3D11VertexShader*)shader.GetBody(), NULL, 0);
		}
		/*!
		* @brief	ピクセルシェーダーを設定。
		*@param[in]	shader		頂点シェーダー。
		*/
		void PSSetShader(CShader& shader)
		{
			m_pD3DDeviceContext->PSSetShader((ID3D11PixelShader*)shader.GetBody(), NULL, 0);
		}
		/*!
		* @brief	コンピュートシェーダーを設定。
		*@param[in]	shader		コンピュートシェーダー。
		*/
		void CSSetShader(CShader& shader)
		{
			m_pD3DDeviceContext->CSSetShader((ID3D11ComputeShader*)shader.GetBody(), NULL, 0);
		}
		/*!
		* @brief	CSステージに定数バッファを設定。
		*@param[in]	slotNo		スロット番号。
		*@param[in]	cb			定数バッファ。
		*/
		void CSSetConstantBuffer(
			UINT slotNo,
			CConstantBuffer& cb
		)
		{
			m_pD3DDeviceContext->CSSetConstantBuffers(slotNo, 1, &cb.GetBody());
		}
		/*!
		* @brief	コンピュートシェーダーにSRVを設定。。
		*@param[in]	slotNo		スロット番号
		*@param[in]	srv			シェーダーリソースビュー。
		*/
		void CSSetShaderResource(int slotNo, CShaderResourceView& srv)
		{
			m_pD3DDeviceContext->CSSetShaderResources(slotNo, 1, &srv.GetBody());
		}
		void CSUnsetShaderResource(int slotNo)
		{
			ID3D11ShaderResourceView* view[] = {
				NULL
			};
			m_pD3DDeviceContext->CSSetShaderResources(slotNo, 1, view);
		}
		/*!
		* @brief	コンピュートシェーダーにUAVを設定。
		*@param[in]	slotNo		スロット番号
		*@param[in]	uav			UAV。
		*/
		void CSSetUnorderedAccessView(int slotNo, CUnorderedAccessView& uav)
		{
			m_pD3DDeviceContext->CSSetUnorderedAccessViews(slotNo, 1, &uav.GetBody(), NULL);
		}
		/*!
		* @brief	コンピュートシェーダーにUAVを設定。
		*@param[in]	slotNo		スロット番号
		*@param[in]	uav			UAV。
		*/
		void CSUnsetUnorderedAccessView(int slotNo)
		{
			ID3D11UnorderedAccessView* view[] = {
				NULL
			};
			m_pD3DDeviceContext->CSSetUnorderedAccessViews(slotNo, 1, view, NULL);
		}
		/*!
		* @brief	描画。
		* @param[in]	vertexCount			頂点数。
		* @param[in]	startVertexLocation	描画を開始する頂点の位置。大抵0で大丈夫。
		*/
		void Draw(
			unsigned int vertexCount,
			unsigned int startVertexLocation
		)
		{
			m_pD3DDeviceContext->Draw(vertexCount, startVertexLocation);
		}
		/*!
		* @brief	インデックス付き描画。
		* @param[in]	indexCount			インデックスの数。
		* @param[in]	StartIndexLocation	描画を開始するインデックスの位置。大抵0で大丈夫。
		* @param[in]	BaseVertexLocation	ベース頂点インデックス。大抵0で大丈夫。
		*/
		void DrawIndexed(
			_In_  UINT IndexCount,
			_In_  UINT StartIndexLocation,
			_In_  INT BaseVertexLocation)
		{
			m_pD3DDeviceContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);

		}
		/*!
		* @brief	ディスパッチ。
		* @details
		*  コンピュートシェーダーを実行。
		* @param[in]	threadGroupCountX	x 方向にディスパッチしたグループの数。
		* @param[in]	threadGroupCountY	y 方向にディスパッチしたグループの数。
		* @param[in]	thredGroupCountZ	ｚ 方向にディスパッチしたグループの数。
		*/
		void Dispatch(UINT threadGroupCountX, UINT threadGroupCountY, UINT thredGroupCountZ)
		{
			m_pD3DDeviceContext->Dispatch(threadGroupCountX, threadGroupCountY, thredGroupCountZ);
		}
		private:
		/*!
		* @brief	入力レイアウトを設定。
		*/
		void IASetInputLayout(ID3D11InputLayout* inputLayout)
		{
			m_pD3DDeviceContext->IASetInputLayout(inputLayout);
		}
		public:
		/*!
		* @brief	リソースをコピー。
		*@param[out]	destRes		コピー先。
		*@param[in]		srcRes		コピー元。
		*/
		template<class TResource>
		void CopyResource(TResource& destRes, TResource& srcRes)
		{
			if (destRes.GetBody() != nullptr
				&& srcRes.GetBody() != nullptr
				) {
				m_pD3DDeviceContext->CopyResource(destRes.GetBody(), srcRes.GetBody());
			}
		}

		void CopyResource(ID3D11Resource* destRes, ID3D11Resource* srcRes)
		{
			if (destRes != nullptr
				&& srcRes != nullptr
				) {
				m_pD3DDeviceContext->CopyResource(destRes, srcRes);
			}
		}
		/*!
		* @brief	マップ。
		*@param[out]	destRes		コピー先。
		*@param[in]		srcRes		コピー元。
		*/
		template<class TBuffer>
		void Map(TBuffer& buffer, UINT subresource, D3D11_MAP mapType, UINT mapFlags, D3D11_MAPPED_SUBRESOURCE& mappedResource)
		{
			if (buffer.GetBody() != nullptr) {
				m_pD3DImmidiateDeviceContext->Map(buffer.GetBody(), subresource, mapType, mapFlags, &mappedResource);
			}
		}
		template<class TBuffer>
		void Unmap(TBuffer& buffer, UINT subresource)
		{
			if (buffer.GetBody() != nullptr) {
				m_pD3DImmidiateDeviceContext->Unmap(buffer.GetBody(), subresource);
			}
		}
		/*!
		* @brief	サブリソースを更新。
		*@param[out]	destRes		コピー先。
		*@param[in]		srcRes		コピー元。
		*/
		template<class TBuffer, class SrcBuffer>
		void UpdateSubresource(TBuffer& gpuBuffer, const SrcBuffer* buffer)
		{
			if (gpuBuffer.GetBody() != nullptr) {
				m_pD3DDeviceContext->UpdateSubresource(gpuBuffer.GetBody(), 0, NULL, buffer, 0, 0);
			}
		}
		/*!
		* @brief	現在のレンダリングステップを取得。
		*@return レンダリングステップ。
		*/
		EnRenderStep GetRenderStep() const
		{
			return m_renderStep;
		}
		/*!
		* @brief	現在のレンダリングステップを設定。
		*@details
		* エンジン内部でのみ使用されます。ユーザーは使用しないように。
		*/
		void SetRenderStep(EnRenderStep step)
		{
			m_renderStep = step;
		}
		/*!
		* @brief	ID3D11DeviceContext::ResolveSubresourceと同じ。
		*/
		void ResolveSubresource(
			ID3D11Resource* dstResource,
			UINT DstSubresource,
			ID3D11Resource *pSrcResource,
			UINT SrcSubresource,
			DXGI_FORMAT Format)
		{
			m_pD3DDeviceContext->ResolveSubresource(
				dstResource,
				DstSubresource,
				pSrcResource,
				SrcSubresource,
				Format
			);
		}
		/// <summary>
		/// 描画コマンドの生成で使用しているデバイスコンテキストを取得する。
		/// </summary>
		/// <returns></returns>
		ID3D11DeviceContext* GetD3DDeviceContext() const
		{
			return m_pD3DDeviceContext;
		}
	private:
		/// <summary>
		/// 各種レンダリングステート。
		/// </summary>
		struct SRenderState {
			ID3D11DepthStencilState*	depthStencilState = nullptr;	//!<現在のデプスステンシルステート。
			ID3D11RasterizerState*		rasterrizerState = nullptr;		//!<現在のラスタライザステート。
			ID3D11BlendState*			blendState = nullptr;			//!<現在のブレンドステート。

		};
		/// <summary>
		/// レンダリングターゲット。
		/// </summary>
		struct SRenderTarget {
			CRenderTarget* renderTargets[MRT_MAX] = { nullptr };
			unsigned int numRenderTargetViews = 0;
		};
		SRenderState m_currentRenderState;	//現在のレンダリングステート。
		std::stack< SRenderState> m_renderStateStack;	//レンダリングステートのスタック。
		std::stack<SRenderTarget> m_renderTargetStack;	//レンダリングターゲットのスタック。
		ID3D11DeviceContext*			m_pD3DImmidiateDeviceContext = nullptr;	//!<D3D即時デバイスコンテキスト。MapとUnmapでは即時デバイスコンテキストが必要なので、持たす。
		ID3D11DeviceContext*			m_pD3DDeviceContext = nullptr;			//!<描画コマンドを積んでいくコンテキスト。
		D3D11_VIEWPORT 					m_viewport;								//!<ビューポート。
		CRenderTarget*					m_renderTargetViews[MRT_MAX] = { nullptr };
		unsigned int 					m_numRenderTargetView = 0;		//!<レンダリングターゲットビューの数。
		EnRenderStep					m_renderStep = enRenderStep_LightCulling;	//!<レンダリングステップ。
	};
}