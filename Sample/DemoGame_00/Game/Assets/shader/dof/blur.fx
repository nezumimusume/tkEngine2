/*!
 * @brief	ブラー。
 */

struct PS_BlurInput{
	float4 pos	: SV_POSITION;
	float2 originTex : NORMAL;
	float2 tex0	: TEXCOORD0;
	float2 tex1 : TEXCOORD1;
	float2 tex2 : TEXCOORD2;
	float2 tex3 : TEXCOORD3;
	float2 tex4 : TEXCOORD4;
	float2 tex5 : TEXCOORD5;
	float2 tex6 : TEXCOORD6;
	float2 tex7 : TEXCOORD7;
};

Texture2D<float4> blurTexture : register(t0);	//ブラーテクスチャ。

struct VSInput{
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};
struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};
sampler Sampler : register(s0);

/*!
 * @brief	ブラー用の定数バッファ。
 */
cbuffer CBBlur : register(b0)
{
	float4 offset;		//!<オフセット。
	float4 weight[2];	//!<重み。
	float2 uvOffset;
	
};
/*!
 * @brief	Xブラー頂点シェーダー。
 */
PS_BlurInput VSXBlur(VSInput In)
{
	float2 texSize;
	float level;
	blurTexture.GetDimensions( 0, texSize.x, texSize.y, level );
	PS_BlurInput Out;
	Out.pos = In.pos;
	float2 tex = In.uv;
	tex += uvOffset;
	Out.originTex = tex;
	Out.tex0 = tex + float2( - 1.0f/texSize.x, 0.0f );
    Out.tex1 = tex + float2( - 3.0f/texSize.x, 0.0f );
    Out.tex2 = tex + float2( - 5.0f/texSize.x, 0.0f );
    Out.tex3 = tex + float2( - 7.0f/texSize.x, 0.0f );
    Out.tex4 = tex + float2( - 9.0f/texSize.x, 0.0f );
    Out.tex5 = tex + float2( -11.0f/texSize.x, 0.0f );
    Out.tex6 = tex + float2( -13.0f/texSize.x, 0.0f );
    Out.tex7 = tex + float2( -15.0f/texSize.x, 0.0f );
    return Out;
}

/*!
 * @brief	Yブラー頂点シェーダー。
 */
PS_BlurInput VSYBlur(VSInput In)
{
	float2 texSize;
	float level;
	blurTexture.GetDimensions( 0, texSize.x, texSize.y, level );
	PS_BlurInput Out;
	Out.pos = In.pos;
	float2 tex = In.uv;
	tex += uvOffset;
	Out.originTex = tex;
	
	Out.tex0 = tex + float2( 0.0f,- 1.0f/texSize.y  );
    Out.tex1 = tex + float2( 0.0f,- 3.0f/texSize.y  );
    Out.tex2 = tex + float2( 0.0f,- 5.0f/texSize.y  );
    Out.tex3 = tex + float2( 0.0f,- 7.0f/texSize.y  );
    Out.tex4 = tex + float2( 0.0f,- 9.0f/texSize.y  );
    Out.tex5 = tex + float2( 0.0f,-11.0f/texSize.y  );
    Out.tex6 = tex + float2( 0.0f,-13.0f/texSize.y  );
    Out.tex7 = tex + float2( 0.0f,-15.0f/texSize.y  );
    return Out;
}
void ComputeNewWeightAndColor( out float4 newColor, out float newWeight, inout float totalWeight, float weightBase, float2 uv )
{
	//錯乱円の半径が小さいピクセルはブラーがかかるピクセルに侵入しないようにするために、
	//錯乱円の半径をガウスブラーのウェイトに乗算する。
	newColor = blurTexture.Sample( Sampler, uv );
	newWeight = weightBase * newColor.a;
	totalWeight += newWeight;
}
/*!
 * @brief	ブラーピクセルシェーダー。
 */
float4 PSBlur( PS_BlurInput In ) : SV_Target0
{
	float totalWeight = 0.0f;
	float newWeight[16];
	float4 newColor[16];
	ComputeNewWeightAndColor(newColor[ 0], newWeight[ 0], totalWeight, weight[0].x, In.tex0);
	ComputeNewWeightAndColor(newColor[ 1], newWeight[ 1], totalWeight, weight[0].y, In.tex1);
	ComputeNewWeightAndColor(newColor[ 2], newWeight[ 2], totalWeight, weight[0].z, In.tex2);
	ComputeNewWeightAndColor(newColor[ 3], newWeight[ 3], totalWeight, weight[0].w, In.tex3);
	ComputeNewWeightAndColor(newColor[ 4], newWeight[ 4], totalWeight, weight[1].x, In.tex4);
	ComputeNewWeightAndColor(newColor[ 5], newWeight[ 5], totalWeight, weight[1].y, In.tex5);
	ComputeNewWeightAndColor(newColor[ 6], newWeight[ 6], totalWeight, weight[1].z, In.tex6);
	ComputeNewWeightAndColor(newColor[ 7], newWeight[ 7], totalWeight, weight[1].w, In.tex7);
	ComputeNewWeightAndColor(newColor[ 8], newWeight[ 8], totalWeight, weight[0].x, In.tex7 + offset);
	ComputeNewWeightAndColor(newColor[ 9], newWeight[ 9], totalWeight, weight[0].y, In.tex6 + offset);
	ComputeNewWeightAndColor(newColor[10], newWeight[10], totalWeight, weight[0].z, In.tex5 + offset);
	ComputeNewWeightAndColor(newColor[11], newWeight[11], totalWeight, weight[0].w, In.tex4 + offset);
	ComputeNewWeightAndColor(newColor[12], newWeight[12], totalWeight, weight[1].x, In.tex3 + offset);
	ComputeNewWeightAndColor(newColor[13], newWeight[13], totalWeight, weight[1].y, In.tex2 + offset);
	ComputeNewWeightAndColor(newColor[14], newWeight[14], totalWeight, weight[1].z, In.tex1 + offset);
	ComputeNewWeightAndColor(newColor[15], newWeight[15], totalWeight, weight[1].w, In.tex0 + offset);
	
	//トータルウェイトが0.0001以下ならブラーを×すべてのピクセルがボケが発生しない
	//ピクセルなのでピクセルキル～。
	clip( totalWeight - 0.0001f);
	
	//ウェイトが再計算されているので、もっかい規格化(ウェイトの合計を1.0にする)を行う。
	for( int i = 0; i < 16; i++ ){
		newWeight[i] /= totalWeight;
	}
	
	float4 Color = 0;
	for( int i = 0; i < 16; i++ ){
		Color += newColor[i] * newWeight[i];
	}
	
	return Color;
}
