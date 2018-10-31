/*!
 *@brief	自己発光ライト
 */

#pragma once

#include "tkEngine/graphics/tkLight.h"
#include "tkEngine/light/tkLightBase.h"

namespace tkEngine{
	namespace prefab {
		/*!
		 *@brief	自己発光ライト。
		 */
		class CEmissionLight : public CLightBase {
		public:
			CEmissionLight();
			~CEmissionLight();
			/*!
			 *@brief	自己発光色を設定する。
			 *@param[in]	color		自己発光色。
			 */
			void SetColor( const CVector4& color )
			{
			}
		private:
		};
	}
}