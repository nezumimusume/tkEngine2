/*!
 *@brief	Dof(被写界深度)
 *@details
 * ボケテクスチャ作成の描画パスで使用されるシェーダー。
 */

Texture2D<float4> blurTexture : register(t0);	//ブラーをかけるテクスチャ。
sampler linearSampler : register(s0);			//バイリニアサンプリングを行うサンプラ。

void ComputeNewWeightAndColor( out float4 newColor, out float newWeight, inout float totalWeight,  float2 uv)
{
	//錯乱円の半径が小さいピクセルはブラーがかかるピクセルに侵入しないようにするために、
	//錯乱円の半径をガウスブラーのウェイトに乗算する。
	newColor = blurTexture.Sample( linearSampler, uv );
	newWeight = newColor.a;
	totalWeight += newWeight;
}

//////////////////////////////////////////////////////////////////////////////////////
// ミニブラー
//////////////////////////////////////////////////////////////////////////////////////

/*!
 *@brief	定数バッファ。
 */
cbuffer cbDownSampleParam : register( b0 )
{
	float2 invRenderTargetSize;		//レンダリングターゲットの逆数。
};
// 4点サンプリングブラー
/*!
 *@brief	ピクセルシェーダーへの入力。
 */
struct PSBlurIn{
	float4 position : SV_Position;
	float4 uv0_1	: TEXCOORD0;
	float4 uv2_3	: TEXCOORD1;
};

/*!
 *@brief	頂点シェーダー。
 */
PSBlurIn VSMinBlur( float4 pos : POSITION, float2 uv : TEXCOORD0 )
{
	PSBlurIn psIn = (PSBlurIn)0;
	psIn.position = pos;
	float offset =  0.25f;
	psIn.uv0_1.xy = uv + float2( -offset, -offset ) * invRenderTargetSize;
	psIn.uv0_1.zw = uv + float2(  offset, -offset ) * invRenderTargetSize;
	psIn.uv2_3.xy = uv + float2( -offset,  offset ) * invRenderTargetSize;
	psIn.uv2_3.zw = uv + float2(  offset,  offset ) * invRenderTargetSize;
	return psIn;
}
/*!
 *@brief	ピクセルシェーダー。
 */
float4 PSMinBlur( PSBlurIn psIn ) : SV_Target0
{
	float4 color[4];
	float weight[4];
	float totalWeight = 0;
	ComputeNewWeightAndColor(color[0], weight[0], totalWeight, psIn.uv0_1.xy);
	ComputeNewWeightAndColor(color[1], weight[1], totalWeight, psIn.uv0_1.zw);
	ComputeNewWeightAndColor(color[2], weight[2], totalWeight, psIn.uv2_3.xy);
	ComputeNewWeightAndColor(color[3], weight[3], totalWeight, psIn.uv2_3.zw);
	
	clip( totalWeight - 0.001f );	//ウェイトの合計が0.001以下ならピクセルキル。
	
	float4 finalColor = 0;
	//ウェイトを規格化(ウェイトの合計を1.0にする)を行う。
	for( int i = 0; i < 4; i++ ){
		weight[i] /= totalWeight;
		finalColor += color[i] * weight[i];
	}
	
	return finalColor;
}

//////////////////////////////////////////////////////////////////////////////////////
// 六角形ブラー
// ボケが発生しないピクセルがボケ画像に侵入しないようにしている。
//////////////////////////////////////////////////////////////////////////////////////

/*!
 *@brief	定数バッファ。
 */
cbuffer cbParam : register( b0 ){
	float2 texSize : packoffset(c0);		//ブラーをかけるテクスチャのサイズ。
	float radius : packoffset(c0.z);		//ブラーをかける半径。
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

//1パス目のピクセルシェーダーの出力。
struct PSOutput_1{
	float4 color_0 : SV_Target0;	//垂直ブラーの出力先
	float4 color_1 : SV_Target1;	//斜めブラーの出力先。
};


/*!
 *@brief	垂直、斜めブラーのピクセルシェーダー。
 */
PSOutput_1 PSVerticalDiagonalBlur(PSInput pIn)
{
	float totalWeight = 0.0f;
	float newWeight[5];
	float4 newColor[5];

	//ブラーステップの長さ。8テクセル分にブラーをかける場合は、
	//blurStepLenは1.0になる。16テクセル分にブラーを×場合は、blurStepLenは2.0になる。
	//8テクセルフェッチしてブラーをかけるので、blurStepLenが2.0の場合は、2 × 8 で16テクセル分
	//ブラーがかかる。
	float blurStepLen = radius / 4.0f;
	
	float2 uvOffset = float2(0.0f, -blurStepLen / texSize.y );
	PSOutput_1 psOut = (PSOutput_1)0;
	
	{
		//垂直ブラー
		for( int i = 0; i < 4; i++ ){
			ComputeNewWeightAndColor(newColor[i], newWeight[i], totalWeight, pIn.uv + uvOffset * (i + 1));
		}
		totalWeight = max( 0.001f, totalWeight);
		
		//ウェイトを規格化(ウェイトの合計を1.0にする)を行う。
		for( int i = 0; i < 4; i++ ){
			newWeight[i] /= totalWeight;
			psOut.color_0 += newColor[i] * newWeight[i];
		}
	}
	uvOffset.x = -0.86602f / texSize.x;
	uvOffset.y = 0.5f / texSize.y;
	uvOffset *= blurStepLen;
	{
		//斜めブラー。
		totalWeight = 0.0f;
		
		for( int i = 0; i < 5; i++ ){
			ComputeNewWeightAndColor(newColor[i], newWeight[i], totalWeight, pIn.uv + uvOffset * i);
		}
		totalWeight = max( 0.001f, totalWeight);
		
		//ウェイトを規格化(ウェイトの合計を1.0にする)を行う。
		for( int i = 0; i < 5; i++ ){
			newWeight[i] /= totalWeight;
			psOut.color_1 += newColor[i] * newWeight[i];
		}
	}
	psOut.color_1 += psOut.color_0;
	return psOut;
}

#if 0
//////////////////////////////////////////////////////////////////////////////////////
// ガウシアンブラー
// ボケが発生しないピクセルがボケ画像に侵入しないようにしている。
//////////////////////////////////////////////////////////////////////////////////////

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


struct VSInput{
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};


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
void ComputeNewWeightAndColor( out float4 newColor, out float newWeight, inout float totalWeight, float weightBase, float2 uv)
{
	//錯乱円の半径が小さいピクセルはブラーがかかるピクセルに侵入しないようにするために、
	//錯乱円の半径をガウスブラーのウェイトに乗算する。
	newColor = blurTexture.Sample( linearSampler, uv );
	newWeight = weightBase * newColor.a ;
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

#endif


