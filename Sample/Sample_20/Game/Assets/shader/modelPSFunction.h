/*!
 * @brief	モデル用のピクセルシェーダー。
 */

#include "LightingFunction.h"

/*!
 * @brief	物理ベースレンダリング。
 */
float4 PBR( 
	float4 albedo,
	float3 tangent,
	float3 normal,
	float3 biNormal,
	float3 worldPos,
	float4 spec,
	float shadow,
	float2 screenPos,
	uint uMatID
) 
{

	float3 lig = 0.0f;
	//視点までのベクトルを求める。
	float3 toEye = normalize(eyePos - worldPos);

	//スペキュラ。
	float specPow = 0.0f;
	float roughness = 1.0f;

	specPow = spec.x;
	roughness = 1.0f - spec.y;
	
	float toEyeLen = length(toEye);
	float3 toEyeDir = float3(1.0f, 0.0f, 0.0f);
	if (toEyeLen > 0.001f) {
		toEyeDir = toEye / toEyeLen;
	}

	float3 toEyeReflection = -toEyeDir + 2.0f * dot(normal, toEyeDir) * normal;

	//ディレクションライト
	float3 finalColor = 0.0f;

	//影が落ちる可能性が低い場合のみ計算する。
	finalColor = CalcDirectionLight(
		albedo,
		worldPos,
		normal,
		tangent,
		biNormal,
		toEyeDir,
		toEyeReflection,
		roughness,
		specPow,
		uMatID,
		shadow
	) ;

	//ポイントライトを計算。
	finalColor += CalcPointLightInner(
		albedo,
		worldPos,
		screenPos,
		normal,
		tangent,
		biNormal,
		toEyeDir,
		toEyeReflection,
		roughness,
		specPow
	);

	//アンビエントライト。
	finalColor += CalcAmbientLight(
		albedo,
		normal,
		tangent,
		biNormal,
		toEyeDir,
		roughness,
		specPow
	);

	// brightness
	float brightness = 1.0f;
	finalColor *= brightness;
	/*
	// exposure
	float exposure = 1.0f;
	finalColor *= pow( 2.0, exposure );
	*/
	float gamma = 2.2f;
	finalColor = max(0.0f, pow(finalColor, 1.0 / gamma));
	if (isnan(finalColor.x) || isnan(finalColor.y) || isnan(finalColor.z)) {
		return float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	return float4(finalColor, 1.0f);
}