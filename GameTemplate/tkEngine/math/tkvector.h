/*!
 * @brief	ベクトル
 */

#ifndef _TKVECTOR_H_
#define _TKVECTOR_H_


namespace tkEngine{
	class CMatrix;
	
	class CVector2 {
	public:
		static const CVector2 Zero;
		CVector2()
		{

		}
		CVector2(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		/*!
		*@brief	代入演算子。
		*/
		CVector2& operator=(const CVector2& _v)
		{
			vec = _v.vec;
			return *this;
		}
		union {
			DirectX::XMFLOAT2 vec;
			struct { float x, y; };
			float v[2];
		};
		/*!
		* @brief	線形補間。
		*@details
		* this = v0 + (v1-v0) * t;
		*/
		void Lerp(float t, const CVector2& v0, const CVector2& v1)
		{
			x = v0.x + (v1.x - v0.x) * t;
			y = v0.y + (v1.y - v0.y) * t;
		}
	};
	/*!
	 * @brief	ベクトル。
	 */
	class CVector3{
	public:
		union{
			DirectX::XMFLOAT3 vec;
			float v[3];
			struct { float x, y, z; };
		};
		static const CVector3 Zero;
		static const CVector3 Right;
		static const CVector3 Left;
		static const CVector3 Up;
		static const CVector3 Down;
		static const CVector3 Front;
		static const CVector3 Back;
		static const CVector3 AxisX;
		static const CVector3 AxisY;
		static const CVector3 AxisZ;
		static const CVector3 One;
	public:
		//XMVECTORへの暗黙の変換。
		operator DirectX::XMVECTOR() const
		{
			return DirectX::XMLoadFloat3(&vec);
		}
		//operator D3DXVECTOR3(void) { return s_cast<D3DXVECTOR3>(*this); }
		/*!
		*@brief	代入演算子。
		*/
		CVector3& operator=(const CVector3& _v)
		{
			vec = _v.vec;
			return *this;
		}
		CVector3() 
		{
			x = y = z = 0.0f;
		}
		/*!
		* @brief	コンストラクタ。
		*/
		CVector3(float x, float y, float z)
		{
			Set(x, y, z);
		}
		/*!
		* @brief	線形補間。
		*@details
		* this = v0 + (v1-v0) * t;
		*/
		void Lerp(float t, const CVector3& v0, const CVector3& v1)
		{
			DirectX::XMVECTOR _v = DirectX::XMVectorLerp(
				DirectX::XMLoadFloat3(&v0.vec),
				DirectX::XMLoadFloat3(&v1.vec),
				t);
			DirectX::XMStoreFloat3(&vec, _v);
		}
		template<class TVector>
		void CopyTo(TVector& dst) const
		{
			dst.x = x;
			dst.y = y;
			dst.z = z;
		}
		/*!
		* @brief	ベクトルの各要素を設定。
		*/
		void Set(float _x, float _y, float _z)
		{
			vec.x = _x;
			vec.y = _y;
			vec.z = _z;
		}
		template<class TVector>
		void Set(TVector& _v)
		{
			Set(_v.x, _v.y, _v.z);
		}
		void Set(btVector3& _v)
		{
			this->x = _v.x();
			this->y = _v.y();
			this->z = _v.z();
		}
		
		/*!
		 * @brief	ベクトルを加算。
		 */
		void Add( const CVector3& _v) 
		{
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&_v.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd(xmv0, xmv1);
			DirectX::XMStoreFloat3(&vec, xmvr);
		}
		void Add( const CVector3& v0, const CVector3& v1 )
		{
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&v0.vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v1.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd(xmv0, xmv1);
			DirectX::XMStoreFloat3(&vec, xmvr);
		}
		/*!
		 * @brief	ベクトルを減算。
		 */
		void Subtract( const CVector3& _v )
		{
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&_v.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract(xmv0, xmv1);
			DirectX::XMStoreFloat3(&vec, xmvr);
		}
		void Subtract( const CVector3& v0, const CVector3& v1 )
		{
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&v0.vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v1.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract(xmv0, xmv1);
			DirectX::XMStoreFloat3(&vec, xmvr);
		}
		/*!
		 * @brief	内積。
		 */
		float Dot( const CVector3& _v ) const
		{
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&_v.vec);
			return DirectX::XMVector3Dot(xmv0, xmv1).m128_f32[0];
		}
		/*!
		 * @brief	外積。
		 */
		void Cross(const CVector3& _v)
		{
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&_v.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVector3Cross(xmv0, xmv1);
			DirectX::XMStoreFloat3(&vec, xmvr);
		}
		void Cross(const CVector3& v0, const CVector3& v1)
		{
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&v0.vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v1.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVector3Cross(xmv0, xmv1);
			DirectX::XMStoreFloat3(&vec, xmvr);
		}
		/*!
		 * @brief	長さを取得
		 */
		float Length() const
		{
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3(&vec);
			return DirectX::XMVector3Length(xmv).m128_f32[0];
		}
		/*!
		 * @brief	長さの二乗を取得
		 */
		float LengthSq() const
		{
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3(&vec);
			return DirectX::XMVector3LengthSq(xmv).m128_f32[0];
		}
		/*!
		* @brief	拡大。
		*/
		void Scale(float s)
		{
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3(&vec);
			xmv = DirectX::XMVectorScale(xmv, s);
			DirectX::XMStoreFloat3(&vec, xmv);
		}
		/*!
		* @brief	法線を正規化。
		*/
		void Normalize()
		{
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3(&vec);
			xmv = DirectX::XMVector3Normalize(xmv);
			DirectX::XMStoreFloat3(&vec, xmv);
		}
		/*!
		* @brief	除算。
		*/
		void Div(float d)
		{
			float scale = 1.0f / d;
			Scale(scale);
		}
		/*!
		* @brief	最大値を設定。
		*/
		void Max(const CVector3& vMax)
		{
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&vMax.vec);
			DirectX::XMStoreFloat3(&vec,  DirectX::XMVectorMax(xmv0, xmv1));
		}
		/*!
		* @brief	最小値を設定。
		*/
		void Min(const CVector3& vMin)
		{
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&vMin.vec);
			DirectX::XMStoreFloat3(&vec, DirectX::XMVectorMin(xmv0, xmv1));
		}
		/*!
		 *@brief	加算代入演算子。
		 */
		const CVector3& operator+=(const CVector3& _v)
		{
			Add(_v);
			return *this;
		}
		/*!
		*@brief　乗算代入演算子。
		*/
		const CVector3& operator*=(float s) 
		{
			Scale(s);
			return *this;
		}
		/*!
		*@brief	減算代入演算子。
		*/
		const CVector3& operator-=(const CVector3& _v)
		{
			Subtract(_v);
			return *this;
		}
		/*!
		 *@brief	除算代入演算子。
		 */
		const CVector3& operator/=(const float s)
		{
			Div(s);
			return *this;
		}
	};
	/*!
	 *@brief	4要素のベクトルクラス。
	 */
	class CVector4{
	public:
		union{
			DirectX::XMFLOAT4 vec;
			struct { float x, y, z, w; };
			float v[4];
		};
		static const CVector4 White;
		static const CVector4 Black;
		static const CVector4 Yellow;
	public:
		operator DirectX::XMVECTOR() const
		{
			return DirectX::XMLoadFloat4(&vec);
		}
		CVector4()
		{
			x = y = z = w = 0.0f;
		}
		/*!
		*@brief	代入演算子。
		*/
		CVector4& operator=(const CVector4& _v)
		{
			vec = _v.vec;
			return *this;
		}
		/*!
		 *@brief	コンストラクタ
		 */
		CVector4(float x, float y, float z, float w)
		{
			Set(x, y, z, w);
		}
		/*!
		*@brief	コンストラクタ
		*@details
		* wには1.0が格納されます。
		*/
		CVector4(const CVector3& v)
		{
			Set(v);
		}
		/*!
		 *@brief	ベクトルの各要素を設定。
		 */
		void Set(float _x, float _y, float _z, float _w)
		{
			this->x = _x;
			this->y = _y;
			this->z = _z;
			this->w = _w;
		}
		/*!
		* @brief	法線を正規化。
		*/
		void Normalize()
		{
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
			xmv = DirectX::XMVector4Normalize(xmv);
			DirectX::XMStoreFloat4(&vec, xmv);
		}
		/*!
		*@brief	ベクトルを設定。
		*/
		void Set(const CVector4& _v)
		{
			*this = _v;
		}
		/*!
		*@brief	ベクトルを設定。
		*@details
		* wには1.0が格納されます。
		*/
		
		void Set(const CVector3& _v)
		{
			this->x = _v.x;
			this->y = _v.y;
			this->z = _v.z;
			this->w = 1.0f;
		}
		/*!
		 *@brief	ベクトルを加算。
		 */
		void Add( const CVector4& _v )
		{
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&_v.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd(xmv0, xmv1);
			DirectX::XMStoreFloat4(&vec, xmvr);
		}
		void Add( const CVector4& v0, const CVector4& v1 )
		{
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&v0.vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&v1.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd(xmv0, xmv1);
			DirectX::XMStoreFloat4(&vec, xmvr);
		}
		/*!
		 *@brief	ベクトルを減算。
		 */
		void Subtract( const CVector4& _v )
		{
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&_v.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract(xmv0, xmv1);
			DirectX::XMStoreFloat4(&vec, xmvr);
		}
		void Subtract( const CVector4& v0, const CVector4& v1 )
		{
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&v0.vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&v1.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract(xmv0, xmv1);
			DirectX::XMStoreFloat4(&vec, xmvr);
		}
		/*!
		 *@brief	内積
		 */
		float Dot( const CVector4& _v )
		{
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&_v.vec);
			return DirectX::XMVector4Dot(xmv0, xmv1).m128_f32[0];
		}
		/*!
		 * @brief	長さを取得
		 */
		float Length()
		{
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
			return DirectX::XMVector4Length(xmv).m128_f32[0];
		}
		/*!
		 * @brief	長さの二乗を取得
		 */
		float LengthSq()
		{
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
			return DirectX::XMVector4LengthSq(xmv).m128_f32[0];
		}
		/*!
		* @brief	拡大。
		*/
		void Scale(float s)
		{
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
			xmv = DirectX::XMVectorScale(xmv, s);
			DirectX::XMStoreFloat4(&vec, xmv);
		}
	};
	
	class CQuaternion : public CVector4{
	public:
		static const CQuaternion Identity;		//!<単位クォータニオン。
		CQuaternion() 
		{
			x = y = z = 0.0f;
			w = 1.0f;
		}
		CQuaternion(float x, float y, float z, float w) :
			CVector4(x, y, z, w)
		{
		}
		
		/*!
		 *@brief	任意の軸周りの回転クォータニオンを作成。
		 */
		void SetRotation( const CVector3& axis, float angle )
		{
			float s;
			float halfAngle = angle * 0.5f;
			s = sin(halfAngle);
			w = cos(halfAngle);
			x = axis.x * s;
			y = axis.y * s;
			z = axis.z * s;
		}
		void SetRotationDeg(const CVector3& axis, float angle)
		{
			float s;
			float halfAngle = CMath::DegToRad(angle) * 0.5f;
			s = sin(halfAngle);
			w = cos(halfAngle);
			x = axis.x * s;
			y = axis.y * s;
			z = axis.z * s;
		}
		/*!
		*@brief	行列からクォータニオンを作成。
		*/
		void SetRotation(const CMatrix& m);
		/*!
		 *@brief	球面線形補完。
		 */
		void Slerp(float t, CQuaternion q1, CQuaternion q2)
		{
			DirectX::XMVECTOR xmv = DirectX::XMQuaternionSlerp(
				DirectX::XMLoadFloat4(&q1.vec),
				DirectX::XMLoadFloat4(&q2.vec),
				t
			);
			DirectX::XMStoreFloat4(&vec, xmv);
		}
		/*!
		*@brief	クォータニオン同士の積。
		*/
		void Multiply(const CQuaternion& rot)
		{
			float pw, px, py, pz;
			float qw, qx, qy, qz;

			qw = w; qx = x; qy = y; qz = z;
			pw = rot.w; px = rot.x; py = rot.y; pz = rot.z;

			w = pw * qw - px * qx - py * qy - pz * qz;
			x = pw * qx + px * qw + py * qz - pz * qy;
			y = pw * qy - px * qz + py * qw + pz * qx;
			z = pw * qz + px * qy - py * qx + pz * qw;

		}
		/*!
		 *@brief	クォータニオン同士の乗算。
		 *@details
		 * this = rot0 * rot1;
		 */
		void Multiply(const CQuaternion& rot0, const CQuaternion& rot1)
		{
			float pw, px, py, pz;
			float qw, qx, qy, qz;

			qw = rot0.w; qx = rot0.x; qy = rot0.y; qz = rot0.z;
			pw = rot1.w; px = rot1.x; py = rot1.y; pz = rot1.z;

			w = pw * qw - px * qx - py * qy - pz * qz;
			x = pw * qx + px * qw + py * qz - pz * qy;
			y = pw * qy - px * qz + py * qw + pz * qx;
			z = pw * qz + px * qy - py * qx + pz * qw;
		}
		/*!
		*@brief	ベクトルにクォータニオンを適用する。
		*@param[in,out] v	ベクトル。
		*/
		void Multiply(CVector4& _v)
		{
			DirectX::XMVECTOR xmv = DirectX::XMVector3Rotate(_v, *this);
			DirectX::XMStoreFloat4(&_v.vec, xmv);
		}
		void Multiply(CVector3& _v)
		{
			DirectX::XMVECTOR xmv = DirectX::XMVector3Rotate(_v, *this);
			DirectX::XMStoreFloat3(&_v.vec, xmv);
		}
	};
	//整数型のベクトルクラス。
	__declspec(align(16)) class CVector4i {
	public:
		union {
			struct { int x, y, z, w; };
			int v[4];
		};
	};
	/*!
	*@brief	ベクトル同士の加算。
	*/
	template<class TVector>
	static inline TVector operator+(const TVector& v0, const TVector& v1)
	{
		TVector result;
		result.Add(v0, v1);
		return result;
	}
	/*!
	*@brief	ベクトルのスケール倍。
	*/
	template<class TVector>
	static inline TVector operator*(const TVector& v, float s)
	{
		TVector result;
		result = v;
		result.Scale(s);
		return result;
	}
	/*!
	*@brief	ベクトルの除算。
	*/
	template<class TVector>
	static inline TVector operator/(const TVector& v, float s)
	{
		TVector result;
		result = v;
		result.Div(s);
		return result;
	}
	/*!
	*@brief	ベクトル同士の減算。
	*/
	template<class TVector>
	static inline TVector operator-(const TVector& v0, const TVector& v1)
	{
		TVector result;
		result.Subtract(v0, v1);
		return result;
	}
}


#endif // _TKVECTOR_H_