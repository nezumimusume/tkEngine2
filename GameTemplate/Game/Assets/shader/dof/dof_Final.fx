/*!
 *@brief	Dof(被写界深度)
 *@details
 * 最終合成。
 */

Texture2D<float4> cocTexture : register(t0);	//CoCが書き込まれたテクスチャ
Texture2D<float4> bokeTexture : register(t1);	//ボケテクスチャ。

sampler bilinearSampler : register(s0);			//バイリニアサンプリングを行うサンプラ。

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

	//ピクセルのCoCを取得。
	float4 color = cocTexture.Sample( bilinearSampler, psIn.uv);
	float coc = color.a;
	coc = min( 1.0f, coc );
	//錯乱円の半径が0.001以下はボケないので、ピクセルキル。
	clip( coc - 0.0001f );
	
	//CoCがそのままブレンディング率。
	float blendRate = min( 1.0f, coc ) ;
	
	float4 blendColor = lerp(color, bokeTexture.Sample(bilinearSampler, psIn.uv), blendRate);
	//適当にボケボケ
	return float4(blendColor.xyz, 1.0f);
}
