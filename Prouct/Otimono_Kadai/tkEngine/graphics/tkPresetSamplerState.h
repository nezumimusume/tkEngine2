/*!
* @brief	良く使うサンプラステートをまとめたもの。
*/
#pragma once

namespace tkEngine {
	class CPresetSamplerState
	{
	public:
		static void Init();
		static CSamplerState* sampler_clamp_clamp_clamp_linear;	//デフォルトのサンプラステート
																//AddressU = CLAMP
																//AddressV = CLAMP
																//AddressW = CLAMP
																//Filter = LINEAR
	};
}
