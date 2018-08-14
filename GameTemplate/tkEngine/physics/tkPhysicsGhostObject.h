/*!
 * @brief	ゴーストオブジェクト。
 */

#pragma once


namespace tkEngine{
	/*!
	 * @brief	ゴーストオブジェクト
	 *@detail
	 * ゴーストオブジェクトは剛体がすり抜けるあたり判定のオブジェクトです。
	 * キャラクターのチェックポイント通過判定、クリア判定などの
	 * キャラクターがすり抜ける必要のあるあたり判定に使えます。
	 */
	class CPhysicsGhostObject{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CPhysicsGhostObject();
		/*!
		 * @brief	デストラクタ。
		 */
		~CPhysicsGhostObject();
		/*!
		 * @brief	ボックス形状のゴーストオブジェクトを作成。
		 *@param[in]	pos			座標。
		 *@param[in]	rot			回転。
		 *@param[in]	size		サイズ。	
		 */
		void CreateBox(CVector3 pos, CQuaternion rot, CVector3 size);
		/*!
		 * @brief	カプセル形状のゴーストオブジェクトを作成。
		 *@param[in]	pos			座標。
		 *@param[in]	rot			回転。
		 *@param[in]	radius		カプセルの半径。
		 *@param[in]	height		カプセルの高さ。
		 */
		void CreateCapsule(CVector3 pos, CQuaternion rot, float radius, float height );
		/*!
		* @brief	球体形状のゴーストオブジェクトを作成。
		*@param[in]	pos			座標。
		*@param[in]	rot			回転。
		*@param[in]	radius		球体の半径。
		*/
		void CreateSphere(CVector3 pos, CQuaternion rot, float radius);
		/*!
		* @brief	メッシュ形状のゴーストオブジェクトを作成。
		*@param[in]	pos					座標。
		*@param[in]	rot					回転。
		*@param[in]	skinModelData		スキンモデルデータ。
		*/
		void CreateMesh(CVector3 pos, CQuaternion rot, const CSkinModelData& skinModelData);
		/*!
		* @brief	ゴーストオブジェクトを解放。
		*@detail
		* 明示的なタイミングでゴーストオブジェクトを削除したい場合に呼び出してください。
		*/
		void Release();
		/*!
		* @brief	引数で渡されたゴーストオブジェクトが自分自身かどうか判定。
		*/
		bool IsSelf(const btCollisionObject& ghost) const
		{
			return &ghost == &m_ghostObject;
		}
		/*!
		* @brief	座標を設定。
		*/
		void SetPosition(const CVector3& pos)
		{
			auto& btTrans = m_ghostObject.getWorldTransform();
			btVector3 btPos;
			pos.CopyTo(btPos);
			btTrans.setOrigin(btPos);
		}
		/*!
		* @brief	回転を設定。
		*/
		void SetRotation(const CQuaternion& rot)
		{
			auto& btTrans = m_ghostObject.getWorldTransform();
			btQuaternion btRot;
			rot.CopyTo(btRot);
			btTrans.setRotation(btRot);
		}
	private:
		/*!
		* @brief	ゴースト作成処理の共通処理。
		*/
		void CreateCommon(CVector3 pos, CQuaternion rot, std::unique_ptr<ICollider> collider);
	private:
		bool						m_isRegistPhysicsWorld = false;	//!<物理ワールドに登録しているかどうかのフラグ。
		btGhostObject				m_ghostObject;	//!<ゴースト
		std::unique_ptr<ICollider>	m_collider;		//!<コライダー。
	};
}