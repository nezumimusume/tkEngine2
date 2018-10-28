/*!
 *@brief	ディレクションライト
 */

#pragma once

#include "tkEngine/graphics/tkLight.h"
#include "tkEngine/light/tkLightBase.h"

namespace tkEngine{
	namespace prefab {
		/*!
		 *@brief	ディレクションライト。
		 */
		class CDirectionLight : public CLightBase {
		public:
			CDirectionLight();
			~CDirectionLight();
			bool StartSub() override final;
			void Update() override final;
			/*!
			*@brief ライトの方向を設定。
			*@param[in]	direction	ライトの方向。
			*/
			void SetDirection(const CVector3& direction);
			/*!
			 *@brief	ディレクションライトを取得。
			 */
			const CVector3& GetDirection() const
			{
				return m_light.direction;
			}
			/*!
			*@brief ライトのカラーを設定。
			*@param[in]	color	ライトのカラー。
			*/
			void SetColor(const CVector4& color);
			/*!
			*@brief 生データの取得。
			*/
			SDirectionLight& GetRawData()
			{
				return m_light;
			}
			
		private:
			SDirectionLight		m_light;		//!<ライト。
			
		};
	}
}