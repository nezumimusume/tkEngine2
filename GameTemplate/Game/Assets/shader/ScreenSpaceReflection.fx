/*!
 * @brief	SSR
 */
#include "CommonFunc.h"

struct VSInput{
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

//カメラパラメータの定数バッファ
cbuffer cb : register(b0){
	float4x4 mViewProjInv;		//ビュープロジェクション行列の逆行列。
	float4x4 mViewProj;			//ビュープロジェクション行列。
	float4 cameraPosInWorld;	//ワールド座標系でのカメラの視点。
};

Texture2D<float4> sceneTexture : register(t0);	//シーンテクスチャ。
Texture2D<float4> normalTexture : register(t1);	//法線テクスチャ。
Texture2D<float4> depthTexture : register(t2);	//深度テクスチャ。
Texture2D<float4> specTexture : register(t3);	//すぺきゅらテクスチャ。

sampler Sampler : register(s0);


float GetRandomNumber(float2 texCoord, int Seed)
{
	return frac(sin(dot(texCoord.xy, float2(12.9898, 78.233)) + Seed) * 43758.5453);
}


PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}
float4 PSMain( PSInput In ) : SV_Target0
{
	//ピクセルのワールド座標を計算する。
	float3 worldPos = CalcWorldPosFromUVZ(
		In.uv,
		depthTexture.Sample(Sampler, In.uv).r,
		mViewProjInv
	);
	float4 sceneColor = sceneTexture.Sample(Sampler, In.uv);
	//ピクセルの法線を取得。
	float3 normal = normalTexture.Sample(Sampler, In.uv).xyz;
	//カメラの視点からピクセルに向かうベクトルを計算する。
	float3 toPixelDir = normalize( worldPos - cameraPosInWorld );
	//反射ベクトルを求める。
	float3 toPixelDirReflect = reflect( toPixelDir, normal );
	//反射ベクトルを使って、レイマーチングを行う。
	//レイマーチングのイテレーション回数と線分を伸ばす距離は
	//後で定数バッファにするんやで？
	float spec = specTexture.Sample(Sampler, In.uv);
	for( int step = 0; step < 50; step++){
		worldPos += toPixelDirReflect * 2.0f * GetRandomNumber( In.uv, 1 );
		float4 posInProj = float4(worldPos, 1.0f);
		//ビュープロジェクション空間に変換する。
		posInProj = mul(mViewProj, posInProj);
		posInProj.xyz /= posInProj.w;
		posInProj.xy = posInProj.xy * float2(0.5f, -0.5f) + 0.5f;
		float pixelZ = depthTexture.Sample(Sampler, posInProj.xy).r;
		if (posInProj.z - pixelZ > 0 && posInProj.z - pixelZ < 0.001f ) {
			//ぶつかったのでレイマーチング終了。
			float4 reflectColor = sceneTexture.Sample(Sampler, posInProj.xy);
			float reflectRate = spec * 0.5f;
			sceneColor = lerp(sceneColor, reflectColor, reflectRate);
			break;
		}
	}
	return float4(sceneColor.xyz, 1.0f);//sceneTexture.Sample(Sampler, In.uv);
}