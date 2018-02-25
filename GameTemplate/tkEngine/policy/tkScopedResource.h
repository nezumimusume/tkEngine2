/*!
 * @brief	スコープを抜けたら自動的に開放されるリソース。
 */

#pragma once

namespace tkEngine{
	template<class T>
	class TScopedResource{
	private:
		
	public:
		
		T* res = nullptr;
		TScopedResource()
		{
		}
		//Moveコンストラクタ。
		TScopedResource(TScopedResource&& a) :
			res(a.res)
		{
			a.res = nullptr;
		}
		//Move代入演算子。
		TScopedResource& operator=(TScopedResource&& a)
		{
			if (this != &a) {
				res = a.res;
				a.res = nullptr;
			}
			return *this;
		}
		~TScopedResource()
		{
			if (res) {
				res->Release();
			}
		}
		
	};
}