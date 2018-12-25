/// <summary>
/// DOF(被写界深度)
/// </summary>
#pragma once

namespace tkEngine {
	class CPostEffect;
	/// <summary>
	/// DOF(被写界深度)
	/// </summary>
	class CDof : Noncopyable {
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		CDof();
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~CDof();
		/// <summary>
		/// 解放。
		/// </summary>
		/// <remarks>
		/// 本関数はデストラクタから呼び出されているため、通常
		/// 明示的なタイミングでリソースを解放したい場合に呼び出してください。
		/// </remarks>
		void Release();
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="config">グラフィック設定</param>
		void Init(const SGraphicsConfig& config);
		/// <summary>
		/// 更新。
		/// </summary>
		void Update();
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="postEffect">ポストエフェクト</param>
		void Render(CRenderContext& rc, CPostEffect* postEffect);
		/// <summary>
		/// 被写界深度の範囲に影響するパラメータを設定。
		/// </summary>
		/// <param name="nearStartDistance">手前ボケの減衰が開始するカメラからの距離</param>
		/// <param name="nearEndDistance">手前ボケの減衰が終了するカメラからの距離</param>
		/// <param name="farStartDistance">奥ボケの増幅が開始するカメラからの距離</param>
		/// <param name="farEndDistance">奥ボケの増幅が終了するカメラからの距離</param>
		/// <remarks>
		/// 
		///                      nearStartDistance          nearEndDistance         farStartDistance    　　farEndDistance
		///　　　　　　　　　　　　　　　 |　　　　　　　　　　　　  |                    |                      |
		/// 　　　　　ここは最大の手前　　|手前ボケが発生している　　|被写界深度内 。     |奥ボケが発生している　|ここから後ろは最大の奥ボケが
		///　　　　　　ボケが発生している |　　　　　　　　　　　　　|ボケが発生していない|                    　|発生している
		/// カメラ－＞＿＿＿＿＿＿＿＿＿＿|＿＿＿＿＿＿＿＿＿＿＿＿＿|____________________|____________________＿|________________＿＿＿＿＿＿＿＿
		/// 
		/// </remarks>
		void SetDofRangeParam(float nearStartDistance, float nearEndDistance, float farStartDistance, float farEndDistance)
		{
			//【重要！！！！！】
			//endは必ずstartより大きくなることを保証するように！
			//もし保証しなくなると、シェーダー側で0除算が発生する可能性が生まれます。
			m_nearStartDistance = std::max<float>(0.0f, nearStartDistance);
			m_nearEndDistance = std::max<float>(m_nearStartDistance + 0.001f, nearEndDistance);
			m_farStartDistance = std::max<float>(m_nearEndDistance + 0.001f, farStartDistance);
			m_farEndDistance = std::max<float>(m_farStartDistance + 0.001f, farEndDistance);
		}
	private:
		/// <summary>
		/// ピクセル単位のDofマスクの作成とCocの計算を行う描画パス。
		/// </summary>
		void CreateDofMaskAndCalcCoc(CRenderContext& rc, CPostEffect* postEffect);
		/// <summary>
		/// シーンのカラー情報とCoC情報のダウンサンプリングを行う描画パス。
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="postEffect"></param>
		void DownSamplingCocAndColor(CRenderContext& rc, CPostEffect* postEffect);
		/// <summary>
		/// 最終シーン合成の描画パス。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="postEffect">ポストエフェクト</param>
		void Final(CRenderContext& rc, CPostEffect* postEffect);
		/// <summary>
		/// レンダリングターゲットを初期化。
		/// </summary>
		void InitRenderTargets();
		/// <summary>
		/// シェーダーを初期化。
		/// </summary>
		void InitShaders();
		/// <summary>
		/// サンプラステートの初期化。
		/// </summary>
		void InitSamplerStates();
		/// <summary>
		/// 定数バッファの初期化。
		/// </summary>
		void InitConstantBuffers();
		/// <summary>
		/// ブレンディングステートを初期化する。
		/// </summary>
		void InitBlendStates();
	private:
		/// <summary>
		/// CreateDofMaskAndCalcCocのパスで使用する定数バッファの構造体。
		/// </summary>
		/// <remarks>
		/// この構造体の中身を変更したら、Assets/shader/dof/dof_CreateDofMaskAndCalcCoc.fxの
		/// cbParamの内容も変更する。
		/// </remarks>
		struct SCreateDofMaskAndCalcCocCB {
			CVector4 dofRange;				//被写界深度の範囲を表すデータ。SetDofRangeParam関数で設定されるデータ。
											//xは手前ボケの減衰が開始するカメラからの距離。
											//yは手前ボケの減衰が終了するカメラからの距離。
											//zは奥ボケの増幅が開始するカメラからの距離。
											//wは奥ボケの増幅が終了するカメラからの距離。			
		};
		/// <summary>
		/// CreateDofMaskAndCalcCocのパスで使用するデータ集。
		/// </summary>
		struct SCreateDofMaskAndCalcCoc {
			CRenderTarget calcCocAndColorRt;			//ピクセルのCoCの計算結果が描き込まれるレンダリングターゲット。CoCはαチャンネルに書き込まれます。
			CRenderTarget dofMaskRt;					//被写界深度内のピクセルを表すマスク。0と1の2値化されたデータ。0は被写界深度内のピクセルなのでボケない。
			CShader vs;									//頂点シェーダー。
			CShader ps;									//ピクセルシェーダー。
			CSamplerState	samplerState;				//サンプラステート。
			ID3D11BlendState* blendState = nullptr;		//アルファブレンディングステート。
			CConstantBuffer cb;							//定数バッファ。
		};
		//ダウンサンプリングの回数
		static const int NUM_DOWN_SAMPLING = 4;
		/// <summary>
		/// DownSamplingCocAndColorCBのパスで使用する定数バッファの構造体。
		/// </summary>
		/// <remarks>
		/// この構造体の中身を変更したら、Assets/shader/dof/dof_DownSamplingCocAndColor.fxの
		/// cbParamの内容も変更する。
		/// </remarks>
		struct SDownSamplingCocAndColorCB {
			CVector2 invRenderTargetSize;	//レンダリングターゲットの逆数。
		};
		/// <summary>
		/// CoCの情報とシーンカラーの画像をダウンサンプリングしていく時に使用するデータ集。
		/// </summary>
		struct SDownSamplingCocAndColor {
			CRenderTarget downSamplingRt[NUM_DOWN_SAMPLING];
			CShader vs;
			CShader ps;
			CConstantBuffer cb;
		};
		/// <summary>
		/// 最終合成のパスで使用するデータ。
		/// </summary>
		struct SFinal {
			CShader vs;
			CShader ps;
			CSamplerState* pointSamplerState = nullptr;	//SCreateDofMaskAndCalcCocのsamplerStateを使わせていただく。
		};
		bool m_isEnable = false;	//Dofが有効かどうかのフラグ。
		SCreateDofMaskAndCalcCoc m_createDofMaskAndCalcCocParam;
		SDownSamplingCocAndColor m_downSampligCocAndColorParam;
		SFinal m_finalParam;

		float m_nearStartDistance = 0.0f;		//手前ボケの減衰が開始するカメラからの距離。
		float m_nearEndDistance = 150.0f;		//手前ボケの減衰が終了するカメラからの距離。
		float m_farStartDistance = 200.0f;		//奥ボケの増幅が開始するカメラからの距離。
		float m_farEndDistance = 500.0f;		//奥ボケの増幅が終了するカメラからの距離。
	};
}