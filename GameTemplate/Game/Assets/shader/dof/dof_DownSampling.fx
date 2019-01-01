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
	psIn.uv = uv + 0.5f * invRenderTargetSize;
	return psIn;
}
/*!
 *@brief	ピクセルシェーダー。
 */
float4 PSDownSample( PSDownSampleIn psIn ) : SV_Target0
{
	return cocTexture.Sample( linearSampler, psIn.uv );
}

