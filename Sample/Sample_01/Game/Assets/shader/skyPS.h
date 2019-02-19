/*!
 *@brief	空用のピクセルシェーダー。
 * model.fxでインクルードされています。
 */
TextureCube<float4> skyCubeMap : register(t0);	//スカイキューブマップ。

/*!
 *@brief	空用のシェーダー。
 */
float4 PSMain_SkyCube( PSInput In ) : SV_Target0
{
	float4 color = skyCubeMap.Sample( Sampler, In.Normal);
	color.xyz += emissionColor;
	return color ;
}