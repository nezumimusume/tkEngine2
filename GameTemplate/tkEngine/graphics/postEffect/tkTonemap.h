/*!
 *@brief	トーンマップ。
 */
#pragma once

namespace tkEngine{
	/*!
	 *@brief	トーンマップ。
	 */
	class CTonemap{
	public:
		CTonemap();
		~CTonemap();
		/*!
		*@brief	作成。
		*@param[in]		config		グラフィック設定。
		*/
		void Init(const SGraphicsConfig& config);
		void Release();
		/*!
		 *@brief	平均輝度の計算。
		 *@param[in]	renderContext		レンダリングコンテキスト
		 */
		void CalcLuminanceAvarage(CRenderContext& renderContext, CPostEffect* postEffect);
		/*!
		 *@brief	トーンマップの実行。
		 *@details
		 * この関数を呼ぶ前にCalcLuminanceAvarageをコールして平均輝度の計算を行ってください。
		 * 平均輝度の計算はポストエフェクトの戦闘で行ってください。
		 *@param[in]	renderContext		レンダリングコンテキスト
		 */
		void Render(CRenderContext& renderContext, CPostEffect* postEffect);
		/*!
		*@brief	トーンマップの処理をリセット。
		*@details
		*　輝度が大きく変わるシーン切り替えなどが発生する場合などに明暗順応がおかしく見える場合があります。
		* そのような場合はシーン切り替えのタイミングでリセットを行ってみてください。
		*/
		void Reset()
		{
			m_isFirstWhenChangeScene = true;
		}
		/*!
		*@brief	 明るさを設定。
		*@@param[in]	lum		明るさ。この値を大きくすると明るくなる。
		*/
		void SetLuminance(float lum)
		{
			m_tonemapParam.midddleGray = lum;
		}
		/*!
		*@brief	 トーンマップの有効無効を設定。
		*/
		void SetEnable(bool flag)
		{
			m_isEnable = flag;
		}
	public:
		static const int MAX_SAMPLES = 16;
	private:
		struct STonemapParam {
			float deltaTime;
			float midddleGray;
		};
		static const int NUM_CALC_AVG_RT = 5;				//!<平均輝度計算用のレンダリングターゲットの枚数。
//		CEffect*		m_effect = nullptr;					//!<エフェクト。
		bool			m_isEnable = false;					//!<トーンマップ有効？
		CRenderTarget	m_calcAvgRT[NUM_CALC_AVG_RT];		//!<平均輝度計算用のレンダリングターゲット。
		CRenderTarget   m_avgRT[2];							//!<平均輝度が格納されるレンダリングターゲット。
		int				m_currentAvgRT = 0;					//!<
		CShader			m_vsShader;							//!<頂点シェーダー。
		CShader			m_psCalcLuminanceLogAvarageShader;	//!<輝度の対数平均を求めるピクセルシェーダー。
		CShader			m_psCalcLuminanceAvarageShader;		//!<輝度の平均を求めるピクセルシェーダー。
		CShader			m_psCalcLuminanceExpAvarageShader;	//!<輝度の指数平均を求めるピクセルシェーダー。
		CShader			m_psCalcAdaptedLuminanceShader;		//!<明暗順応のピクセルシェーダー。
		CShader			m_psCalcAdaptedLuminanceFirstShader;	//!<明暗順応のピクセルシェーダー。(シーンが切り替わったときに使用される。)
		CShader			m_psFinal;							//!<最終合成シェーダー。
		bool			m_isFirstWhenChangeScene = true;	//!<シーンが切り替わって初回の描画かどうかのフラグ。
		CVector4		m_avSampleOffsets[MAX_SAMPLES];
		CConstantBuffer	m_cbCalcLuminanceLog;
		CConstantBuffer m_cbTonemapCommon;
		STonemapParam	m_tonemapParam;
	};
}