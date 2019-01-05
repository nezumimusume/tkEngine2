/*!
 *@brief	Dof(被写界深度)
 *@details
 * シーン画像とCoCデータのダウンサンプリング
 */

Texture2D<float4> cocTexture : register(t0);	//シーンテクスチャ。
sampler linearSampler : register(s0);			//バイリニアサンプリングを行うサンプラ。

/*!
 *@brief	ピクセルシェーダーへの入力。
 */
struct PSDownSampleIn{
	float4 position : SV_Position;
	float2 uv : TEXCOORD0;
};

/*!
 *@brief	定数バッファ。
 */
cbuffer cbDownSampleParam : register( b0 )
{
	float2 invRenderTargetSize;		//レンダリングターゲットの逆数。
};
/*!
 *@brief	頂点シェーダー。
 */
PSDownSampleIn VSDownSample( float4 pos : POSITION, float2 uv : TEXCOORD0 )
{
	PSDownSampleIn psIn = (PSDownSampleIn)0;
	psIn.position = pos;
	psIn.uv = uv;
	return psIn;
}
/*!
 *@brief	ピクセルシェーダー。
 */
float4 PSDownSample( PSDownSampleIn psIn ) : SV_Target0
{
	return cocTexture.Sample( linearSampler, psIn.uv );
}

/*!
 *@brief	ピクセルシェーダーへの入力。
 */
struct PSBlurIn{
	float4 position : SV_Position;
	float4 uv0_1	: TEXCOORD0;
	float4 uv2_3	: TEXCOORD1;
};

/*!
 *@brief	頂点シェーダー。
 */
PSBlurIn VSBlur( float4 pos : POSITION, float2 uv : TEXCOORD0 )
{
	PSBlurIn psIn = (PSBlurIn)0;
	psIn.position = pos;
	float offset =  0.25f;
	psIn.uv0_1.xy = uv + float2( -offset, -offset ) * invRenderTargetSize;
	psIn.uv0_1.zw = uv + float2(  offset, -offset ) * invRenderTargetSize;
	psIn.uv2_3.xy = uv + float2( -offset,  offset ) * invRenderTargetSize;
	psIn.uv2_3.zw = uv + float2(  offset,  offset ) * invRenderTargetSize;
	return psIn;
}
/*!
 *@brief	ピクセルシェーダー。
 */
float4 PSBlur( PSBlurIn psIn ) : SV_Target0
{
	float4 color = cocTexture.Sample( linearSampler, psIn.uv0_1.xy );
	color += cocTexture.Sample( linearSampler, psIn.uv0_1.zw );
	color += cocTexture.Sample( linearSampler, psIn.uv2_3.xy );
	color += cocTexture.Sample( linearSampler, psIn.uv2_3.zw );
	color /= 4.0f;
	return color;
}


