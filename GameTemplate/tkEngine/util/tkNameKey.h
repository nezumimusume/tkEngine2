#pragma once

#include "tkEngine/Util/tkUtil.h"

namespace tkEngine {
	
	class NameKey {
		std::string m_name;			//!<名前
		unsigned int m_hashCode = 0;	//!<ハッシュ値。
	public:
		NameKey(){}
		NameKey( const char* name )
		{
			Init(name);
		}
		//初期化。
		void Init(const char* name)
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
		const char* GetName() const
		{
			return m_name.c_str();
		}
	};
}
