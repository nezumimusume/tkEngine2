/*!
 * @brief	ディファードシェーディング。
 */
#include "modelCB.h"
#include "modelStruct.h"
#include "modelSRV.h"
#include "sampleBRDF.h"

#include "LightingFunction.h"

cbuffer PSDefferdCb : register( b0 )
{
	float4x4 mViewProjInv;	//ビュープロジェクション行列の逆行列。
};

struct VSDefferdInput{
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

struct PSDefferdInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

PSDefferdInput VSMain( VSDefferdInput In ) 
{
	PSDefferdInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}

/*!
 * @brief	UV座標からワールド座標を計算する。
 */
float3 CalcWorldPosFromUV( float2 uv )
{
	float3 screenPos;
	screenPos.xy = (uv * float2(2.0f, -2.0f)) + float2( -1.0f, 1.0f);
	screenPos.z = depthMap.Sample(Sampler, uv).r;
	
	float4 worldPos = mul(mViewProjInv, float4(screenPos, 1.0f));
	worldPos.xyz /= worldPos.w;
	return worldPos.xyz;
}
float4 PSMain( PSDefferdInput In ) : SV_Target0
{
	//アルベド。
	float4 albedo = albedoTexture.Sample(Sampler, In.uv);

	//接ベクトルを取得。。
	float3 tangent = tangentMap.Sample(Sampler, In.uv);
	//接ベクトル空間の法線を取得。
	float3 normal = normalMap.Sample(Sampler, In.uv);
	//従法線を計算。(todo 後で治す？	)
	float3 biNormal = normalize(cross(tangent, normal));

	//ワールド座標を計算する。
	float3 worldPos = CalcWorldPosFromUV( In.uv );

	float3 lig = 0.0f;
	//視点までのベクトルを求める。
	float3 toEye = normalize(eyePos - worldPos);

	//スペキュラ。
	float specPow = 0.0f;
	float roughness = 1.0f;
	float4 t = specularMap.Sample(Sampler, In.uv);
	specPow = t.x;
	roughness = 1.0f - t.y;
	roughness *= 0.8f;	//@todo マテリアルパラメータにすべきだな。
	
	float toEyeLen = length(toEye);
	float3 toEyeDir = float3(1.0f, 0.0f, 0.0f);
	if (toEyeLen > 0.001f) {
		toEyeDir = toEye / toEyeLen;
	}

	float3 toEyeReflection = -toEyeDir + 2.0f * dot(normal, toEyeDir) * normal;

	//影マップを参照
	float shadow = softShadowMap.Sample(Sampler, In.uv).r;
	//ディレクションライト
	float3 finalColor = 0.0f;

	if (shadow < 0.99f) {
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
			specPow
		) * (1.0f - shadow);
	}

	//ポイントライトを計算。
	finalColor += CalcPointLightInner(
		albedo,
		worldPos,
		In.uv,
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
