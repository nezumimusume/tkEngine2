/*!
 * @brief	行列。
 */

#ifndef _TKMATRIX_H_
#define _TKMATRIX_H_

#include "tkEngine/math/tkVector.h"

namespace tkEngine{
	/*!
	 * @brief	行列。
	 */
	class CMatrix {
	public:

		union {
			DirectX::XMFLOAT4X4 mat;
			struct {
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			CVector4 v[4];
			float m[4][4];
		};
		static const CMatrix Identity;	//!<単位行列。
	public:
		operator DirectX::XMMATRIX() const
		{
			return DirectX::XMLoadFloat4x4(&mat);
		}
		operator Effekseer::Matrix44() const
		{
			Effekseer::Matrix44 eMat = *((Effekseer::Matrix44*)&mat);
			return eMat;
		}
		operator Effekseer::Matrix43() const
		{
			Effekseer::Matrix43 eMat;
			eMat.Value[0][0] = mat.m[0][0];
			eMat.Value[0][1] = mat.m[0][1];
			eMat.Value[0][2] = mat.m[0][2];

			eMat.Value[1][0] = mat.m[1][0];
			eMat.Value[1][1] = mat.m[1][1];
			eMat.Value[1][2] = mat.m[1][2];

			eMat.Value[2][0] = mat.m[2][0];
			eMat.Value[2][1] = mat.m[2][1];
			eMat.Value[2][2] = mat.m[2][2];

			eMat.Value[3][0] = mat.m[3][0];
			eMat.Value[3][1] = mat.m[3][1];
			eMat.Value[3][2] = mat.m[3][2];

			return eMat;
		}
		CMatrix() {}
		CMatrix(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) :
			mat(m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33)
		{

		}
		CMatrix(const DirectX::XMFLOAT4X4& m)
		{
			mat = m;
		}
		/*!
		*@brief	代入演算子。
		*/
		CMatrix& operator=(const CMatrix& _m)
		{
			mat = _m.mat;
			return *this;
		}
		/*!
		*@brief	ベクトルと3x3行列の乗算
		*@param[in,out]		v	乗算されるベクトル。
		*/
		void Apply3x3(CVector3& vOut) const
		{
			CVector3 vTmp = vOut;
			vOut.x = vTmp.x * m[0][0] + vTmp.y * m[1][0] + vTmp.z * m[2][0];
			vOut.y = vTmp.x * m[0][1] + vTmp.y * m[1][1] + vTmp.z * m[2][1];
			vOut.z = vTmp.x * m[0][2] + vTmp.y * m[1][2] + vTmp.z * m[2][2];
		}
		/*!
		*@brief	ベクトルに行列を乗算
		*@param[in,out]		v	乗算されるベクトル。
		*/
		void Apply(CVector3& vOut) const
		{
			DirectX::XMStoreFloat3(
				&vOut.vec,
				DirectX::XMVector3Transform(vOut, *this)
			);
		}
		void Apply(CVector4& vOut) const
		{
			DirectX::XMStoreFloat4(
				&vOut.vec,
				DirectX::XMVector4Transform(vOut, *this)
			);
		}
		[[deprecated("This function will be delete. please use Apply() function.")]]
		void Mul(CVector3& vOut) const
		{
			Apply(vOut);
		}
		[[deprecated("This function will be delete. please use Apply() function.")]]
		void Mul(CVector4& vOut) const
		{
			Apply(vOut);
		}
		/*!
		 *@brief	平行移動行列を作成。
		 */
		void MakeTranslation(const CVector3& trans)
		{
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixTranslationFromVector(trans)
			);
		}
		/*!
		*@brief	Y軸周りの回転行列を作成。
		*@param[in]	angle	回転角度(単位ラジアン)
		*/
		void MakeRotationY(float angle)
		{
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixRotationY(angle)
			);
		}
		/*!
		*@brief	Z軸周りの回転行列を作成。
		*@param[in]	angle	回転角度(単位ラジアン)
		*/
		void MakeRotationZ(float angle)
		{
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixRotationZ(angle)
			);
		}
		/*!
		*@brief	X軸周りの回転行列を作成。
		*@param[in]	angle	回転角度(単位ラジアン)
		*/
		void MakeRotationX(float angle)
		{
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixRotationX(angle)
			);
		}
		/*!
		 *@brief	クォータニオンから回転行列を作成。
		 *@param[in]	q		クォータニオン
		 */
		void MakeRotationFromQuaternion(const CQuaternion& q)
		{
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixRotationQuaternion(q)
			);
		}
		/*!
		*@brief	任意の軸周りの回転行列を作成。
		*@param[in]	axis	回転軸。
		*@param[in]	angle	回転角度
		*/
		void MakeRotationAxis(const CVector3& axis, float angle)
		{
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixRotationAxis(axis, angle)
			);
		}
		/*!
		*@brief	拡大行列を作成。
		*@param[in] scale		拡大率。
		*/
		void MakeScaling(const CVector3& scale)
		{
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixScalingFromVector(scale)
			);
		}
		/*!
		* @brief	プロジェクション行列を作成。
		*@param[in]	viewAngle	画角。(ラジアン)。
		*@param[in]	aspect		アスペクト比。
		*@param[in]	fNear		近平面。
		*@param[in]	fFar		遠平面。
		*/
		void MakeProjectionMatrix(
			float viewAngle,
			float aspect,
			float fNear,
			float fFar
		)
		{
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixPerspectiveFovLH(viewAngle, aspect, fNear, fFar)
			);
		}
		/*!
		* @brief	平行投影行列を作成。
		*@param[in]	w			ビューボリュームの幅。
		*@param[in]	h			ビューボリュームの高さ。
		*@param[in]	fNear		近平面。
		*@param[in]	fFar		遠平面。
		*/
		void MakeOrthoProjectionMatrix(float w, float h, float fNear, float fFar)
		{
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixOrthographicLH(w, h, fNear, fFar)
			);
		}
		/*!
		 * @brief	注視点、上方向、カメラ位置からカメラ行列を作成。
		 *@param[in]	position	カメラ位置。
		 *@param[in]	target		注視点。
		 *@param[in]	up			カメラの上方向。
		 */
		void MakeLookAt(const CVector3& position, const CVector3& target, const CVector3& up)
		{
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixLookAtLH(position, target, up)
			);
		}
		/*!
		 *@brief	行列と行列の乗算
		 *@details
		 * *this = m0 * m1
		 */
		void Multiply(const CMatrix& m0, const CMatrix& m1)
		{
			DirectX::XMFLOAT4X4 lm;
			DirectX::XMStoreFloat4x4(
				&lm,
				DirectX::XMMatrixMultiply(m0, m1)
			);
			mat = lm;
		}
		[[deprecated("This function will be delete. please use Multiply() function.")]]
		void Mul(const CMatrix& m0, const CMatrix& m1)
		{
			Multiply(m0, m1);
		}
		/*!
		*@brief	行列の代入演算子
		*@details
		* this = this * _m;
		*/
		const CMatrix& operator*=(const CMatrix& _m)
		{
			Multiply(*this, _m);
			return *this;
		}
		/*!
		 *@brief	逆行列を計算。
		 *@param[in]	m	元になる行列。
		 */
		void Inverse(const CMatrix& _m)
		{
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixInverse(NULL, _m)
			);
		}
		void Inverse()
		{
			Inverse(*this);
		}
		void Transpose()
		{
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixTranspose(*this)
			);
		}
		
	};
	/*!
	*@brief	行列同士の乗算。
	*@details
	* 乗算は左から右に向かってかかっていく。
	*/
	static inline CMatrix operator*(const CMatrix& m1, const CMatrix m2)
	{
		CMatrix mRet;
		mRet.Multiply(m1, m2);
		return mRet;
	}

};
#endif // _TKMATRIX_H_