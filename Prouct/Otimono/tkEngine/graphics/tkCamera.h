/*!
 * @brief	カメラ
 */

#ifndef _TKCAMERA_H_
#define _TKCAMERA_H_

namespace tkEngine{
	class CCamera : Noncopyable{
	public:
		/*!
		* @brief	射影行列の更新方法。
		*/
		enum EnUpdateProjMatrixFunc {
			enUpdateProjMatrixFunc_Perspective,		//!<透視射影行列。遠近法が効いた絵を作りたいならこっち。
			enUpdateProjMatrixFunc_Ortho,			//!<平行投影。２Ｄ的な表現がしたいならこっち。
		};
		CCamera();
		~CCamera();
		/*!
		 * @brief	ビュー行列、プロジェクション行列、ビュープロジェクション行列を更新。
		 */
		virtual void Update();
		/*!
		 * @brief	カメラの座標を設定。
		 *@param[in]	pos		カメラの座標。
		 */
		void SetPosition( const CVector3& pos ) 
		{
			m_position = pos;
		}
		/*!
		 * @brief	カメラの座標を取得
		 *@return	カメラの座標。
		 */
		const CVector3& GetPosition() const
		{
			return m_position;
		}
		/*!
		 * @brief	注視点を設定。
		 *@param[in]	target		注視点。
		 */
		void SetTarget( const CVector3& target )
		{
			m_target = target;
		}
		/*!
		 * @brief	注視点を取得。
		 *@return	注視点。
		 */
		const CVector3& GetTarget() const
		{
			return m_target;
		}
		/*!
		 * @brief	カメラの上方向を設定。
		 */
		void SetUp( const CVector3& up )
		{
			m_up = up;
		}
		/*!
		 * @brief	カメラの上方向を取得
		 */
		const CVector3& GetUp() const
		{
			return m_up;
		}
		/*!
		* @brief	ビュー行列の逆行列を取得。
		*/
		const CMatrix& GetViewMatrixInv() const
		{
			return m_viewMatrixInv;
		}
		/*!
		 * @brief	ビュー行列を取得。
		 */
		const CMatrix& GetViewMatrix() const
		{
			return m_viewMatrix;
		}
		
		/*!
		 * @brief	プロジェクション行列を取得。
		 */
		const CMatrix& GetProjectionMatrix() const
		{
			return m_projectionMatrix;
		}
		/*!
		 * @brief	ビュープロジェクション行列を取得。
		 */
		const CMatrix& GetViewProjectionMatrix() const
		{
			return m_viewProjectionMatrix;
		}
		/*!
		* @brief	カメラの回転行列を取得。
		*/
		const CMatrix& GetCameraRotation() const
		{
			return m_cameraRotation;
		}
		/*!
		 * @brief	遠平面を設定。
		 */
		void SetFar( float fFar )
		{
			m_far = fFar;
		}
		/*!
		 * @brief	近平面を設定。
		 */
		void SetNear( float fNear )
		{
			m_near = fNear;
		}
		/*!
		 * @brief	遠平面を取得。
		 */
		float GetFar() const
		{
			return m_far;
		}
		/*!
		 * @brief	近平面を取得。
		 */
		float GetNear() const
		{
			return m_near;
		}
		/*!
		* @brief	平行投影の幅を設定。
		*@details
		* m_updateProjMatrixFuncがenUpdateProjMatrixFunc_Orthoの時に使用される。
		*/
		void SetWidth(float w)
		{
			m_width = w;
		}
		/*!
		* @brief	平行投影の高さを設定。
		*@details
		* m_updateProjMatrixFuncがenUpdateProjMatrixFunc_Orthoの時に使用される。
		*/
		void SetHeight(float h)
		{
			m_height = h;
		}
		/*!
		* @brief	平行投影の幅を取得
		*/
		float GetWidth() const
		{
			return m_width;
		}
		/*!
		* @brief	平行投影の高さを取得
		*/
		float GetHeight() const
		{
			return m_height;
		}
		/*!
		* @brief	平行投影の高さを設定。
		*@details
		* m_updateProjMatrixFuncがenUpdateProjMatrixFunc_Orthoの時に使用される。
		*/
		void SetUpdateProjMatrixFunc(EnUpdateProjMatrixFunc func)
		{
			m_updateProjMatrixFunc = func;
		}
		/*!
		* @brief	画角を設定。
		* @details
		*  m_updateProjMatrixFuncがenUpdateProjMatrixFunc_Perspectiveの時に使用される。
		*/
		void SetViewAngle(float viewAngle)
		{
			m_viewAngle = viewAngle;
		}
		/*!
		* @brief	画角を取得。
		*/
		float GetViewAngle() const
		{
			return m_viewAngle;
		}
		/*!
		* @brief	注視点と視点の距離を取得。
		*/
		float GetTargetToPositionLength() const
		{
			return m_targetToPositionLen;
		}
		/*!
		* @brief	カメラの前方方向を取得。
		*/
		const CVector3& GetForward() const
		{
			return m_forward;
		}
		/*!
		* @brief	カメラの右方向を取得。
		*/
		const CVector3& GetRight() const
		{
			return m_right;
		}
		/*!
		 *@brief	アスペクト比を取得。
		 */
		float GetAspect() const
		{
			return m_aspect;
		}
		/*!
		*@brief　カメラのコピーを作成。。
		*/
		void CopyTo(CCamera& dst) 
		{
			memcpy(&dst, this, sizeof(dst));
		}
		/*!
		* @brief	ワールド座標からスクリーン座標を計算する。
		*@details
		* 計算されるスクリーン座標は画面の中心を{0,0}、左上を{画面の幅*-0.5,画面の高さ*-0.5}</br>
		* 右下を{画面の幅*0.5,画面の高さ*0.5}とする座標です。
		*@param[out]	screenPos		スクリーン座標の格納先。
		*@param[in]		worldPos		ワールド座標。
		*/
		void CalcScreenPositionFromWorldPosition(CVector2& screenPos, const CVector3& worldPos) const;

#if BUILD_LEVEL != BUILD_LEVEL_MASTER
		CVector3    m_debugCameraPosition = CVector3::Zero; //!<デバッグカメラの座標。
		CVector3	m_debugCameraTarget = CVector3::Zero;	//!<デバッグカメラの注視点。
		bool		m_isDebugCamera = false;
#endif

	protected:
		float		m_targetToPositionLen = 1.0f;		//!<注視点と視点まで距離。
		CVector3	m_position;							//!<カメラ位置。
		CVector3	m_up;								//!<カメラの上方向。
		CVector3	m_target;							//!<カメラの中止点。
		CMatrix		m_viewMatrix;						//!<ビュー行列。
		CMatrix		m_projectionMatrix;					//!<プロジェクション行列。
		CMatrix		m_viewProjectionMatrix;				//!<ビュープロジェクション行列。
		CMatrix		m_viewMatrixInv;					//!<ビュー行列の逆行列。
		CMatrix		m_cameraRotation;					//!<カメラの回転行列。
		CVector3	m_forward = CVector3::Front;		//!<カメラの前方。
		CVector3	m_right = CVector3::Right;			//!<カメラの右。
		float		m_near;								//!<近平面。
		float		m_far;								//!<遠平面。
		float		m_viewAngle;						//!<画角(ラジアン)。
		float		m_aspect;							//!<アスペクト比。
		float		m_width = 1280.0f;
		float		m_height = 720.0f;
		EnUpdateProjMatrixFunc m_updateProjMatrixFunc = enUpdateProjMatrixFunc_Perspective;	//!<プロジェクション行列の更新の仕方。
		bool		m_isNeedUpdateProjectionMatrix;

	};
}
#endif //_TKCAMERA_H_