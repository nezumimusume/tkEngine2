/*!
 *@brief	描画エンジン
 */

#pragma once


#include "tkEngine/graphics/preRender/tkPreRender.h"
#include "tkEngine/graphics/tkCamera.h"
#include "tkEngine/tkEngineStruct.h"
#include "tkengine/light/tkLightManager.h"
#include "tkEngine/graphics/postEffect/tkPostEffect.h"
#include "tkEngine/resource/tkShaderResources.h"
#include "tkEngine/graphics/effect/tkEffectEngine.h"
#include "tkEngine/resource/tkDirectXModelResource.h"

namespace tkEngine{
	
	 
	/*!
	*@brief	描画エンジン。
	*@details
	* 2017/08/03 release notes
	* 	tkEngineの描画エンジンはforward+で実装されている。
	* 	シーンの描画手順は下記となる。
	*  	.1 プリレンダリング
	*     		1-1 ZPrepass
	*	   		1-2 LightCulling
	*  	.2 モデル描画
	*  	.3 ポストエフェクト
	* forward+の採用により、大量の動的光源を高速に扱うことができる。
	* 動的光源は最大で1024個設置することができる。エンジンをカスタマイズすることで拡張も可能。
	* 独自のモデルシェーダーを記述する場合は下記の注意が必要です。
	*    tレジスタの100番以降とbレジスタの1番はエンジンが使用しているため、使ってはならない。
	*/
	class CGraphicsEngine : Noncopyable {
	public:
		CGraphicsEngine();
		~CGraphicsEngine();
		/*!
		* @brief	初期化。
		*/
		bool Init(HWND hwnd, const SInitParam& initParam);
		/*!
		* @brief	開放。
		*/
		void Release();
		/*!
		 *@brief	CGBufferRenderの取得。
		 */
		CGBufferRender& GetGBufferRender()
		{
			return m_preRender.GetGBufferRender();
		}
		/*!
		* @brief	ZPrepassの取得。
		*/
		CZPrepass& GetZPrepass()
		{
			return m_preRender.GetZPrepass();
		}
		/*!
		 *@brief	エフェクトエンジンの取得。
		 */
		CEffectEngine& GetEffectEngine()
		{
			return m_effectEngine;
		}
		/*!
		* @brief	シャドウマップを取得。
		*/
		CShadowMap& GetShadowMap()
		{
			return m_preRender.GetShadowMap();
		}
		/*!
		* @brief	トーンマップを取得。
		*/
		CTonemap& GetTonemap()
		{
			return m_postEffect.GetTonemap();
		}
		/*!
		*@brief	メインカメラを取得。
		*/
		CCamera& GetMainCamera()
		{
			return m_mainCamera;
		}
		/*!
		*@brief	2Dカメラを取得。
		*/
		CCamera& Get2DCamera()
		{
			return m_2dCamera;
		}
		/*!
		*@brief	Direct3DDeviceの取得。
		*/
		ID3D11Device* GetD3DDevice() const
		{
			return m_pd3dDevice;
		}
		/*!
		*@brief	ID3D11DeviceContextの取得。
		*/
		ID3D11DeviceContext* GetD3DDeviceContext() const
		{
			return m_pImmediateContext;
		}
		/*!
		*@brief		フレームバッファの幅を取得。
		*/
		int GetFrameBufferWidth() const
		{
			return m_frameBufferWidth;
		}
		/*!
		*@brief		フレームバッファの高さを取得。
		*/
		int GetFrameBufferHeight() const
		{
			return m_frameBufferHeight;
		}
		/*!
		 *@brief	2D空間のスクリーンの幅。
		 */
		int Get2DSpaceScreenWidth() const
		{
			return m_2dSpaceScreenWidth;
		}
		/*!
		*@brief	2D空間のスクリーンの高さ。
		*/
		int Get2DSpaceScreenHeight() const
		{
			return m_2dSpaceScreenHeight;
		}

		/*!
		* @brief	メインレンダリングターゲットの取得。
		*/
		CRenderTarget& GetMainRenderTarget()
		{
			return m_mainRenderTarget;
		}
		/*!
		* @brief	描画開始。
		*/
		void BeginRender();
		/*!
		* @brief	描画終了。
		*/
		void EndRender();
		/*!
		* @brief	プリレンダリング取得。。
		*/
		CPreRender& GetPreRender()
		{
			return m_preRender;
		}
		/*!
		* @brief	ポストエフェクトを取得。
		*/
		CPostEffect& GetPostEffect()
		{
			return m_postEffect;
		}
		CRenderContext& GetRenderContext()
		{
			return m_renderContext;
		}
		/*!
		*@brief	ライトの管理者を取得。
		*/
		CLightManager& GetLightManager()
		{
			return m_lightManager;
		}
		/*!
		*@brief	メインレンダリングターゲットのMSAAの設定を取得。
		*/
		const DXGI_SAMPLE_DESC& GetMainRenderTargetMSAADesc() const
		{
			return m_mainRenderTargetMSAADesc;
		}
		/*!
		*@brief	GPUイベント開始。
		*@param[in]		eventName		イベントの名前。
		*/
		void BeginGPUEvent(const wchar_t* eventName)
		{
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
			if (m_userAnnoation) {
				m_userAnnoation->BeginEvent(eventName);
			}
#endif
		}
		/*!
		*@brief	GPUイベント終了。
		*/
		void EndGPUEvent()
		{
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
			if (m_userAnnoation) {
				m_userAnnoation->EndEvent();
			}
#endif
		}
		/*!
		*@brief	シェーダーリソースを取得。
		*/
		CShaderResources& GetShaderResources()
		{
			return m_shaderResources;
		}
		/*!
		*@brief	SpriteBatchの取得。
		*@details
		* ゲーム層では使用しないように。
		*/
		DirectX::SpriteBatch* GetSpriteBatch() const
		{
			return m_spriteBatch.get();
		}
		/*!
		*@brief	SpriteFontの取得。
		*@details
		* ゲーム層では使用しないように。
		*/
		DirectX::SpriteFont* GetSpriteFont() const
		{
			return m_spriteFont.get();
		}
		/*!
		*@brief	ポストエフェクトの処理が完了したときに呼ばれる処理。
		*@details
		* ゲーム層では使用しないように。
		*/
		void EndPostEffect(CRenderContext& rc);
		/*!
		*@brief	DirectXモデルリソースの取得。
		*/
		CDirectXModelResource& GetDirectXModelResource()
		{
			return m_directXModelResource;
		}
	private:
		/*!
		 *@brief	D3Dデバイスとスワップチェインの初期化。
		 */
		bool InitD3DDeviceAndSwapChain(HWND hwnd, const SInitParam& initParam);
		/*!
		 *@brief	バックバッファの初期化。
		 */
		bool InitBackBuffer();
		/*!
		 *@brief	メインレンダリングターゲットの初期化。
		 */
		bool InitMainRenderTarget();
	private:
		ID3D11Device*			m_pd3dDevice = nullptr;						//!<D3D11デバイス。
		CRenderContext			m_renderContext;							//!<レンダリングコンテキスト。
		ID3D11DeviceContext*	m_pImmediateContext = nullptr;				//!<D3D11即時デバイスコンテキスト。
		IDXGISwapChain*			m_pSwapChain = nullptr;						//!<SwapChain。
		CRenderTarget			m_mainRenderTarget;							//!<メインレンダリングターゲット。
		CPreRender				m_preRender;								//!<プリレンダリング。
		CPostEffect				m_postEffect;								//!<ポストエフェクト。
		CCamera					m_mainCamera;								//!<メインカメラ。
		CCamera					m_2dCamera;									//!<2Dカメラ。
		CEffectEngine			m_effectEngine;								//!<エフェクトエンジン。
		int						m_frameBufferWidth = 0;						//!<フレームバッファの幅。これが内部解像度。
		int						m_frameBufferHeight = 0;					//!<フレームバッファの高さ。これが内部解像度。
		DXGI_SAMPLE_DESC		m_mainRenderTargetMSAADesc;					//!<メインレンダリングターゲットのMSAAの設定。
		D3D_DRIVER_TYPE			m_driverType = D3D_DRIVER_TYPE_NULL;		//!<ドライバの種類のオプション。
		D3D_FEATURE_LEVEL		m_featureLevel = D3D_FEATURE_LEVEL_11_0;	//!<Direct3D デバイスのターゲットとなる機能セット。
		CLightManager			m_lightManager;								//!<ライトの管理者。
		CShader					m_copyVS;									//!<コピー用の頂点シェーダー。
		CShader					m_copyPS;									//!<コピー用のピクセルシェーダー。
		ID3D11RenderTargetView*	m_backBufferRT = nullptr;					//!<バックバッファのレンダリングターゲットビュー。
		ID3DUserDefinedAnnotation* m_userAnnoation = nullptr;
		CShaderResources		m_shaderResources;							//!<シェーダーリソース。
		std::unique_ptr<DirectX::SpriteBatch>	m_spriteBatch;				//!<スプライトバッチ。
		std::unique_ptr<DirectX::SpriteFont>	m_spriteFont;				//!<スプライトフォント。
		int						m_2dSpaceScreenWidth = 1280;				//!<2D空間のスクリーンの幅。2Dはこの座標系を前提に表示されていれば、解像度が変わっても大丈夫。
		int						m_2dSpaceScreenHeight = 720;				//!<2D空間のスクリーンの高さ。2Dはこの座標系を前提に表示されていれば、解像度が変わっても大丈夫。
		CDirectXModelResource	m_directXModelResource;						//!<DirectXモデルリソースの管理者。
	};
}