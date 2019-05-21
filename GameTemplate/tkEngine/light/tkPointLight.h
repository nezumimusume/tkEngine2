/*!
 *@brief	ポイントライト
 */

#pragma once

#include "tkEngine/graphics/tkLight.h"
#include "tkEngine/light/tkLightBase.h"

namespace tkEngine{
	namespace prefab {
		/*!
		 *@brief	ポイントライト。
		 */
		class CPointLight : public CLightBase {
		public:
			CPointLight();
			~CPointLight();
			bool StartSub() override final;
			void Update() override final;
			const CVector3& GetPosition()
			{
				return light.position;
			}
			void SetPosition(const CVector3& pos);
			
			void SetColor(const CVector3& color);
			/*!
			*@brief	減衰パラメータを設定。
			*@param[in] attn	減衰定数。xはポイントライトの影響が届く範囲。yはポイントライトの減衰率に影響を与えます。</br>
			*					yが大きくなると、減衰が強くなります。1.0で線形の減衰率になります。zは未使用。
			*/
			void SetAttn(const CVector3& attn);
			
			/*!
			*@brief 生データの取得。
			*/
			SPointLight& GetRawData()
			{
				return light;
			}
			/// <summary>
			/// このライトをスポットライトにする。
			/// </summary>
			void EnableSpotLight()
			{
				light.hasDirection = 1;
			}
			/// <summary>
			/// スポットライトを無効にする。
			/// </summary>
			void DisableSpotLight()
			{
				light.hasDirection = 0;
			}
			/// <summary>
			/// スポットライトの照射方向。
			/// </summary>
			/// <param name="dir">方向</param>
			void SetSpotLightDirection(const CVector3& dir)
			{
				light.directionAndAngle.x = dir.x;
				light.directionAndAngle.y = dir.y;
				light.directionAndAngle.z = dir.z;
			}
			/// <summary>
			/// スポットライトの照射角度。
			/// </summary>
			/// <param name="angle">照射角度(単位：ラジアン)</param>
			void SetSpotLightAngle( float angle )
			{
				light.directionAndAngle.w = angle;
			}
		private:
			SPointLight	light;
		};

		using CSpotLight = CPointLight;
	}
}