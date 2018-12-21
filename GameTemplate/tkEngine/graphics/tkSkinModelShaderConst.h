/*!
 *@brief	スキンモデル関係関係のシェーダーパラメータ。
 */

#pragma once

namespace tkEngine{
	/*!
	 *@brief	定数バッファのレジスタ番号
	 */
	enum EnSkinModelCBReg{
		enSkinModelCBReg_VSPS,		//!<頂点シェーダーとピクセルシェーダー共通の定数バッファ。
		enSkinModelCBReg_Light,		//!<ライト用の定数バッファ。
		enSkinModelCBReg_Material,	//!<マテリアルパラメータ。
		enSkinModelCBReg_Shadow,	//!<影。
		enSkinModelCBReg_GBuffer,	//!<G-Buffer。
		
	};
	/*!
	 *@brief	SRVのレジスタ番号。
	 */
	enum EnSkinModelSRVReg{
		enSkinModelSRVReg_AlbedoTexture = 0,		//!<アルベドテクスチャ。
		enSkinModelSRVReg_NormalMap,				//!<法線マップ。
		enSKinModelSRVReg_Specularmap,				//!<スペキュラマップ。
		enSkinModelSRVReg_ShadowMap_0,				//!<シャドウマップ
		enSkinModelSRVReg_ShadowMap_1,				//!<シャドウマップ
		enSkinModelSRVReg_ShadowMap_2,				//!<シャドウマップ
		enSkinModelSRVReg_SilhouetteTexture,		//!<シルエットテクスチャ。
		enSkinModelSRVReg_SoftShadowMap,			//!<ソフトシャドウマップ。
		enSkinModelSRVReg_DepthMap,					//!<深度マップ。
		enSkinModelSRVReg_Tangent,					//!<接ベクトル。
		enSkinModelSRVReg_MaterialID,				//!<マテリアルID。
		enSkinModelSRVReg_EmissionColor,			//!<エミッションカラー。
		
		enSkinModelSRVReg_DirectionLight = 100,		//!<ディレクションライト。
		enSkinModelSRVReg_PointLight,				//!<ポイントライトのリスト。
		enSkinModelSRVReg_PointLightListInTile,		//!<タイルに含まれるポイントライトのリスト。
		enSkinModelSRVReg_BoneMatrix,				//!<ボーン行列。
		enSkinModelSRVReg_InstanceMatrix,			//!<インスタンシング用の行列。
	};
}