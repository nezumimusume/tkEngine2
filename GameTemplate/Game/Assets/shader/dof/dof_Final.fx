/*!
 *@brief	Dof(被写界深度)
 *@details
 * 最終合成。
 */

Texture2D<float4> cocTexture : register(t0);	//CoCが書き込まれたテクスチャ
Texture2D<float4> dofMasTexture : register(t1);	//被写界深度のマスクテクスチャ
Texture2D<float4> bokeTexture_0 : register(t2);	//ボケテクスチャ　レベル０。
Texture2D<float4> bokeTexture_1 : register(t3);	//ボケテクスチャ　レベル１。

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

	//ピクセルのCoCを取得。
	float coc = cocTexture.Sample( bilinearSampler, psIn.uv).a;
	coc = min( 1.0f, coc );
	//錯乱円の半径が0.001以下はボケないので、ピクセルキル。
	clip( coc - 0.0001f );
	float4 colorTbl[3] = {
		cocTexture.Sample(bilinearSampler, psIn.uv),
		bokeTexture_0.Sample(bilinearSampler, psIn.uv),
		bokeTexture_1.Sample(bilinearSampler, psIn.uv),
	};
	
	float fTexIndex = min( 0.998f, coc ) / 0.5f;
	int iTexIndex = floor(fTexIndex);
	float blendRate = frac(fTexIndex);
	
	float4 blendColor = lerp(colorTbl[iTexIndex], colorTbl[iTexIndex+1], blendRate);
	//適当にボケボケ
	return float4(blendColor.xyz, 1.0f);
}
