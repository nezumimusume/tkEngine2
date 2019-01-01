/*!
 * @brief	シャドウマップ
 */

#pragma once

namespace tkEngine{
	class IShadowCaster;
	/*!
	 * @brief	シャドウマップ。
	 */
	class CShadowMap : Noncopyable{
	public:
		
		/*!
		 * @brief	コンストラクタ。
		 */
		CShadowMap();
		/*!
		 * @brief	デストラクタ。
		 */
		~CShadowMap();
		/*!
		 * @brief	シャドウマップの作成。
		 *@param[in]	config		コンフィグ。
		 *@return	trueが返ってきたらシャドウマップの作成成功。
		 */
		bool Init( const SShadowRenderConfig& config );
		/*!
		* @brief	更新。
		*/
		void Update();
		/*!
		 *@brief	影を落とすためのパラメータをGPUに転送する。
		 */
		void SendShadowReceiveParamToGPU(CRenderContext& rc);
		/*!
		 *@brief	シャドウマップへ書き込み。
		 */
		void RenderToShadowMap(CRenderContext& rc);
		/*!
		* @brief	開放。
		*/
		void Release();
		/*!
		* @brief	シャドウキャスターをエントリー。
		*/
		void Entry(IShadowCaster* caster);
		/// <summary>
		/// シャドウキャスターをリストから削除。
		/// </summary>
		void Remove(IShadowCaster* caster)
		{
			auto it = std::find(m_shadowCaster.begin(), m_shadowCaster.end(), caster);
			if (it != m_shadowCaster.end()) {
				//見つけたので削除。
				m_shadowCaster.erase(it);
			}
		}
		/*!
		 *@brief	ライトの方向を設定。
		 */
		void SetLightDirection(const CVector3 lightDir)
		{
			m_lightDirection = lightDir;
		}
		
		/*!
		* @brief	遠平面を設定。
		*/
		void SetFar(float Far)
		{
			m_far = Far;
		}
		/*!
		* @brief	遠平面を設定。
		*/
		void SetNear(float Near)
		{
			m_near = Near;
		}
		/*!
		 *@brief	ライトの高さを設定。
		 */
		void SetLightHeight(float h)
		{
			m_lightHeight = h;
		}
		/*!
		 *@brief	ソフトシャドウが有効？
		 */
		EnSoftShadowQualityLevel GetSoftShadowLevel() const
		{
			return m_softShadowLevel;
		}
	private:
		/*!
		 *@brief	この中身を変更したら、modelCB.hのShadowCbも変更するように。
		 */
		struct SShadowCb {
			CMatrix mLVP[NUM_SHADOW_MAP];
			CVector4 texOffset[NUM_SHADOW_MAP];
			float depthOffset[NUM_SHADOW_MAP];
		};
		bool m_isEnable = false;							//!<影の処理が有効？
		CVector3 m_lightDirection = CVector3::Down;			//!<ライトの方向。
		float m_near = 0.1f;								//!<近平面。
		float m_far = 100.0f;								//!<遠平面。
		float m_lightHeight = UnitM(20.0f);				//!<ライトの高さ。
		float m_accpect = 1.0f;								//!<アスペクト。不要？
		float m_shadowAreaW[NUM_SHADOW_MAP] = {0};			//!<影を落とす範囲の幅。
		float m_shadowAreaH[NUM_SHADOW_MAP] = {0};			//!<影を落とす範囲の高さ。
		CRenderTarget	m_shadowMapRT[NUM_SHADOW_MAP];		//!<シャドウマップを書き込むレンダリングターゲット。
		std::vector<IShadowCaster*> m_shadowCaster;			//!<シャドウキャスター。
		CMatrix	m_LVPMatrix[NUM_SHADOW_MAP] = { CMatrix::Identity };				//!<ライトビュープロジェクション行列。
		SShadowCb m_shadowCbEntity;
		CConstantBuffer m_shadowCb;							//!<影を落とす時に使用する定数バッファ。
		EnSoftShadowQualityLevel m_softShadowLevel = EnSoftShadowQualityLevel::enNone;							//!<ソフトシャドウのレベル。
	};
}