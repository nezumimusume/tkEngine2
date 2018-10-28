/*!
 * @brief	ベクトルクラス。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/math/tkVector.h"

namespace tkEngine{

	const CVector4 CVector4::White = {1.0f, 1.0f, 1.0f, 1.0f};
	const CVector4 CVector4::Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	const CVector4 CVector4::Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	const CVector4 CVector4::Gray = { 0.5f, 0.5f, 0.5f, 1.0f };

	const CVector2 CVector2::Zero   = { 0.0f,  0.0f };
	const CVector3 CVector3::Zero	= {  0.0f,  0.0f,  0.0f };
	const CVector3 CVector3::Right	= {  1.0f,  0.0f,  0.0f };
	const CVector3 CVector3::Left	= { -1.0f,  0.0f,  0.0f };
	const CVector3 CVector3::Up		= {  0.0f,  1.0f,  0.0f };
	const CVector3 CVector3::Down	= {  0.0f, -1.0f,  0.0f };
	const CVector3 CVector3::Front	= { 0.0f,   0.0f,  1.0f };
	const CVector3 CVector3::Back	= { 0.0f,   0.0f, -1.0f };

	const CVector3 CVector3::AxisX	= { 1.0f,  0.0f,  0.0f };
	const CVector3 CVector3::AxisY	= { 0.0f,  1.0f,  0.0f };
	const CVector3 CVector3::AxisZ	= { 0.0f,  0.0f,  1.0f };
	const CVector3 CVector3::One = { 1.0f, 1.0f, 1.0f };
	const CQuaternion CQuaternion::Identity = { 0.0f,  0.0f, 0.0f, 1.0f };

	/*!
	*@brief	行列からクォータニオンを作成。
	*/
	void CQuaternion::SetRotation(const CMatrix& m)
	{
		DirectX::XMStoreFloat4(&vec, DirectX::XMQuaternionRotationMatrix(m));
	}
	/*!
	*@brief	fromベクトルからtoベクトルに回転させるクォータニオンを作成。
	*/
	void CQuaternion::SetRotation(CVector3 from, CVector3 to)
	{
	from.Normalize();
		to.Normalize();
		auto t = tkEngine::Dot(from, to);
		CVector3 rotAxis;
		if (t > 0.998f) {
			//ほぼ同じ向きなので単位クォータニオンにする。
			*this = CQuaternion::Identity;
		}
		else if (t < -0.998f) {
			//ほぼ逆向きなので、
			if (fabsf(to.x) < 1.0f) {
				//
				rotAxis = Cross(CVector3::AxisX, to);
			}
			else {
				rotAxis = Cross(CVector3::AxisY, to);
			}
		}
		else {
			rotAxis = Cross(from, to);
		}
		rotAxis.Normalize();
		SetRotation(rotAxis, acos(t));
	}
}