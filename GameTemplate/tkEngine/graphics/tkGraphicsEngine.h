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
	/// .3  フォワードシェーディング
	///			半透明や特殊なシェーディングを行うマテリアルの描画を行う。
	/// .4  ポストエフェクト
	///			1-1  トーンマップ。
	///			1-2  Effekseerを利用したエフェクト描画。
	///			1-3  Temporal Screen Space Reflection。
	///			1-4  川瀬式ブルーム。
	///			1-5  DOF
	///			1-6  アンチエイリアス(FXAA)。
	///			1-7  ディザリング。
	/// .5  ポストレンダリング
	///			HUDなどポストエフェクトをかけたくない描画を行う。
	/// </remarks>
	class CGraphicsEngine : Noncopyable {
	public:
		CGraphicsEngine();
		~CGraphicsEngine();
		
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="hwnd">ウィンドウハンドル</param>
		/// <param name="initParam">初期化用のパラメータ。</param>
		/// <returns></returns>
		bool Init(HWND hwnd, const SInitParam& initParam);

		/// <summary>
		/// 開放。
		/// </summary>
		/// <remarks>
		/// エンジン内部で利用されています。ユーザーは利用しないでください。
		/// </remarks>
		void Release();

		/// <summary>
		/// GBufferRenderクラスのインスタンスを取得。
		/// </summary>
		CGBufferRender& GetGBufferRender()
		{
			return m_preRender.GetGBufferRender();
		}

		/// <summary>
		/// CEffectEngineクラスのインスタンスを取得。
		/// </summary>
		CEffectEngine& GetEffectEngine()
		{
			return m_effectEngine;
		}
		/*!
		* @brief	指向性シャドウマップを取得。
		*/
		/// <summary>
		/// 指向性シャドウマップクラスのインスタンスを取得。
		/// </summary>
		CDirectionShadowMap& GetDirectionShadowMap()
		{
			return m_preRender.GetDirectionShadowMap();
		}
		/// <summary>
		/// 全方位シャドウマップクラスのインスタンスを取得。
		/// </summary>
		COminiDirectionShadowMap& GetOminiDirectionShadowMap()
		{
			return m_preRender.GetOminiDirectionShadowMap();
		}
		/// <summary>
		/// トーンマップクラスのインスタンスを取得。
		/// </summary>
		CTonemap& GetTonemap()
		{
			return m_postEffect.GetTonemap();
		}
		/// <summary>
		/// 3Dカメラを取得取。
		/// </summary>
		CCamera& GetMainCamera()
		{
			return m_mainCamera;
		}
		/// <summary>
		/// 2Dカメラを取得。
		/// </summary>
		CCamera& Get2DCamera()
		{
			return m_2dCamera;
		}
		/// <summary>
		/// D3Dデバイスを取得。
		/// </summary>
		ID3D11Device* GetD3DDevice() const
		{
			return m_pd3dDevice;
		}
	
		/// <summary>
		/// D3D即時デバイスコンテキストの取得。
		/// </summary>
		/// <returns></returns>
		ID3D11DeviceContext* GetD3DImmediateDeviceContext() const
		{
			return m_pImmediateContext;
		}
		/// <summary>
		/// 描画コマンドを積むのに使用してるD3Dデバイスコンテキストを取得。
		/// </summary>
		/// <returns></returns>
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
		/// <summary>
		/// フレームバッファの幅を取得。
		/// </summary>
		int GetFrameBufferWidth() const
		{
			return m_frameBufferWidth;
		}
		/// <summary>
		/// フレームバッファの高さを取得。
		/// </summary>
		int GetFrameBufferHeight() const
		{
			return m_frameBufferHeight;
		}
		/// <summary>
		/// 2D空間のスクリーンの幅を取得。
		/// </summary>
		int Get2DSpaceScreenWidth() const
		{
			return m_2dSpaceScreenWidth;
		}
		/// <summary>
		/// 2D空間のスクリーンの高さを取得。
		/// </summary>
		int Get2DSpaceScreenHeight() const
		{
			return m_2dSpaceScreenHeight;
		}
		/// <summary>
		/// メインレンダリングターゲットの取得。
		/// </summary>
		/// <returns></returns>
		CRenderTarget& GetMainRenderTarget()
		{
			return m_mainRenderTarget;
		}
		/// <summary>
		/// メインスレッドから呼び出す描画開始の処理。
		/// </summary>
		void BeginRender();

		/// <summary>
		/// メインスレッドから呼び出す描画終了処理。
		/// これが1フレームの最後の描画処理になる。
		/// </summary>
		void EndRender();

		/// <summary>
		/// ゲームスレッドから呼び出す終了処理。
		/// </summary>
		void EndRenderFromGameThread();

		/// <summary>
		/// プリレンダリングのインスタンスを取得。
		/// </summary>
		/// <returns></returns>
		CPreRender& GetPreRender()
		{
			return m_preRender;
		}
		/// <summary>
		/// ポストエフェクトのインスタンスを取得。
		/// </summary>
		/// <returns></returns>
		CPostEffect& GetPostEffect()
		{
			return m_postEffect;
		}
		/// <summary>
		/// レンダリングコンテキストのインスタンスを取得。
		/// </summary>
		/// <returns></returns>
		CRenderContext& GetRenderContext()
		{
			return m_renderContext;
		}
		/// <summary>
		/// ライトの管理者のインスタンスを取得。
		/// </summary>
		/// <returns></returns>
		CLightManager& GetLightManager()
		{
			return m_lightManager;
		}

		/// <summary>
		/// メインレンダリングターゲットのMSAAの設定を取得。
		/// </summary>
		/// <returns></returns>
		const DXGI_SAMPLE_DESC& GetMainRenderTargetMSAADesc() const
		{
			return m_mainRenderTargetMSAADesc;
		}

		/// <summary>
		/// GPUイベント開始。
		/// </summary>
		/// <param name="eventName">イベントの名前</param>
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
		/// <summary>
		/// GPUイベント終了。
		/// </summary>
		void EndGPUEvent()
		{
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
			if (m_userAnnoation) {
				m_userAnnoation->EndEvent();
			}
#endif
		}
		/// <summary>
		/// シェーダーリソースの取得。
		/// </summary>
		/// <returns></returns>
		CShaderResources& GetShaderResources()
		{
			return m_shaderResources;
		}
		/*!
		*@brief	SpriteBatchの取得。
		*@details
		* ゲーム層では使用しないように。
		*/
		/// <summary>
		/// SpriteBatchの取得
		/// </summary>
		/// <remarks>
		/// エンジン内部で使用されます。ゲーム層では使用しないように。
		/// </remarks>
		/// <returns></returns>
		DirectX::SpriteBatch* GetSpriteBatch() const
		{
			return m_spriteBatch.get();
		}
		/// <summary>
		/// SpriteFontの取得。
		/// </summary>
		/// <remarks>
		/// エンジン内部で使用されます。ゲーム層では使用しないように。
		/// </remarks>
		/// <returns></returns>
		DirectX::SpriteFont* GetSpriteFont() const
		{
			return m_spriteFont.get();
		}
		/// <summary>
		/// ポストエフェクトの処理が完了したときに呼ばれる処理。
		/// </summary>
		/// <remarks>
		/// エンジン内部で使用されます。ゲーム層では使用しないように。
		/// </remarks>
		/// <param name="rc">レンダリングコンテキスト</param>
		void EndPostEffect(CRenderContext& rc);
		
		/// <summary>
		/// DirectXモデルリソースの取得
		/// </summary>
		/// <returns></returns>
		CDirectXModelResource& GetDirectXModelResource()
		{
			return m_directXModelResource;
		}
		/// <summary>
		/// ディファードシェーディングの実行。
		/// </summary>
		/// <param name="rc"></param>
		void DefferdShading(CRenderContext& rc);
	private:
		/// <summary>
		/// D3Dデバイスとスワップチェインの初期化。
		/// </summary>
		/// <param name="hwnd">ウィンドウハンドル</param>
		/// <param name="initParam">初期化パラメータ</param>
		/// <returns>trueが返ってきたら初期化成功</returns>
		bool InitD3DDeviceAndSwapChain(HWND hwnd, const SInitParam& initParam);

		/// <summary>
		/// バックバッファの初期化。
		/// </summary>
		/// <returns>trueが返ってきたら初期化成功</returns>
		bool InitBackBuffer();
		/// <summary>
		/// メインレンダリングターゲットの初期化。
		/// </summary>
		/// <returns></returns>
		bool InitMainRenderTarget();
	
		/// <summary>
		/// ディファードシェーディングの初期化。
		/// </summary>
		void InitDefferdShading();
	private:
		/// <summary>
		/// この構造体のメンバを変更したら、defferdShading.fxのPSDefferdCbも変更するように！！！
		/// </summary>
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