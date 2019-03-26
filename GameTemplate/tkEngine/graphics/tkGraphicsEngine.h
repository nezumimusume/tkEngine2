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
	/// <summary>
	/// 描画エンジン
	/// </summary>
	/// <remarks>
	/// 2019/03/26 release notes
	/// tkEngineの描画エンジンはTBDR(Tile Based Deferred Rendering)で実装されている。
	/// TBDRの採用により、大量の動的光源を高速に扱うことができる。
	/// 動的光源は最大で1024個設置することができる。エンジンをカスタマイズすることで拡張も可能。
	/// シーンの描画手順は下記となる。
	/// .1　プリレンダリング
	///			1-1  全方位シャドウマップの生成。
	///			1-2  指向性ライトによるシャドウマップの生成。
	///			1-3  G-Bufferの作成。
	///			1-4  ライトカリング。
	/// .2　ディファードシェーディング
	///			1-4で作成されたシャドウマップ、G-Buffer、ライトカリング情報を元に
	///			PBRベースのシェーディングを行う。
	/// .3  フォワードレンダリング
	///			半透明や特殊なシェーディングを行うマテリアルの描画を行う。
	/// .4  ポストエフェクト
	///			1-1  トーンマップ。
	///			1-2  Effekseerを利用したエフェクト描画。
	///			1-3  Temporal Screen Space Reflection。
	///			1-4  川瀬式ブルーム。
	///			1-5  DOF
	///			1-5  FXAA。
	///			1-6  ディザリング。
	/// .5  ポストレンダリング。HUDなどポストエフェクトをかけたくない描画を行う。
	/// </remarks>
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
		 *@brief	エフェクトエンジンの取得。
		 */
		CEffectEngine& GetEffectEngine()
		{
			return m_effectEngine;
		}
		/*!
		* @brief	指向性シャドウマップを取得。
		*/
		CDirectionShadowMap& GetDirectionShadowMap()
		{
			return m_preRender.GetDirectionShadowMap();
		}
		/*!
		* @brief	全方位シャドウマップを取得。
		*/
		COminiDirectionShadowMap& GetOminiDirectionShadowMap()
		{
			return m_preRender.GetOminiDirectionShadowMap();
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
		ID3D11DeviceContext* GetD3DImmediateDeviceContext() const
		{
			return m_pImmediateContext;
		}
		/*!
		*@brief	描画コマンドを積むのに使用しているID3D11DeviceContextの取得。
		*/
		ID3D11DeviceContext* GetD3DDeviceContext() const
		{
			return m_renderContext.GetD3DDeviceContext();
		}
		/// <summary>
		/// マルチスレッドレンダリングを行っている？
		/// </summary>
		/// <returns>trueならマルチスレッドレンダリングを行っている。</returns>
		bool IsMultithreadRendering() const
		{
			return m_pDeferredDeviceContext != nullptr;
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
		/// <summary>
		/// メインスレッドから呼び出す描画開始の処理。
		/// </summary>
		void BeginRender();
		/*!
		* @brief	描画終了。
		*/
		/// <summary>
		/// メインスレッドから呼び出す描画終了処理。
		/// これが1フレームの最後の描画処理になる。
		/// </summary>
		void EndRender();
		/// <summary>
		/// ゲームスレッドから呼び出す終了処理。
		/// </summary>
		void EndRenderFromGameThread();
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
#else
			(void)eventName;
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
		/*!
		*@brief	ディファードシェーディング。
		*/
		void DefferdShading(CRenderContext& rc);
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
		/*!
		*@brief	ディファードシェーディングの初期化。
		*/
		void InitDefferdShading();
	private:
		/*!
		*@brief	これのメンバを変更したら、defferdShading.fxのPSDefferdCbも変更するように！！！
		*/
		struct PSDefferdCb {
			CMatrix mViewProjInv;		//!<ビュープロジェクション行列の逆行列。
		};
		ID3D11Device*			m_pd3dDevice = nullptr;						//!<D3D11デバイス。
		CRenderContext			m_renderContext;							//!<レンダリングコンテキスト。
		ID3D11DeviceContext*	m_pImmediateContext = nullptr;				//!<D3D11即時デバイスコンテキスト。
		ID3D11DeviceContext*	m_pDeferredDeviceContext = nullptr;			//!<D3D11ディファードデバイスコンテキスト。
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
		const int				m_2dSpaceScreenWidth = 1280;				//!<2D空間のスクリーンの幅。2Dはこの座標系を前提に表示されていれば、解像度が変わっても大丈夫。
		const int				m_2dSpaceScreenHeight = 720;				//!<2D空間のスクリーンの高さ。2Dはこの座標系を前提に表示されていれば、解像度が変わっても大丈夫。
		CDirectXModelResource	m_directXModelResource;						//!<DirectXモデルリソースの管理者。
		CConstantBuffer			m_cbDefferd;								//!<ディファードシェーディング用の定数バッファ。
		CShader					m_vsDefferd;								//!<ディファードシェーディング用の頂点シェーダー。
		CShader					m_psDefferd;								//!<ディファードシェーディング用のピクセルシェーダー。
		ID3D11CommandList*		m_commandList[2] = { nullptr };				//!<コマンドリスト。
		D3D11_FEATURE_DATA_THREADING m_featureDataThreading;				//!<グラフィックドライバでサポートされているマルチスレッド機能についての情報。
		int m_commandListNoMainThread = 0;									//!<メインスレッドでアクセスしているコマンドリストの番号。
		CConstantBuffer m_cb;
	};
}