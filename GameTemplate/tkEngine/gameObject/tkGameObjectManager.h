/*!
 *@brief	CGameObjectのマネージャー。
 */

#ifndef _CGAMEOBJECTMANAGER_H_
#define _CGAMEOBJECTMANAGER_H_

#include "tkEngine/gameObject/tkGameObject.h"
#include "tkEngine/util/tkUtil.h"
#include "tkEngine/gameObject/tkTransform.h"

namespace tkEngine{
	struct SRenderContextMap;
	class CPreRender;
	/*!
	 *@brief	CGameObjectのマネージャ
	 */
	class CGameObjectManager : Noncopyable{
	private:
		CGameObjectManager() :
			m_gameObjectPriorityMax(0)
		{
		}
		~CGameObjectManager()
		{
		}
		/*!
		*@brief	ゲームオブジェクトの名前キーを作成。
		*/
		static unsigned int MakeGameObjectNameKey(const char* objectName)
		{
			static const unsigned int defaultNameKey = CUtil::MakeHash("Undefined");	//名前キー。
			unsigned int hash;
			if (objectName == nullptr) {
				hash = defaultNameKey;
			}
			else {
				hash = CUtil::MakeHash(objectName);
			}
			return hash;
		}
	public:
		/*!
		 *@brief	インスタンスの取得。
		 */
		static CGameObjectManager& Instance()
		{
			static CGameObjectManager instance;
			return instance;
		}
		/// <summary>
		/// メインスレッドから呼ばれる実行処理。
		/// </summary>
		void ExecuteFromMainThread();
		/*!
		*@brief	ゲームスレッドから呼ばれる実行処理。
		*/
		void ExecuteFromGameThread();
		/*!
		 *@brief	初期化。
		 *@param[in]	gameObjectPrioMax	ゲームオブジェクトの優先度の最大値。(255まで)
		 */
		void Init( int gameObjectPrioMax );
		/*!
		*@brief	ゲームオブジェクトの追加。
		*@param[in]	prio			実行優先順位。
		*@param[in] go				ゲームオブジェクト
		*@param[in] objectName		オブジェクト名
		*/
		void AddGameObject(GameObjectPrio prio, IGameObject* go, const char* objectName = nullptr)
		{
			(void)objectName;
			if (!go->m_isRegist) {
				go->Awake();
				unsigned int hash = MakeGameObjectNameKey(objectName);
				m_gameObjectListArray.at(prio).push_back(go);
				go->m_isRegist = true;
				go->m_priority = prio;
				go->m_isStart = false;
				go->m_nameKey = hash;
				if (go->m_isDead) {
					//死亡フラグが立っている。
					//削除リストに入っていたらそこから除去する。
					go->m_isDead = false;
				}
				
			}
		}
		/*!
		 *@brief	ゲームオブジェクトのnew
		 *@details
		 * この関数を使用してnewしたオブジェクトは必ずDeleteGameObjectを実行することでdeleteされます。
		 *@param[in]	prio		実行優先順位。
		 *@param[in]	objectName	オブジェクト名。
		 */
		template<class T, class... TArgs>
		T* NewGameObject(GameObjectPrio prio, const char* objectName)
		{
			(void*)objectName;
			TK_ASSERT( prio <= m_gameObjectPriorityMax, "ゲームオブジェクトの優先度の最大数が大きすぎます。");
			T* newObject = new T();
			newObject->Awake();
			newObject->SetMarkNewFromGameObjectManager();
			m_gameObjectListArray.at(prio).push_back(newObject);
			unsigned int hash = MakeGameObjectNameKey(objectName);
			newObject->m_isRegist = true;
			newObject->m_priority = prio;
			newObject->m_nameKey = hash;
			return newObject;
		}
		/*!
		 *@brief	ゲームオブジェクトの削除。
		 */
		void DeleteGameObject( IGameObject* gameObject )
		{
			if (gameObject != nullptr
				&& gameObject->m_isRegist
			) {
				gameObject->SetDeadMark();
				gameObject->OnDestroyWrapper();
				gameObject->m_isRegist = false;
				gameObject->m_isRegistDeadList = true;
		
				m_deleteObjectArray[m_currentDeleteObjectBufferNo].at(gameObject->GetPriority()).push_back(gameObject);
				gameObject = nullptr;
			}
		}
		/*!
		*@brief	ゲームオブジェクトの検索。
		*@details
		* 重いよ！
		*@param[in]	objectName		オブジェクト名。
		*/
		template<class T>
		T* FindGameObject(const char* objectName, bool enableErrorMessage)
		{
			unsigned int nameKey = CUtil::MakeHash(objectName);
			for (auto goList : m_gameObjectListArray) {
				for (auto go : goList) {
					if (go->m_nameKey == nameKey) {
						//見つけた。
						T* p = dynamic_cast<T*>(go);
						if (p == nullptr && enableErrorMessage == true) {
							//型変換に失敗。
							
							TK_WARNING_MESSAGE_BOX(
								"FingGameObject ： 型変換に失敗しました。テンプレート引数を確認してください。typeName = %s, objectName = %s", 
								typeid(T).name(),
								objectName
							);
						}
						return p;
					}
				}
			}
			if (enableErrorMessage == true) {
				TK_WARNING_MESSAGE_BOX("FindGO関数に指定された名前のインスタンスを見つけることができませんでした。\n"
					"名前が間違っていないか確認をして下さい。\n"
					"\n\n検索された名前 【%s】\n", objectName);
			}
			//見つからなかった。
			return nullptr;
		}
		template<class T>
		void FindGameObjects(const char* objectName, std::function<bool(T* go)> func)
		{
			unsigned int nameKey = CUtil::MakeHash(objectName);
			for (auto goList : m_gameObjectListArray) {
				for (auto go : goList) {
					if (go->m_nameKey == nameKey) {
						//見つけた。
						T* p = dynamic_cast<T*>(go);
						if (func(p) == false) {
							//クエリ中断。
							return;
						}
					}
				}
			}
		}
		/*!
		*@brief	指定したタグのいずれかがが含まれるゲームオブジェクトを検索して、見つかった場合指定されたコールバック関数を呼び出す。
		*/
		
		void FindGameObjectsWithTag(unsigned int tags, std::function<void(IGameObject* go)>func)
		{
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					unsigned int goTags = go->GetTags();
					if ((goTags & tags) != 0) {
						func(go);
					}
				}
			}
			
		}
	private:
		/*!
		 *@brief	ゲームオブジェクトの削除を実行。
		 */
		void ExecuteDeleteGameObjects();
		/*!
		*@brief	シーングラフの更新。
		*/
		void UpdateSceneGraph();

		void Start();
		void PreUpdate();
		void Update();
		void PostUpdate();
		void BeginRender(CRenderContext& rc);
		void ForwardPreRender(CRenderContext& rc);
		void ForwardRender(CRenderContext& rc);
		void PostRender(CRenderContext& rc);
	private:
		CTransform m_transform;												//!<Transform。
		typedef std::list<IGameObject*>	GameObjectList;
		std::vector<GameObjectList>	m_gameObjectListArray;					//!<ゲームオブジェクトの優先度付きリスト。
		std::vector<GameObjectList>	m_deleteObjectArray[2];					//!<削除するオブジェクトのリスト。削除処理を行っている最中にDeleteGameObjectが呼ばれる可能性が高いので、ダブルバッファ化。
		std::list<CTransform*>		m_childrenOfRootTransformList;			//!<ルートの子供のTransformのリスト。
		GameObjectPrio				m_gameObjectPriorityMax;				//!<ゲームオブジェクトの優先度の最大数。
		int m_currentDeleteObjectBufferNo = 0;								//!<現在の削除オブジェクトのバッファ番号。
		static const unsigned char 			GAME_OBJECT_PRIO_MAX = 255;		//!<ゲームオブジェクトの優先度の最大値。
	};

	static inline CGameObjectManager& GameObjectManager()
	{
		return CGameObjectManager::Instance();
	}
	/*!
	*@brief	ゲームオブジェクトの検索のヘルパー関数。
	*@details
	* 名前の検索が入るため遅いです。
	*@param[in]	objectName	ゲームオブジェクトの名前。
	*@param[in] enableErrorMessage	エラーメッセージが有効？
	*　呼び出し側でnullptrの判定を行いたい場合は、第二引数にfalseを指定して、エラーメッセージの処理を無効にしてください。
	*
	*@return 見つかったインスタンスのアドレス。見つからなかった場合はnullptrを返す。
	*/
	template<class T>
	static inline T* FindGO(const char* objectName, bool enableErrorMessage = true)
	{
		return GameObjectManager().FindGameObject<T>(objectName, enableErrorMessage);
	}
	/*!
	*@brief	ゲームオブジェクトの検索のヘルパー関数。
	*@details
	* 同名のゲームオブジェクトに全てに対して、クエリを行いたい場合に使用してください。
	*@param[in]	objectName	ゲームオブジェクトの名前。
	*@param[in]	func		ゲームオブジェクトが見つかったときに呼ばれるコールバック関数。
	*/
	template<class T>
	static inline void QueryGOs(const char* objectName, std::function<bool(T* go)> func)
	{
		return GameObjectManager().FindGameObjects<T>(objectName, func);
	}
	/*!
	 *@brief	ゲームオブジェクト生成のヘルパー関数。
	 *@param[in]	priority	プライオリティ。
	 *@param[in]	objectName	オブジェクト名。(NULLの指定可）
	 *@details
	 よくあるコンパイルエラー　よくあるコンパイルエラー　よくあるコンパイルエラー　よくあるコンパイルエラー
	 Ｑ
	 Ｑ
	 Ｑ　　　  【１】 public IGameObjectを忘れてない？
	 Ｑ　　　  【２】 #includeしてる？
     Ｑ
	 Ｑ
	 よくあるコンパイルエラー　よくあるコンパイルエラー　よくあるコンパイルエラー　よくあるコンパイルエラー
	 */
	template<class T>
	static inline T* NewGO( int priority, const char* objectName = nullptr, typename T::IGameObjectIsBase* = nullptr)
	{
		return GameObjectManager().NewGameObject<T>( (GameObjectPrio)priority, objectName);
	}
	
	/*!
	 *@brief	ゲームオブジェクト削除のヘルパー関数。
	 * NewGOを使用して作成したオブジェクトは必ずDeleteGOを実行するように。
	 *@param[in]	go		削除するゲームオブジェクト。
	 */
	static inline void DeleteGO(IGameObject* go)
	{
		GameObjectManager().DeleteGameObject(go);
	}
	/*!
	 *@brief	ゲームオブジェクト削除のヘルパー関数。
	 * NewGOを使用して作成したオブジェクトは必ずDeleteGOを実行するように。
	 *@details
	 * 名前の検索が入るため遅いです。
	 *@param[in]	objectName		削除するゲームオブジェクトの名前。
	 */
	static inline void DeleteGO(const char* objectName)
	{
		IGameObject* go = FindGO<IGameObject>(objectName);
		GameObjectManager().DeleteGameObject(go);
	}
	/*!
	*@brief	ゲームオブジェクトを名前指定で削除。
	*@details
	* 名前検索が行われるため遅いです。
	*@param[in]	objectName		削除するゲームオブジェクトの名前。
	*/
	static inline void DeleteGOs(const char* objectName)
	{
		QueryGOs<IGameObject>(objectName, [](auto go) {
			DeleteGO(go);
			return true;
		});
	}
	/*!
	 *@brief	ゲームオブジェクトの追加のヘルパー関数。
	 *@param[in]	go			追加するゲームオブジェクト。
	 *@param[in]	priority	プライオリティ。
	 *@param[in]	objectName	ゲームオブジェクトの名前。
	 */
	static inline void AddGO(int priority, IGameObject* go, const char* objectName = nullptr)
	{
		GameObjectManager().AddGameObject(static_cast<GameObjectPrio>(priority), go, objectName);
	}
	/*!
	*@brief	指定したタグのいずれかがが含まれるゲームオブジェクトを検索して、見つかった場合指定されたコールバック関数を呼び出す。
	*/
	static inline 	void FindGameObjectsWithTag(unsigned int tags, std::function<void(IGameObject* go)>func)
	{
		GameObjectManager().FindGameObjectsWithTag(tags, func);
	}
	
	
}
#endif // _CGAMEOBJECTMANAGER_H_
 