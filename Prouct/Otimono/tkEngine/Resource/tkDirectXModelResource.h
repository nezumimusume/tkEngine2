/*!
 *@brief	DirectX::Modelリソース管理クラス。
 */
#pragma once

namespace tkEngine {
	class CDirectXModelResource : Noncopyable{
	public:
		/*!
		 *@brief	コンストラクタ。
		 */
		CDirectXModelResource();
		/*!
		 *@brief	デストラクタ。
		 */
		~CDirectXModelResource();
		/*!
		 *@brief	DirectX::Modelリソースのロード。
		 */
		DirectX::Model* Load(const wchar_t* filePath, CSkeleton& skeleton);
		/*!
		*@brief	リソースをすべて解放。
		*/
		void Release()
		{
			m_resourceMap.clear();
		}
	private:
		static bool m_isInstanced;		//実体が生成されているかどうかのフラグ。
		using DirectXModelPtr = std::unique_ptr<DirectX::Model>;
		std::map<int, DirectXModelPtr> m_resourceMap;
	};
}