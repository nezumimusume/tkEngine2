/*!
 *@brief		ログ
 */

#ifndef _TKLOG_H_
#define _TKLOG_H_


namespace tkEngine{
	/*!
	 *@brief	ログ出力
	 */
	static inline void Log( const char* format, ... )
	{
		static char log[1024*10];
		va_list va;
		va_start( va, format );
		vsprintf_s( log, format, va );
		OutputDebugString( log );
		OutputDebugString("\n");
		va_end( va );
	}

	/*!
	*@brief	警告出力
	*/
	static inline void Warning(const char* format, ...)
	{
		static char newFormat[1024 * 10];
		strcpy(newFormat, "TK_WARNING : ");
		strcat_s(newFormat, format);		

		static char log[1024 * 10];
		va_list va;
		va_start(va, format);
		vsprintf_s(log, newFormat, va);
		OutputDebugString(log);
		OutputDebugString("\n");
		va_end(va);
	}

	/*!
	*@brief	警告のメッセージボックス
	*/
	static inline void WarningMessageBox(const char* format, ...)
	{
		static char log[1024 * 10];
		va_list va;
		va_start(va, format);
		vsprintf_s(log, format, va);
		OutputDebugString(log);
		OutputDebugString("\n");
		va_end(va);

		MessageBox(NULL, log, "Warning!!", MB_OK);
	}
	/*!
	*@brief	警告のメッセージボックス
	*/
	static inline void WarningMessageBoxW(const wchar_t* format, ...)
	{
		static wchar_t log[1024 * 10];
		va_list va;
		va_start(va, format);
		vswprintf_s(log, format, va);
		OutputDebugStringW(log);
		OutputDebugStringW(L"\n");
		va_end(va);

		MessageBoxW(NULL, log, L"Warning!!", MB_OK);
	}
}

#if BUILD_LEVEL != BUILD_LEVEL_MASTER
	#define TK_LOG( format, ... )	tkEngine::Log(format, __VA_ARGS__)
	#define TK_WARNING(format, ... ) tkEngine::Warning(format, __VA_ARGS__)
	#define TK_WARNING_MESSAGE_BOX(format, ... ) tkEngine::WarningMessageBox(format, __VA_ARGS__)
	#define TK_WARNING_MESSAGE_BOX_W(format, ... ) tkEngine::WarningMessageBoxW(format, __VA_ARGS__)
#else // _DEBUG
#define TK_LOG( format, ... )
#define TK_WARNING(format, ... )
#define TK_WARNING_MESSAGE_BOX(format, ... )
#endif // _DEBUG
#endif // _TKLOG_H_