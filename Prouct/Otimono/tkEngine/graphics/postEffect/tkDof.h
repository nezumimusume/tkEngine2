/// <summary>
/// DOF(被写界深度)
/// </summary>
#pragma once

#include "tkEngine/graphics/postEffect/tkDofBlur.h"

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
		/// カメラ−＞＿＿＿＿＿＿＿＿＿＿|＿＿＿＿＿＿＿＿＿＿＿＿＿|____________________|____________________＿|________________＿＿＿＿＿＿＿＿
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
		void GetDofRangeParam(float& nearStartDistance, float& nearEndDistance, float& farStartDistance, float& farEndDistance)
		{
			nearStartDistance = m_nearStartDistance;
			nearEndDistance = m_nearEndDistance;
			farStartDistance = m_farStartDistance;
			farEndDistance = m_farEndDistance;
		}
		/// <summary>
		/// DoFを有効にする。
		/// </summary>
		void Enable()
		{
			m_isEnable = true;
		}
		/// <summary>
		/// DoFを無効にする。
		/// </summary>
		void Disable()
		{
			m_isEnable = false;
		}
	private:
		/// <summary>
		/// 錯乱円テクスチャの作成行う描画パス。
		/// </summary>
		void CreateCoCTexture(CRenderContext& rc, CPostEffect* postEffect);
		/// <summary>
		/// ボケ画像の作成を行う描画パス。
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="postEffect"></param>
		void CreateBokeTexture(CRenderContext& rc, CPostEffect* postEffect);
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
		/// 定数バッファの初期化。
		/// </summary>
		void InitConstantBuffers();
		/// <summary>
		/// ブレンディングステートを初期化する。
		/// </summary>
		void InitBlendStates();
		
	private:
		/// <summary>
		/// CreateCocのパスで使用する定数バッファの構造体。
		/// </summary>
		/// <remarks>
		/// この構造体の中身を変更したら、Assets/shader/dof/dof_CreateCoc.fxの
		/// cbParamの内容も変更する。
		/// </remarks>
		struct SCreateCocParamCB {
			CVector4 dofRange;				//被写界深度の範囲を表すデータ。SetDofRangeParam関数で設定されるデータ。
											//xは手前ボケの減衰が開始するカメラからの距離。
											//yは手前ボケの減衰が終了するカメラからの距離。
											//zは奥ボケの増幅が開始するカメラからの距離。
											//wは奥ボケの増幅が終了するカメラからの距離。			
		};
		/// <summary>
		/// CreateCoCTextureのパスで使用するデータ集。
		/// </summary>
		struct SCreateCoCTexture {
			CRenderTarget calcCocAndColorRt;			//ピクセルのCoCの計算結果が描き込まれるレンダリングターゲット。CoCはαチャンネルに書き込まれます。
			CShader vs;									//頂点シェーダー。
			CShader ps;									//ピクセルシェーダー。
			ID3D11BlendState* blendState = nullptr;		//アルファブレンディングステート。
			CConstantBuffer cb;							//定数バッファ。
		};
		//ダウンサンプリングの回数
		static const int NUM_DOWN_SAMPLING = 2;
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
			CDofBlur blur;		 //todo Release関数がないぞ。						//ブラー。
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
		};
		bool m_isEnable = false;	//Dofが有効かどうかのフラグ。
		SCreateCoCTexture m_createCocParam;
		SDownSamplingCocAndColor m_downSampligCocAndColorParam;
		SFinal m_finalParam;

		float m_nearStartDistance = 0.0f;		//手前ボケの減衰が開始するカメラからの距離。
		float m_nearEndDistance = 150.0f;		//手前ボケの減衰が終了するカメラからの距離。
		float m_farStartDistance = 200.0f;		//奥ボケの増幅が開始するカメラからの距離。
		float m_farEndDistance = 500.0f;		//奥ボケの増幅が終了するカメラからの距離。
	};
}