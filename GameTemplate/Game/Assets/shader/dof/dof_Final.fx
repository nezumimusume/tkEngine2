/*!
 *@brief	Dof(被写界深度)
 *@details
 * 最終合成。
 */

Texture2D<float4> cocTexture : register(t0);	//CoCが書き込まれたテクスチャ
Texture2D<float4> dofMasTexture : register(t1);	//被写界深度のマスクテクスチャ
Texture2D<float4> bokeTexture_0 : register(t2);	//ボケテクスチャ　レベル０。


sampler pointSampler : register(s0);			//ポイントサンプリングを行うサンプラ。
sampler bilinearSampler : register(s1);			//バイリニアサンプリングを行うサンプラ。

/*!
 *@brief	ピクセルシェーダーへの入力。
 */
struct PSIn{
	float4 position : SV_Position;
	float2 uv		: TEXCOORD0;
};

/*!
 *@brief	頂点シェーダー。
 */
PSIn VSMain( float4 pos : POSITION, float2 uv : TEXCOORD0 )
{
	PSIn psIn = (PSIn)0;
	psIn.position = pos;
	psIn.uv = uv;
	return psIn;
}
/*!
 *@brief	ピクセルシェーダー。
 */
float4 PSMain( PSIn psIn ) : SV_Target0
{

	//被写界深度をかけない箇所はピクセルキル～
	float mask = dofMasTexture.Sample( bilinearSampler, psIn.uv );
	clip( mask - 0.1f );
	//ピクセルのCoCを取得。
	float coc = cocTexture.Sample( bilinearSampler, psIn.uv).a;
	coc = min( 1.0f, coc ); 

	float4 blendColor = lerp(
		cocTexture.Sample(bilinearSampler, psIn.uv), 
		bokeTexture_0.Sample(bilinearSampler, psIn.uv), 
		coc
	);
	//適当にボケボケ
	return float4(blendColor.xyz, 1.0f);
}
