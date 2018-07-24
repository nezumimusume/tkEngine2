#pragma once

#include "tkEngine/Util/tkUtil.h"

namespace tkEngine {
	
	template<class T>
	class TNameKey {
		using MyString = std::basic_string<T, std::char_traits<T>, std::allocator<T>>;
		MyString m_name;			//!<名前
		unsigned int m_hashCode = 0;	//!<ハッシュ値。
	public:
		TNameKey(){}
		TNameKey( const T* name )
		{
			Init(name);
		}
		//初期化。
		void Init(const T* name)
		{
			if (name != nullptr) {
				m_name = name;
				//名前をハッシュ値に変換。
				m_hashCode = CUtil::MakeHash(name);
			}
		}
		//ハッシュコードを取得。
		unsigned int GetHashCode() const
		{
			return m_hashCode;
		}
		//名前を取得。
		const T* GetName() const
		{
			return m_name.c_str();
		}
	};

	using NameKey = TNameKey<char>;
	using WNameKey = TNameKey<wchar_t>;
}
