/*!
 * @brief	コピー
 */

struct VSInput{
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> sceneTexture : register(t0);	//シーンテクスチャ。
sampler Sampler : register(s0);

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}
float4 PSMain( PSInput In ) : SV_Target0
{
	float2 texSize;
	float level;
	sceneTexture.GetDimensions( 0, texSize.x, texSize.y, level );
	float2 uv = In.uv + float2( -0.5/texSize.x, -0.5/texSize.y);
	return sceneTexture.Sample(Sampler, uv);
}