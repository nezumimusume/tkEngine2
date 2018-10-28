/*!
 *@brief	アサート
 */

#ifndef _TKASSERT_H_
#define _TKASSERT_H_

#include <stdarg.h>

namespace tkEngine{
	/*!
	 *@brief	アサート
	 *@param[in]	flag	偽のときアサートが発生します。
	 */
	static inline void Assert( bool flag, const char* format, const char* file, long line, ... )
	{
		if(!flag){
			va_list va;
			va_start( va, flag );
			vprintf( format, va );
			char fileLineInfo[256];
			sprintf_s(fileLineInfo, "%s, %d行目", file, line );
			char assertMessage[256];
			vsprintf_s(assertMessage, format, va);
			strcat_s(assertMessage, fileLineInfo);
			MessageBox(nullptr, assertMessage, "アサート", MB_OK);
			va_end( va );
			std::abort();
		}
	}
}
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
#define 	TK_ASSERT( flg, format, ... )	tkEngine::Assert( flg, format, __FILE__, __LINE__, __VA_ARGS__)
#else
#define 	TK_ASSERT( flg, format, ... )
#endif 
#endif