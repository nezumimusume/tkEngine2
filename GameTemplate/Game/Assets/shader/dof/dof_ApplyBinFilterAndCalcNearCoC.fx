/*!
 *@brief	Dof(被写界深度)
 *@details
 * シーンテクスチャにバイリニアフィルタをかけて、
 * カメラに近い側のCoC(錯乱円の半径)の計算を行います。
 * tkDof.cppのApplyBilinearFilterAndCalcNearCoc関数で使われているシェーダーです。
 *
 * 参考文献 Gpu gems3
 * 下記URLを参照
 * https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch28.html
 */


/*!
 *@brief	ピクセルシェーダーへの入力。
 */
struct PSIn{
	float4 position : SV_Position;
	float2 tcColor0 : TEXCOORD0;
	float2 tcColor1 : TEXCOORD1;
	float2 tcDepth0 : TEXCOORD2;
	float2 tcDepth1 : TEXCOORD3;
	float2 tcDepth2 : TEXCOORD4;
	float2 tcDepth3 : TEXCOORD5;
};

/*!
 *@brief	頂点シェーダー。
 */
PSIn VSMain( float4 pos : POSITION, float2 tc : TEXCOORD0 )
{
	PSIn psIn = (PSIn)0;
	
	return psIn;
}
/*!
 *@brief	ピクセルシェーダー。
 */
float4 PSMain( PSIn psIn ) : SV_Target0
{
	return float4( 0.0f, 0.0f, 0.0f, 1.0f);
}