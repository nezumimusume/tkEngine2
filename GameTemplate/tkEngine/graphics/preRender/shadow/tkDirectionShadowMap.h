/*!
 * @brief	指向性シャドウマップ
 */

#pragma once

#include "tkEngine/graphics/preRender/shadow/tkIShadowMap.h"

namespace tkEngine{
	class IShadowCaster;
	/*!
	 * @brief	指向性シャドウマップ。
	 */
	class CDirectionShadowMap : public IShadowMap{
	public:
		
		/*!
		 * @brief	コンストラクタ。
		 */
		CDirectionShadowMap();
		/*!
		 * @brief	デストラクタ。
		 */
		~CDirectionShadowMap();
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
		* @brief	開放。
		*/
		void Release();
		/*!
		 *@brief	ライトの方向を設定。
		 */
		void SetLightDirection(const CVector3 lightDir)
		{
			m_lightDirection = lightDir;
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
		/// <summary>
		/// ライトの座標を計算する。
		/// </summary>
		/// <remarks>
		/// 分割された視推台を写すライトの座標を計算します。
		/// ライトの座標の計算はライトの高さ、ライトの方向、そして、
		/// 視推台の中心座標(近平面の中心座標と遠平面の中心座標を結んだ線分の中点座標)
		/// から計算されます。
		/// これらのパラメータをもとにライトの座標は下記の計算で求められます。
		/// 
		/// ①　未知数αについての一次方程式を解きます。
		/// ライトの高さ = 視推台の中心座標.y + ライトの方向.y * α
		/// α = ( ライトの高さ - 視推台の中心座標.y ) / ライトの方向.y
		/// ②　αを使って、ライトの座標を求める。
		/// ライトの座標 = 視推台の中心座標 + ライトの方向 * α
		/// </remarks>
		/// <param name="lightHeight">
		/// ライトの高さ。
		/// </param>
		/// <param name="viewFrustomCenterPosition">
		/// 分割された視推台の中心座標。
		/// </param>
		/// <returns>計算されたライトの座標</returns>
		CVector3 CalcLightPosition(float lightHeight, CVector3 viewFrustomCenterPosition);
		/*!
		 *@brief	シャドウマップへ書き込み。
		 */
		void RenderToShadowMapImp(CRenderContext& rc) override;
	private:
		/*!
		 *@brief	この中身を変更したら、modelCB.hのShadowCbも変更するように。
		 */
		struct SShadowCb {
			CMatrix mLVP[NUM_SHADOW_MAP];
			CVector4 texOffset[NUM_SHADOW_MAP];
			float depthOffset[NUM_SHADOW_MAP];
			float pading;
			float shadowAreaDepthInViewSpace[NUM_SHADOW_MAP];	//カメラ空間での影を落とすエリアの深度テーブル。
		};
		CVector3 m_lightDirection = CVector3::Down;			//!<ライトの方向。
		float m_lightHeight = UnitM(20.0f);				//!<ライトの高さ。
		float m_accpect = 1.0f;								//!<アスペクト。不要？
		float m_shadowAreaW[NUM_SHADOW_MAP] = {0};			//!<影を落とす範囲の幅。
		float m_shadowAreaH[NUM_SHADOW_MAP] = {0};			//!<影を落とす範囲の高さ。
		CRenderTarget	m_shadowMapRT[NUM_SHADOW_MAP];		//!<シャドウマップを書き込むレンダリングターゲット。
		CMatrix	m_LVPMatrix[NUM_SHADOW_MAP] = { CMatrix::Identity };				//!<ライトビュープロジェクション行列。
		SShadowCb m_shadowCbEntity;
		CConstantBuffer m_shadowCb;							//!<影を落とす時に使用する定数バッファ。
		EnSoftShadowQualityLevel m_softShadowLevel = EnSoftShadowQualityLevel::enNone;							//!<ソフトシャドウのレベル。
	};
}