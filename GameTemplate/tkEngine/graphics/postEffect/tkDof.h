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
	private:
		/// <summary>
		/// シーンテクスチャにバイリニアフィルタをかけて、カメラに近い側のCoC(錯乱円の半径)の計算も行います。
		/// </summary>
		void ApplyBilinearFilterAndCalcNearCoc(CRenderContext& rc, CPostEffect* postEffect);
		/// <summary>
		/// レンダリングターゲットを初期化。
		/// </summary>
		void InitRenderTargets();
		/// <summary>
		/// シェーダーを初期化。
		/// </summary>
		void InitShaders();
	private:
		bool m_isEnable = false;
		CRenderTarget m_cocAndColorRt;				//ピクセルのCoCの計算結果とバイリニアフィルタをかけたカラーが描き込まれるレンダリングターゲット。
		CShader m_applyBinFilterAndCalcNearCocVS;	//
		CShader m_applyBinFilterAndCalcNearCocPS;
										
	};
}