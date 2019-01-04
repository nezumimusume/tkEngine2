#include "tkEngine/tkEnginePrecompile.h"
#include "tkPresetSamplerState.h"


namespace tkEngine {

	CSamplerState* CPresetSamplerState::clamp_clamp_clamp_linear = nullptr;
	CSamplerState* CPresetSamplerState::clamp_clamp_clamp_point = nullptr;
	void CPresetSamplerState::Init()
	{
		{
			
			CD3D11_DEFAULT def;
			CD3D11_SAMPLER_DESC desc(def);
			
			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

			clamp_clamp_clamp_linear = new CSamplerState;
			clamp_clamp_clamp_linear->Create(desc);
		}
		{

			CD3D11_DEFAULT def;
			CD3D11_SAMPLER_DESC desc(def);

			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

			clamp_clamp_clamp_point = new CSamplerState;
			clamp_clamp_clamp_point->Create(desc);
		}
	}

}