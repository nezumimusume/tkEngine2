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
		strcpy_s(newFormat, "TK_WARNING : ");
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
	static inline void WarningMessageBox(const char* file, long line, const char* format,  ...)
	{
		static char log[1024 * 10];
		va_list va;
		va_start(va, format);
		char fileLineInfo[256];
		sprintf_s(fileLineInfo, "\n\n%s, %d行目", file, line);
		vsprintf_s(log, format, va);
		strcat_s(log, fileLineInfo);
		va_end(va);

		MessageBox(NULL, log, "Warning!!", MB_OK);
	}
	/*!
	*@brief	警告のメッセージボックス
	*/
	static inline void WarningMessageBoxW(const char* file, long line, const wchar_t* format,  ...)
	{
		static wchar_t log[1024 * 10];
		wchar_t wFile[256];
		size_t numStr;
		mbstowcs_s(&numStr, wFile, 256, file, strlen(file) + 1);
		wchar_t fileLineInfo[256];
		swprintf_s(fileLineInfo, L"\n\n%s, %d行目", wFile, line);
		va_list va;
		va_start(va, format);
		vswprintf_s(log, format, va);
		wcscat_s(log, fileLineInfo);
		OutputDebugStringW(log);
		OutputDebugStringW(L"\n");
		va_end(va);

		MessageBoxW(NULL, log, L"Warning!!", MB_OK);
	}
}

#if BUILD_LEVEL != BUILD_LEVEL_MASTER
	#define TK_LOG( format, ... )	tkEngine::Log(format, __VA_ARGS__)
	#define TK_WARNING(format, ... ) tkEngine::Warning(format, __VA_ARGS__)
	#define TK_WARNING_MESSAGE_BOX(format, ... ) tkEngine::WarningMessageBox(__FILE__, __LINE__,  format, __VA_ARGS__)
	#define TK_WARNING_MESSAGE_BOX_W(format, ... ) tkEngine::WarningMessageBoxW(__FILE__, __LINE__,format, __VA_ARGS__)
#else // _DEBUG
#define TK_LOG( format, ... )
#define TK_WARNING(format, ... )
#define TK_WARNING_MESSAGE_BOX(format, ... )
#define TK_WARNING_MESSAGE_BOX_W(format, ... )
#endif // _DEBUG
#endif // _TKLOG_H_