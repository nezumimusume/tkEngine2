/*!
 *@brief	エンジン。
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

namespace tkEngine{
	/*!
	 *@brief	エンジン。
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
		bool Init( const SInitParam& initParam );
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
			if(instance == nullptr){
				instance = new CEngine;
			}
			return *instance;
		}
		
		/*!
		* @brief	ゲームパッドの取得。
		*@param[in]	padNo	パッド番号
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
		* @brief	ZPrepassの取得。
		*/
		CZPrepass& GetZPrepass()
		{
			return m_graphicsEngine.GetZPrepass();
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
	private:
		/*!
		 *@brief	ウィンドウ初期化。
		 */
		bool InitWindow( const SInitParam& initParam );

		/*!
		* @brief	ウィンドウプロシージャ。
		*/
		static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		/*!
		* @brief	更新。
		*/
		void Update();
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
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
		std::unique_ptr<CFont>	m_font;
		float					m_fps = 30.0f;
		float					m_timeTotal = 0.0f;
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
	
}
