/*!
 *@brief	ã§í ÉwÉbÉ_ÅB
 */
 
#ifndef COMMON_H
#define COMMON_H

sampler clamp_clamp_clamp_linearSampler : register(s0);	//AddressU = CLAMP
														//AddressV = CLAMP
														//AddressW = CLAMP
														//D3D11_FILTER_MIN_MAG_MIP_LINEAR
														
sampler clamp_clamp_clamp_pointSampler : register(s1);	//AddressU = CLAMP
														//AddressV = CLAMP
														//AddressW = CLAMP
														//D3D11_FILTER_MIN_MAG_MIP_POINT

#endif //COMMON_H