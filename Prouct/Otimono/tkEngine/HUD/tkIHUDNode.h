
#pragma once

namespace tkEngine{
	class IHUDNode;
	using IHUDNodeWeakPtr = std::weak_ptr<IHUDNode>;
	using IHUDNodeSharedPtr = std::shared_ptr<IHUDNode>;
	/*!
	*@brief	HUDのノードの基底クラス。
	*/
	class IHUDNode : Noncopyable{
	public:
		/*!
		 *@brief	子供を追加。
		 */
		void AddChildren(IHUDNodeWeakPtr weakNode)
		{
			m_childrenNode.push_back(std::move(weakNode));
		}
		
		/*!
		*@brief	ノードの名前を追加。
		*/
		const wchar_t* GetName() const
		{
			return m_nameKey.GetName();
		}
		/*!
		*@brief	ノードの名前キーを追加。
		*/
		int GetNameKey() const
		{
			return m_nameKey.GetHashCode();
		}
		/*!
		* @brief	子供に対して問い合わせ。
		*@param[in]	func	問い合わせ関数。
		*/
		void QueryChildren(std::function< void(IHUDNode* node)> func) 
		{
			for (auto& child : m_childrenNode) {
				func(child.lock().get());
			}
		}
		/*!
		* @brief	引数で渡したノード名のオブジェクトか調べる。
		*@param[in]	objName		調べる名前。
		*@return	名前が同じ場合にtrueを返します。
		*/
		bool EqualNodeName(const wchar_t* nodeName)
		{
			return wcscmp(m_nameKey.GetName(), nodeName) == 0;
		}
	public:
		IHUDNode(CBone* bone)  
		{
			m_nameKey.Init(bone->GetName());
		}
		virtual ~IHUDNode()
		{
		}
		virtual void Update() = 0;
		virtual void PostRender( CRenderContext& rc ) = 0;
		void UpdateWrapper()
		{
			//ワールド行列の更新処理を入れるべきかな・・・。
			Update();
			for (auto& child : m_childrenNode) {
				child.lock()->UpdateWrapper();
			}
		}
		void PostRenderWrapper(CRenderContext& rc)
		{
			PostRender(rc);
			for (auto& child : m_childrenNode) {
				child.lock()->PostRenderWrapper(rc);
			}
		}
		
	protected:
		IHUDNodeWeakPtr m_parentNode;
		std::vector<IHUDNodeWeakPtr>	m_childrenNode;		//子供のノード。
		WNameKey m_nameKey;									//名前キー。
	};
	
}