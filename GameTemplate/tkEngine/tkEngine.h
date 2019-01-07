/*!
 *@file	tkEngine.h
 */
#pragma once

#include "tkEngine/graphics/tkRenderTarget.h"
#include "tkEngine/timer/tkStopwatch.h"
#include "tkEngine/Sound/tkSoundEngine.h"
#include "tkEngine/graphics/tkGraphicsEngine.h"
#include "tkEngine/tkEngineStruct.h"
#include "tkEngine/physics/tkPhysics.h"
#include "tkEngine/random/tkRandom.h"
#include "tkEngine/graphics/font/tkFont.h"
#include "tkEngine/debug/math/tkVectorRender.h"
#include <thread>
#include <mutex>
#include <condition_variable>

 /*!
  *@namespace	tkEngine全体の名前空間。
  */
namespace tkEngine{
	/*!
	 *@brief	河原学園内製ゲームエンジン version 2.2
	 */
	class CEngine : Noncopyable {
	private:
		/*!
		 *@brief	コンストラクタ。
		 */
		CEngine();
		/*!
		 *@brief	デストラクタ。
		 */
		~CEngine();
	public:
		/*!
		 *@brief	初期化。
		 *@param[in]	initParam		初期化パラメータ。
		 */
		bool Init(const SInitParam& initParam);
		/*!
		 *@brief	終了処理。
		 */
		void Final();
		/*!
		 *@brief	ゲームループを実行。
		 */
		void RunGameLoop();
		/*!
		 *@brief	インスタンスを取得。
		 */
		static CEngine& GetInstance()
		{
			static CEngine* instance = nullptr;
			if (instance == nullptr) {
				instance = new CEngine;
			}
			return *instance;
		}

		/*!
		* @brief	ゲームパッドの取得。
		*@param[in]	padNo	パッド番号。CPad::CONNECT_PAD_MAX-1まで指定可能。
		*/
		CPad& GetPad(int padNo)
		{
			TK_ASSERT(padNo < CPad::CONNECT_PAD_MAX && padNo >= 0, "padNo is invalid");
			return m_pad[padNo];
		}
		/*!
		* @brief	サウンドエンジンの取得。
		*/
		CSoundEngine& GetSoundEngine()
		{
			return m_soundEngine;
		}
		/*!
		* @brief	グラフィックスエンジンの取得。
		*/
		CGraphicsEngine& GetGraphicsEngine()
		{
			return m_graphicsEngine;
		}
		/*!
		 *@brief	物理ワールドの取得。
		 */
		CPhysicsWorld& GetPhysicsWorld()
		{
			return m_physicsWorld;
		}
		/*!
		*@brief	メインカメラを取得。
		*/
		CCamera& GetMainCamera()
		{
			return m_graphicsEngine.GetMainCamera();
		}
		/*!
		*@brief	2Dカメラを取得。
		*/
		CCamera& GetMainCamera2D()
		{
			return m_graphicsEngine.Get2DCamera();
		}
		/*!
		 *@brief	乱数取得。
		 */
		CRandom& GetRandom()
		{
			return m_random;
		}
		/*!
		*@brief		リソースのアンロード。
		*@details
		* 常駐リソース以外のリソースをすべてアンロードします。
		* シーンの切り替えなどで、リソースのアンロードが必要な場合に使用してください。
		*/
		void ResourceUnload();
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
		prefab::CVectorRender* GetVectorRender()
		{
			return m_vectorRender;
		}
#endif
	private:
		/*!
		 *@brief	ウィンドウ初期化。
		 */
		bool InitWindow( const SInitParam& initParam );
		
		/// <summary>
		/// ゲームスレッド。
		/// </summary>
		void GameThread();
		/*!
		* @brief	ウィンドウプロシージャ。
		*@param[in]	hWnd	ウィンドウハンドル
		*@param[in] msg		メッセージ
		*@param[in] wParam	WPARAM
		*@param[in] lParam	LPARAM
		*/
		static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		/*!
		* @brief	更新。
		*/
		void Update();
		/// <summary>
		/// ゲーム部分の更新処理。
		/// </summary>
		void GameUpdate();
	private:
		HINSTANCE				m_hInst = nullptr;	//!<アプリケーションのインスタンス。
		HWND					m_hWnd = nullptr;	//!<ウィンドウハンドル。
		CGraphicsEngine			m_graphicsEngine;							//!<グラフィックエンジン。
		CSoundEngine			m_soundEngine;								//!<サウンドエンジン。
		CPhysicsWorld			m_physicsWorld;								//!<物理ワールド。
		int						m_screenWidth = 0;							//!<スクリーンの幅。
		int						m_screenHeight = 0;							//!<スクリーンの高さ。
		CPad					m_pad[CPad::CONNECT_PAD_MAX];				//!<ゲームパッド。
		CRandom					m_random;									//!<乱数。
		std::unique_ptr<std::thread> m_gameThread;							//ゲームスレッド。
		bool			m_isRunGameThread = false;
		bool			m_isReqDeadGameThread = false;
		std::mutex		m_isRunGameThreadMtx;
		std::condition_variable m_isRunGameThreadCv;
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
		std::unique_ptr<CFont>	m_font;
		float					m_fps = 30.0f;
		float					m_timeTotal = 0.0f;
		prefab::CVectorRender*  m_vectorRender = nullptr;					//!<ベクトルレンダラー。
#endif
	public:
		CStopwatch				m_sw;
	};

	/*!
	* @brief	Engineのインスタンスを取得。
	*/
	static inline CEngine& Engine()
	{
		return CEngine::GetInstance();
	}
	/*!
	* @brief	CPadのインスタンスを取得。
	*/
	static inline CPad& Pad(int padNo)
	{
		return Engine().GetPad(padNo);
	}
	/*!
	* @brief	CSoundEngineのインスタンスを取得。
	*/
	static inline CSoundEngine& SoundEngine()
	{
		return Engine().GetSoundEngine();
	}
	/*!
	* @brief	CGraphicsEngineのインスタンスを取得。
	*/
	static inline CGraphicsEngine& GraphicsEngine()
	{
		return Engine().GetGraphicsEngine();
	}
	/*!
	 *@brief	CPhysicsWorldのインスタンスを取得。
	 */
	static inline CPhysicsWorld& PhysicsWorld()
	{
		return Engine().GetPhysicsWorld();
	}
	/*!
	* @brief	メインカメラのインスタンスを取得。
	*/
	static inline CCamera& MainCamera()
	{
		return GraphicsEngine().GetMainCamera();
	}
	/*!
	* @brief	メインカメラ(2D)のインスタンスを取得。
	*/
	static inline CCamera& MainCamera2D()
	{
		return GraphicsEngine().Get2DCamera();
	}
	/*!
	* @brief	CLightManagerのインスタンスを取得。
	*/
	static inline CLightManager& LightManager()
	{
		return GraphicsEngine().GetLightManager();
	}
	/*!
	* @brief	GPUイベントの開始。
	*/
	static inline void BeginGPUEvent(const wchar_t* name)
	{
		GraphicsEngine().BeginGPUEvent(name);
	}
	/*!
	* @brief	GPUイベントの終了。
	*/
	static inline void EndGPUEvent()
	{
		GraphicsEngine().EndGPUEvent();
	}
	/*!
	 *@brief	乱数を取得。
	 */
	static inline CRandom& Random()
	{
		return Engine().GetRandom();
	}
	/*!
	*@brief	G-BufferのSRVを取得。
	*/
	static inline CShaderResourceView& GetGBufferSRV(EnGBuffer gBuffer)
	{
		return Engine().GetGraphicsEngine().GetGBufferRender().GetRenderTarget(gBuffer).GetRenderTargetSRV();
	}
	/// <summary>
	/// ポストエフェクトの機能に簡易的にアクセスするための名前空間。
	/// </summary>
	namespace postEffect {
		/// <summary>
		/// 被写界深度にアクセス
		/// </summary>
		static inline CDof& Dof()
		{
			return GraphicsEngine().GetPostEffect().GetDof();
		}
		/// <summary>
		/// トーンマップにアクセス。
		/// </summary>
		/// <returns></returns>
		static inline CTonemap& Tonemap()
		{
			return GraphicsEngine().GetPostEffect().GetTonemap();
		}
	}
	//デバッグ機能を簡易的にアクセスするための名前空間
	namespace dbg {
		
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
		/*!
		* @brief	コリジョンのデバッグ表示を有効にする。
		*@param[in]	colliObj	コリジョンオブジェクト。
		*/
		static inline void SetDrawPhysicsCollisionEnable()
		{
			PhysicsWorld().SetDebugDrawMode(1);
		}
		/*!
		* @brief	コリジョンのデバッグ表示を有効にする。
		*@param[in]	colliObj	コリジョンオブジェクト。
		*/
		static inline void SetDrawPhysicsCollisionDisable()
		{
			PhysicsWorld().SetDebugDrawMode(0);
		}
		/*!
		* @brief	ベクトルのデバッグ出力。
		*/
		static inline void DrawVector(CVector3 vector, CVector3 origin, const char* name = "no name")
		{
			prefab::CVectorRender::SRenderData renderData = { name, vector, origin };
			Engine().GetVectorRender()->AddVector(renderData);
		}
		/*!
		* @brief	ベクトルのデバッグ描画を有効にする。
		*/
		static inline void SetDrawVectorEnable()
		{
			Engine().GetVectorRender()->SetRenderFlag(true);
		}
		/*!
		* @brief	ベクトルのデバッグ描画を無効にする。
		*/
		static inline void SetDrawVectorDisable()
		{
			Engine().GetVectorRender()->SetRenderFlag(false);
		}
		static inline void SetDrawVectorNameList(const std::vector<std::string>& list)
		{
			Engine().GetVectorRender()->SetRenderVectorNameList(list);
		}
#else

		static inline void SetDrawPhysicsCollisionEnable()
		{
		}

		static inline void SetDrawPhysicsCollisionDisable()
		{
		}

		static inline void DrawVector(CVector3 vector, CVector3 origin, const char* name = "no name")
		{
		}
		
		static inline void SetDrawVectorEnable()
		{
		}
	
		static inline void SetDrawVectorDisable()
		{
		}
		static inline void SetDrawVectorNameList(const std::vector<std::string>& list)
		{
		}
#endif

	}
}


