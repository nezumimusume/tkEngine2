/*!
 * @brief	ブラー。
 */

struct PS_BlurInput{
	float4 pos	: SV_POSITION;
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

/*!
 * @brief	ブラーピクセルシェーダー。
 */
float4 PSBlur( PS_BlurInput In ) : SV_Target0
{
	float4 Color;
	Color  = weight[0].x * (blurTexture.Sample( Sampler, In.tex0 )
	               + blurTexture.Sample( Sampler, In.tex7 + offset ));
	Color += weight[0].y * (blurTexture.Sample( Sampler, In.tex1 )
	               + blurTexture.Sample( Sampler, In.tex6 + offset ));
	Color += weight[0].z * (blurTexture.Sample( Sampler, In.tex2 )
	          + blurTexture.Sample( Sampler, In.tex5 + offset ));
	Color += weight[0].w * (blurTexture.Sample( Sampler, In.tex3 )
	               + blurTexture.Sample( Sampler, In.tex4 + offset ));
	Color += weight[1].x * (blurTexture.Sample( Sampler, In.tex4 )
	               + blurTexture.Sample( Sampler, In.tex3 + offset ));
	Color += weight[1].y * (blurTexture.Sample( Sampler, In.tex5 )
	               + blurTexture.Sample( Sampler, In.tex2 + offset ));
	Color += weight[1].z * (blurTexture.Sample( Sampler, In.tex6 )
	               + blurTexture.Sample( Sampler, In.tex1 + offset ));
	Color += weight[1].w * (blurTexture.Sample( Sampler, In.tex7 )
	                 + blurTexture.Sample( Sampler, In.tex0 + offset ));
	return Color;
}
