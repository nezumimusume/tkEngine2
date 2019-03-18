/*!
 *@brief	六角形ブラー。
 */
 

#include "common.h"

struct VSInput{
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

/*!
 *@brief	定数バッファ。
 */
cbuffer cbParam : register( b0 ){
	float2 texSize : packoffset(c0);		//ブラーをかけるテクスチャのサイズ。
	float radius : packoffset(c0.z);		//ブラーをかける半径。
};
/*!
 *@brief	頂点シェーダー。
 */
PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}

//1パス目のピクセルシェーダーの出力。
struct PSOutput_1{
	float4 color_0 : SV_Target0;	//垂直ブラーの出力先
	float4 color_1 : SV_Target1;	//斜めブラーの出力先。
};

Texture2D<float4> originalTexture	: register(t0);		//ブラーをかける前のオリジナルテクスチャ。

/*!
 *@brief	垂直、斜めブラーのピクセルシェーダー。
 */
PSOutput_1 PSVerticalDiagonalBlur(PSInput pIn)
{
	//ブラーステップの長さ。8テクセル分にブラーをかける場合は、
	//blurStepLenは1.0になる。16テクセル分にブラーを×場合は、blurStepLenは2.0になる。
	//8テクセルフェッチしてブラーをかけるので、blurStepLenが2.0の場合は、2 × 8 で16テクセル分
	//ブラーがかかる。
	float blurStepLen = radius / 4.0f;
	
	float4 sceneColor = originalTexture.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv );
		
	float2 uvOffset = float2(0.0f, -1.0f / texSize.y );
	uvOffset *= blurStepLen;
	
	PSOutput_1 psOut = (PSOutput_1)0;
	
	psOut.color_0 += originalTexture.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset );
		
	psOut.color_0 += originalTexture.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset * 2 );
		
	psOut.color_0 += originalTexture.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset * 3 );
		
	psOut.color_0 += originalTexture.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset * 4 );
		
	psOut.color_0 /= 4.0f;	
	
	uvOffset.x = -0.86602f / texSize.x;
	uvOffset.y = 0.5f / texSize.y;
	uvOffset *= blurStepLen;
	
	psOut.color_1 = originalTexture.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset );
	
	psOut.color_1 += originalTexture.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset * 2 );
		
	psOut.color_1 += originalTexture.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset * 3 );
		
	psOut.color_1 += originalTexture.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset * 4 );
		
	psOut.color_1 += sceneColor;
	psOut.color_1 /= 5.0f;
	psOut.color_1 += psOut.color_0;
	return psOut;
}

Texture2D<float4> blurTexture_0	: register(t0);		//ブラーテクスチャ_0。1パス目で作成されたテクスチャ。
Texture2D<float4> blurTexture_1	: register(t1);		//ブラーテクスチャ_1。1パス目で作成されたテクスチャ。

float4 PSCombineVerticalDiagonalBlur(PSInput pIn) : SV_Target0
{
	float4 color = blurTexture_0.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv );
	float coc = color.a;
	
	color += blurTexture_1.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv );
	
	return float4( color.xyz, coc );
}
/*!
 *@brief	六角形作成ブラー
 */
float4 PSRhomboidBlur(PSInput pIn) : SV_Target0
{
	//ブラーステップの長さ。8テクセル分にブラーをかける場合は、
	//blurStepLenは1.0になる。16テクセル分にブラーを×場合は、blurStepLenは2.0になる。
	//8テクセルフェッチしてブラーをかけるので、blurStepLenが2.0の場合は、2 × 8 で16テクセル分
	//ブラーがかかる。
	float blurStepLen = radius / 4.0f;
		
	float2 uvOffset;
	uvOffset.x = -0.86602f / texSize.x;
	uvOffset.y = 0.5f / texSize.y;
	uvOffset *= blurStepLen;
	
	float4 color = blurTexture_0.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset );
		
	color += blurTexture_0.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset * 2 );
	
	color += blurTexture_0.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset * 3 );
	
	color += blurTexture_0.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset * 4 );
	

	uvOffset.x = 0.86602f / texSize.x * blurStepLen;

	color += blurTexture_1.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv );
		
	color += blurTexture_1.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset );
		
	color += blurTexture_1.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset * 2 );
	
	color += blurTexture_1.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset * 3 );
	
	color += blurTexture_1.Sample(
		clamp_clamp_clamp_linearSampler, pIn.uv + uvOffset * 4 );
	
	color /= 9.0f;
	
	return color;
}