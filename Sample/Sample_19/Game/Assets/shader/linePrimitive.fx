/*!
 * @brief	ラインプリミティブ用のシェーダー。
 */

cbuffer CB : register(b0){
	float4x4 mView;
	float4x4 mProj;
};


/*!
 * @brief	頂点シェーダー
 */
float4 VSMain( float4 pos : SV_Position ) : SV_POSITION
{
	float4 outPos = mul(mView, pos);
	outPos = mul(mProj, outPos);
	return outPos;
}

float4 PSMain( float4 pos : SV_POSITION ) : SV_Target0
{
	return float4( 1.0f, 0.0f, 0.0f, 1.0f );
}
