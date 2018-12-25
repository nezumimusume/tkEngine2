/*!
 *@brief	Dof(被写界深度)
 *@details
 * 最終合成。
 */

Texture2D<float4> cocTexture : register(t0);	//CoCが書き込まれたテクスチャ
Texture2D<float4> dofMasTexture : register(t1);	//被写界深度のマスクテクスチャ
Texture2D<float4> bokeTexture_0 : register(t2);	//ボケテクスチャ　レベル０。
Texture2D<float4> bokeTexture_1 : register(t3);	//ボケテクスチャ　レベル１。
Texture2D<float4> bokeTexture_2 : register(t4);	//ボケテクスチャ　レベル２。
Texture2D<float4> bokeTexture_3 : register(t5);	//ボケテクスチャ　レベル３。
Texture2D<float4> bokeTexture_4 : register(t6);	//ボケテクスチャ　レベル４。


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
	//cocの値からフェッチするテクスチャを決める。
	float fTexIndex = min( 0.998f, coc ) / 0.25f;
	int iTexIndex = floor(fTexIndex);
	float blendRate = frac(fTexIndex);
	float4 colorTbl[5] = {
		bokeTexture_0.Sample(bilinearSampler, psIn.uv),
		bokeTexture_1.Sample(bilinearSampler, psIn.uv),
		bokeTexture_2.Sample(bilinearSampler, psIn.uv),
		bokeTexture_3.Sample(bilinearSampler, psIn.uv),
		bokeTexture_4.Sample(bilinearSampler, psIn.uv),
	};

	float4 blendColor = lerp(colorTbl[iTexIndex], colorTbl[iTexIndex+1], blendRate);
	//適当にボケボケ
	return float4(blendColor.xyz, 1.0f);
}
