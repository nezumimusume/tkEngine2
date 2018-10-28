#include "tkEngine/tkEnginePrecompile.h"
#include "tkPresetSamplerState.h"


namespace tkEngine {

	CSamplerState* CPresetSamplerState::sampler_clamp_clamp_clamp_linear = nullptr;
	void CPresetSamplerState::Init()
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		sampler_clamp_clamp_clamp_linear = new CSamplerState;
		sampler_clamp_clamp_clamp_linear->Create(desc);
	}

}