/*!
 *@brief	ゲームオブジェクトの基底クラス。
 */

#ifndef _CGAMEOBJECT_H_
#define _CGAMEOBJECT_H_


namespace tkEngine{
	class CGameObjectManager;
	class CRenderContext;
	typedef unsigned char	GameObjectPrio;
	/*!
	 *@brief	ゲームオブジェクト。
	 */
	class IGameObject : Noncopyable{
	public:
		using IGameObjectIsBase = IGameObject;
		IGameObject() :
			m_priority(0),
			m_isStart(false),
			m_isDead(false),
			m_isNewFromGameObjectManager(false)
		{
		}
		/*!
		 *@brief	デストラクタ
		 */
		virtual ~IGameObject()
		{
		}
	public:
		/*!
		*@brief	イベント
		*/
		enum EnEvent {
			enEvent_Undef,			//!<未定義イベント。
			enEvent_Destroy,		//!<インスタンスが破棄される。
			enBaseClassEvent_Num,	//!<基底クラスで定義されているイベントの数。
			enEvent_User,			//!<これ以降にユーザー定義のイベントを作成してください。
			
		};
		/*!
		*@brief	イベント発生時のデータ。
		*/
		struct SEventParam {
			EnEvent eEvent = enEvent_Undef;			//!<発生しているイベント。
			IGameObject* gameObject = nullptr;		//!<イベントを通知しているゲームオブジェクトのインスタンス。
		};
		/*!
		*@brief	Updateの直前で呼ばれる開始処理。
		*@details
		* 本関数がtrueを返すとゲームオブジェクトの準備が完了したと判断されて</br>
		* Update関数が呼ばれ出します。trueを返して以降はStart関数は呼ばれなくなります。</br>
		* ゲームオブジェクトの初期化に複数フレームかかる場合などはfalseを返して、初期化ステップなどを使って</br>
		* 適切に初期化を行ってください。
		*/
		virtual bool Start() { return true; }
		/*!
		 *@brief	更新
		 */
		virtual void Update() {}
		/*!
		 *@brief	描画
		 */
		[[deprecated("This function will be delete. please use ForwardRender() function.")]]
		virtual void Render(CRenderContext& renderContext)
		{
			(void)renderContext;
		}
		/*!
		*@brief	フォワードレンダリングのパスから呼ばれる描画処理。
		*@details
		* 特殊なシェーディングを行いたいものはこちらでレンダリングを行ってください。
		* 通常のレンダリングは物理ベースシェーダーが使用されています。
		*/
		virtual void ForwardRender(CRenderContext& renderContext)
		{
			(void)renderContext;
		}
		/*!
		 *@brief	削除されるときに呼ばれる。
		 *@details	CGameManager::DeleteGameObjectを呼んだときに実行されます。
		 * デストラクタより前に実行されます。
		 */
		virtual void OnDestroy(){} 
		/*!
		*@brief	実行優先度を取得。
		*/
		GameObjectPrio GetPriority() const
		{
			return m_priority;
		}
	public:
		/*!
		 *@brief	インスタンスが生成された直後に呼ばれる関数。
		 *@details	コンストラクタが呼ばれた後で即呼ばれます。
		 */
		virtual void Awake(){}
		/*!
		 *@brief	Update関数が実行される前に呼ばれる更新関数。
		 */
		virtual void PreUpdate() {}
		/*!
		 *@brief	Update関数が実行された後で呼ばれる更新関数。
		 */
		virtual void PostUpdate() {} 
		/*!
		 *@brief	Render関数が実行される前に呼ばれる描画処理。
		 */
		[[deprecated("This function will be delete. please use PreForwardRender() function.")]]
		virtual void PreRender(CRenderContext& renderContext) { 
			(void)renderContext; 
		}
		/*!
		*@brief	Render関数が実行される前に呼ばれる描画処理。
		*/
		virtual void PreForwardRender(CRenderContext& renderContext) {
			(void)renderContext;
		}
		/*!
		 *@brief	Render関数が実行された後で呼ばれる描画処理
		 *@details
		 * ポストエフェクトの後で実行されます。HUDなどポストエフェクトの影響を受けたくない描画物はここでレンダリングしてください。
		 */
		virtual void PostRender(CRenderContext& renderContext) {
			(void)renderContext;
		}
		/*!
		*@brief	死亡フラグを立てる。
		*@details
		* この関数はエンジンの外からは実行しないでください。
		*/
		void SetDeadMark()
		{
			m_isDead = true;
		}
		/*!
		*@brief	死亡判定。
		*/
		bool IsDead() const
		{
			return m_isDead;
		}
		/*!
		*@brief Start関数が完了した？
		*/
		virtual bool IsStart() const
		{
			return m_isStart;
		}
		/*!
		*@brief アクティブかどうかを判定。
		*/
		bool IsActive() const
		{
			return m_isActive;
		}
		/*!
		*@brief アクティブフラグを設定。
		*/
		void SetActiveFlag(bool flag)
		{
			m_isActive = flag;
		}
		/*!
		*@brief タグを設定。
		*/
		void SetTags(unsigned int tags)
		{
			m_tags = tags;
		}
		/*!
		*@brief タグを取得。
		*/
		unsigned int GetTags() const
		{
			return m_tags;
		}
		/*!
		*@brief イベントリスナーを登録
		*@param[in]	listener	イベントリスナー。
		*/
		void AddEventListener(std::function<void(SEventParam& eventParam)> listener)
		{
			m_eventListeners.push_back(listener);
		}
#if 0 //ちょいオミット。
		/*!
		*@brief イベントリスナーを破棄
		*@param[in]	listener	破棄するイベントリスナー。
		*/
		void RemoveEventListener(std::function<void(SEventParam& eventParam)> listener)
		{
			auto it = std::remove_if(
				m_eventListeners.begin(), 
				m_eventListeners.end(), 
				[&](auto& l) { return l == listener; }
			);
			m_eventListeners.erase(it, m_eventListeners.end());
		}
#endif
	public:
		void PostRenderWrapper(CRenderContext& renderContext)
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
				PostRender(renderContext);
			}
		}
		void ForwardRenderWrapper(CRenderContext& renderContext)
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
				ForwardRender(renderContext);
			}
		}
		void PreForwardRenderWrapper(CRenderContext& renderContext)
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
				PreForwardRender(renderContext);
			}
		}
		void PostUpdateWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
				PostUpdate();
			}
		}
		void PreUpdateWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
				PreUpdate();
			}
		}
		void UpdateWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
				Update();
			}
		}
		void StartWrapper()
		{
			if (m_isActive && !m_isStart && !m_isDead && !m_isRegistDeadList) {
				if (Start()) {
					//初期化処理完了。
					m_isStart = true;
				}
			}
		}
		void OnDestroyWrapper()
		{
			SEventParam param;
			param.eEvent = enEvent_Destroy;
			param.gameObject = this;
			//デストロイイベントをリスナーに通知する。
			for (auto& listener : m_eventListeners) {
				listener(param);
			}
			OnDestroy();
			
		}
		void SetMarkNewFromGameObjectManager()
		{
			m_isNewFromGameObjectManager = true;
		}
		bool IsNewFromGameObjectManager() const
		{
			return m_isNewFromGameObjectManager;
		}
		
		friend class CGameObjectManager;
	protected:
		GameObjectPrio	m_priority;			//!<実行優先度。
		bool m_isStart;						//!<Startの開始フラグ。
		bool m_isDead;						//!<死亡フラグ。
		bool m_isRegistDeadList = false;	//!<死亡リストに積まれている。
		bool m_isNewFromGameObjectManager;	//!<GameObjectManagerでnewされた。
		bool m_isRegist = false;			//!<GameObjectManagerに登録されている？
		bool m_isActive = true;				//!<Activeフラグ。
		unsigned int m_tags = 0;			//!<タグ。
		unsigned int m_nameKey = 0;			//!<名前キー。
		std::list<std::function<void(SEventParam& eventParam)>>	m_eventListeners;	//イベントリスナー。。
	};
}
#endif // _CGAMEOBJECT_H_