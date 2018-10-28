/*!
 *@brief	各種ライト
 */

#pragma once

namespace tkEngine{
	/*!
	 *@brief	ディレクションライト。
	 */
	struct SDirectionLight{
		CVector4	color;			//!<ライトのカラー。
		CVector3	direction;		//!<ライトの方向。
		unsigned int lightingMaterialIDGroup;	//!<ライトの影響を与えるマテリアルIDのグループ。
	};
	/*!
	 *@brief	アンビエントライト。
	 */
	struct SAmbientLight{
		CVector3	color;			//!<ライトのカラー。
	};
	/*!
	 *@brief	ポイントライト。
	 *@details
	 * このパラメータを変更したら、LightCulling.fxとmodelStruct.hを変更するように。
	 */
	struct SPointLight{
		CVector3	position;		//!<位置。
		CVector3	positionInView;	//!<ビュー空間での座標。
		CVector4	color;			//!<ライトのカラー。
		CVector4	attn;			//!<減衰定数。xはポイントライトの影響が届く範囲。yはポイントライトの減衰率に影響を与えます。
									//!<yが大きくなると、減衰が強くなります。1.0で線形の減衰率になります。z,wは未使用。
		unsigned int lightingMaterialIDGroup;	//!<ライトの影響を与えるマテリアルIDのグループ。
	};
	/*!
	 * @brief	スポットライト。
	 * このパラメータを変更したら、LightCulling.fxとmodelStruct.hを変更するように。
	 */
	struct SSpotLight{
		CVector3	position;		//!<位置。
		CVector3	direction;		//!<方向。
		CVector3	color;			//!<カラー。
		CVector4	attn;			//!<減衰定数。xはポイントライトの影響が届く範囲。yはポイントライトの減衰率に影響を与えます。
									//!<yが大きくなると、減衰が強くなります。1.0で線形の減衰率になります。z,wは未使用。
		unsigned int lightingMaterialIDGroup;	//!<ライトの影響を与えるマテリアルIDのグループ。
	};
}