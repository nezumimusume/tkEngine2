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
		private:
			SPointLight	light;
		};
	}
}