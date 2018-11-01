/*!
 * @brief	ライトの基底クラス。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/light/tkLightBase.h"
#include "tkEngine/light/tkLightManager.h"
namespace tkEngine{
	namespace prefab {
		bool CLightBase::Start()
		{
			LightManager().AddLight(this);
			return StartSub();
		}
		void CLightBase::OnDestroy()
		{
			LightManager().RemoveLight(this);
			return OnDestorySub();
		}
	}
}