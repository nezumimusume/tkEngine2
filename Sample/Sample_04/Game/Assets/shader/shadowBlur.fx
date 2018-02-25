/*!
 * @brief	影のブラー処理。
 */

/*!
 * @brief	ブラー。
 */

struct PS_BlurInput{
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD0;
};

Texture2D<float4> blurTexture : register(t0);	//ブラーテクスチャ。
Texture2D<float4> depthTexture : register(t1);	//深度テクスチャ。



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
	float4 weight;				//!<重み。
	float4x4	mtxProj;		// 投影行列
	float4x4 	mtxProjInv;		// 投影行列の逆行列。
	float offsetTexelWorld;		// ブラーの時にフェッチするオフセット座標。ワールド空間の量。値が大きいほど大きくボケる。
};
/*!
 * @brief	Xブラー頂点シェーダー。
 */
PS_BlurInput VSBlur(VSInput In)
{
	PS_BlurInput Out;
	Out.pos = In.pos;
	float2 tex = In.uv;
	Out.uv = In.uv;
    return Out;
}

float4 Blur(float2 uv, float2 offset[4])
{
	float4 Color  = weight.x * (blurTexture.Sample( Sampler, uv + offset[0] )
	               + blurTexture.Sample( Sampler, uv - offset[0] ));
	               
	Color += weight.y * (blurTexture.Sample( Sampler, uv + offset[1] )
	               + blurTexture.Sample( Sampler, uv - offset[1] ));
	               
	Color += weight.z * (blurTexture.Sample( Sampler, uv + offset[2] )
	          + blurTexture.Sample( Sampler, uv - offset[2] ));
	          
	Color += weight.w * (blurTexture.Sample( Sampler, uv + offset[3] )
	               + blurTexture.Sample( Sampler, uv - offset[3] ));
	               
	return Color;
}
/*!
 * @brief	UV座標をカメラ座標系に変換する。
 */
float4 CalcUVToPosInCamera( float2 uv)
{
	float depth = depthTexture.Sample(Sampler, uv).r;
	float4 posInCamera = float4(uv, depth, 1.0f);
	//xyを射影空間に変化する。
	posInCamera.xy = posInCamera.xy * float2( 2.0f, -2.0f ) - float2( 1.0f, -1.0f);
	posInCamera = mul(mtxProjInv, posInCamera);
	posInCamera /= posInCamera.w;
	return posInCamera;
}

/*!
 * @brief	Xブラーピクセルシェーダー。
 */
float4 PSXBlur( PS_BlurInput In ) : SV_Target0
{
	float4 Color;
	
	//カメラ空間での近傍ピクセルを求める。
	float4 posInCamera = CalcUVToPosInCamera(In.uv);
	
	//どれくらい離れているところを調べるかは後で定数バッファで送るようにするんやで。
	//X軸方向に調べる。
	float2 offset[4];
	for(int i = 0; i < 4; i++){
		posInCamera.x += offsetTexelWorld;
		float4 pos = posInCamera;
		pos = mul(mtxProj, pos);
		offset[i] = (pos.xy / pos.w) * float2(0.5f, -0.5f) + 0.5f;
		float4 posInCamera2 = CalcUVToPosInCamera(offset[i]);
		
		if(abs(posInCamera.z - posInCamera2.z) < 5.0f){
			offset[i] -= In.uv ;
		}else{
			offset[i] = 0.0f;
		}
	}
	return Blur(In.uv, offset);
}

/*!
 * @brief	Yブラーピクセルシェーダー。
 */
float4 PSYBlur( PS_BlurInput In ) : SV_Target0
{
	float4 Color;
	
	//カメラ空間での近傍ピクセルを求める。
	float4 posInCamera = CalcUVToPosInCamera(In.uv);
	
	//どれくらい離れているところを調べるかは後で定数バッファで送るようにするんやで。
	//X軸方向に調べる。
	float2 offset[4];
	for(int i = 0; i < 4; i++){
		posInCamera.y += offsetTexelWorld;
		float4 pos = posInCamera;
		pos = mul(mtxProj, pos);
		offset[i] = (pos.xy / pos.w) * float2(0.5f, -0.5f) + 0.5f;
		float4 posInCamera2 = CalcUVToPosInCamera(offset[i]);
		
		if(abs(posInCamera.z - posInCamera2.z) < 5.0f){
			offset[i] -= In.uv ;
		}else{
			offset[i] = 0.0f;
		}
		
	}
	return Blur(In.uv, offset);
	
}
