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
	float4x4 mViewProjInv;				//ビュープロジェクション行列の逆行列。
	float4x4 mViewProj;					//ビュープロジェクション行列。
	float4x4 mViewProjInvLastFrame;		//1フレーム前のビュープロジェクション行列。
	float4 cameraPosInWorld;	//ワールド座標系でのカメラの視点。
	float4 renderTargetSize;	//レンダリングターゲットのサイズ。PSFinalでしか使用できません。
								//ほかの箇所で使いたいなら、cpp側の対応が必要です。
	float rayMarchStepRate;		//レイマーチのステップレート。。
};

Texture2D<float4> sceneTexture : register(t0);	//シーンテクスチャ。
Texture2D<float4> normalTexture : register(t1);	//法線テクスチャ。
Texture2D<float4> depthTexture : register(t2);	//深度テクスチャ。
Texture2D<float4> lastFrameDepthTexture : register(t3);	//1フレーム前の深度テクスチャ。

sampler Sampler : register(s0);



PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}
float GetRandomNumber(float2 texCoord, float Seed)
{
	return frac(sin(dot(texCoord.xy, float2(12.9898, 78.233)) + Seed) * 43758.5453);
}
float4 PSMain( PSInput In ) : SV_Target0
{
	//ピクセルのワールド座標を計算する。
	float3 worldPos = CalcWorldPosFromUVZ(
		In.uv,
		depthTexture.Sample(Sampler, In.uv).r,
		mViewProjInv
	);
	//1フレーム前のピクセルのワールド座標を計算する。
	float3 worldPosLastFrame = CalcWorldPosFromUVZ(
		In.uv,
		lastFrameDepthTexture.Sample(Sampler, In.uv).r,
		mViewProjInvLastFrame
	);
	float len = length(worldPos - worldPosLastFrame);
	float alpha = lerp(0.1f, 0.5f, min(1.0f, len / 1.0f));
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
	int maxRayNum = 20;
	float3 raystep = 100.0f/maxRayNum * toPixelDirReflect;
	float maxThickness = 0.01f / maxRayNum;
	for( int step = 1; step < maxRayNum; step++){ //自分自身とぶつかるので、stepは1から進める。
		float3 rayPos = worldPos + step * raystep * lerp( 0.8f, 1.0f, GetRandomNumber(In.uv, rayMarchStepRate));
		float4 posInProj = float4	(rayPos, 1.0f);
		//ビュープロジェクション空間に変換する。
		posInProj = mul(mViewProj, posInProj);
		posInProj.xyz /= posInProj.w;
		posInProj.xy = posInProj.xy * float2(0.5f, -0.5f) + 0.5f;
		float pixelZ = depthTexture.Sample(Sampler, posInProj.xy).r;
		if (posInProj.z - pixelZ > 0 && posInProj.z - pixelZ < maxThickness) {
			//ぶつかったのでレイマーチングを小刻みに行って精度を上げる。。
			float sign = -1.0;
			/*for (int m = 1; m <= 4; ++m) {
				rayPos += raystep * (sign * pow(0.5, m));
				posInProj = mul(mViewProj, float4(rayPos, 1.0f));
				posInProj.xyz /= posInProj.w;
				posInProj.xy = posInProj.xy * float2(0.5f, -0.5f) + 0.5f;
				float pixelZ = depthTexture.Sample(Sampler, posInProj.xy).r;
				sign = posInProj.z - pixelZ > 0 ? -1 : 1;
			}*/
			float4 reflectColor = sceneTexture.Sample(Sampler, posInProj.xy);
			return float4(reflectColor.xyz, alpha);
		}
	}
	return float4(sceneColor.xyz, alpha);
}

Texture2D<float4> reflectTexture : register(t1);		//反射テクスチャ。
Texture2D<float4> specTexture : register(t2);	//スペキュッラ


//最終合成。
float4 PSFinal(PSInput In) : SV_Target0
{
	//反射カラーの平均をとる。
	float4 reflectColor = reflectTexture.Sample(Sampler, In.uv);
	float4 sceneColor = sceneTexture.Sample(Sampler, In.uv);
	//映り込みあり。
	float spec = min( 1.0f, specTexture.Sample(Sampler, In.uv).r );
	//spec *= all(max(0.0f, length(reflectColor.xyz)-0.1f));
	sceneColor.xyz = lerp(sceneColor.xyz, reflectColor.xyz, spec);
	return float4(sceneColor.xyz, 1.0f);

}
