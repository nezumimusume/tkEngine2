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
			
			void SetColor(const CVector4& color);
			
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