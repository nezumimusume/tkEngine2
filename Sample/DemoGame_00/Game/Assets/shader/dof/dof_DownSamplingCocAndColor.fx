/*!
 *@brief	Dof(被写界深度)
 *@details
 * シーン画像とCoCデータのダウンサンプリング
 */

Texture2D<float4> sceneAndCocTexture : register(t0);	//シーンテクスチャ。
sampler pointSampler : register(s0);			//ポイントサンプリングを行うサンプラ。
sampler linearSampler : register(s1);			//バイリニアサンプリングを行うサンプラ。

/*!
 *@brief	ピクセルシェーダーへの入力。
 */
struct PSIn{
	float4 position : SV_Position;
	float2 uv0		: TEXCOORD0;
	float2 uv1		: TEXCOORD1;
	float2 uv2		: TEXCOORD2;
	float2 uv3		: TEXCOORD3;
};

/*!
 *@brief	定数バッファ。
 * この中身を変更したら、tkDof.hのSDownSamplingCocAndColorCBも
 * 更新する。
 */
cbuffer cbParam : register( b0 )
{
	float2 invRenderTargetSize;		//レンダリングターゲットの逆数。
};
/*!
 *@brief	頂点シェーダー。
 */
PSIn VSMain( float4 pos : POSITION, float2 uv : TEXCOORD0 )
{
	PSIn psIn = (PSIn)0;
	psIn.position = pos;
	psIn.uv0 = uv + float2( -1.0f * invRenderTargetSize.x, 0.0f );
	psIn.uv1 = uv + float2(  1.0f * invRenderTargetSize.x, 0.0f );
	psIn.uv2 = uv + float2(                          0.0f,  1.0f * invRenderTargetSize.y );
	psIn.uv3 = uv + float2(                          0.0f, -1.0f * invRenderTargetSize.y );
	return psIn;
}
/*!
 *@brief	ピクセルシェーダー。
 */
float4 PSMain( PSIn psIn ) : SV_Target0
{
	float4 color = sceneAndCocTexture.Sample( pointSampler, psIn.uv0 );
	color += sceneAndCocTexture.Sample( pointSampler, psIn.uv1 );
	color += sceneAndCocTexture.Sample( pointSampler, psIn.uv2 );
	color += sceneAndCocTexture.Sample( pointSampler, psIn.uv3 );
	color /= 4.0f;
	return color;
}
