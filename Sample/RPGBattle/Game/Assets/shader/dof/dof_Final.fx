/*!
 *@brief	Dof(被写界深度)
 *@details
 * 最終合成。
 */

Texture2D<float4> cocTexture : register(t0);			//CoCが書き込まれたテクスチャ
Texture2D<float4> hexaBokeTexture : register(t1);		//六角形ブラーで生成されたボケテクスチャ。

sampler bilinearSampler : register(s0);			//バイリニアサンプリングを行うサンプラ。

/*!
 *@brief	ピクセルシェーダーへの入力。
 */
struct PSIn{
	float4 position : SV_Position;
	float2 uv		: TEXCOORD0;
};
/*!
 *@brief	定数バッファ。
 * この中身を変更したら、tkDof.hのCFinalCBも
 * 更新する。
 */
cbuffer cbParam : register( b0 )
{
	float bokeTextureLuminance;
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
	
	float4 bokeColor = hexaBokeTexture.Sample(bilinearSampler, psIn.uv) * 0.7f * bokeTextureLuminance;
	
	//CoCがそのままブレンディング率。
	float blendRate = min( 1.0f, coc ) ;
	
	float4 blendColor = lerp(color, bokeColor, blendRate);
	//適当にボケボケ
	return float4(blendColor.xyz, 1.0f);
}
