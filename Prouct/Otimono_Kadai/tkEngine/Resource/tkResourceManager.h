/*!
 * @brief	リソースマネージャ。
 */

#pragma once

namespace tkEngine{
	template <class T>
	class TResourceManager : Noncopyable{
	public:
		TResourceManager()
		{
		}
		~TResourceManager()
		{
		}
		void RegistResource(int nameKey, T* res) 
		{
			m_resourceMap.insert(std::pair<int, T*>(nameKey, res));
		}
		T* GetResource(int nameKey) const
		{
			auto it = m_resourceMap.find(nameKey);
			if (it != m_resourceMap.end()) {
				return it->second;
			}
			return nullptr;
		}
		/*!
		 *@brief	リソースをすべて解放。
		 */
		void Release()
		{
			for (auto& it : m_resourceMap) {
				it.second->Release();
			}
		}
	private:
		std::map<int, T*>	m_resourceMap;
	};
}