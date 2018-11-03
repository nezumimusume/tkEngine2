/*!
 * @brief	ディファードシェーディング。
 */
#include "modelCB.h"
#include "modelStruct.h"
#include "modelSRV.h"
#include "sampleBRDF.h"
#include "modelPSFunction.h"
#include "CommonFunc.h"

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
	return CalcWorldPosFromUVZ(
		uv, 
		depthMap.Sample(Sampler, uv).r, 
		mViewProjInv
	);
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
	float3 worldPos = CalcWorldPosFromUVZ(
		In.uv,
		depthMap.Sample(Sampler, In.uv).r,
		mViewProjInv
	);
	//スペキュラ。
	float4 spec = specularMap.Sample(Sampler, In.uv);
	
	float shadow = softShadowMap.Sample(Sampler, In.uv).r;
	
	uint uMatID = floor(materialIDMap.Sample(Sampler, In.uv).r + 0.001f);
	
	//物理ベースライティング。
	float4 finalColor = PBR(
		albedo, 
		tangent, 
		normal, 
		biNormal, 
		worldPos, 
		spec,
		shadow,
		In.uv,
		uMatID
	);
	
	//自己発光を加算。
	finalColor.xyz +=emissionColorTexture.Sample( Sampler, In.uv).xyz;
	return finalColor;
}
