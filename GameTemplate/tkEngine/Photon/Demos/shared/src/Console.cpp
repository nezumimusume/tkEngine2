#include "Console.h"

Console::Console(void)
	: mpImp(NULL)
{
}

Console::~Console(void)
{
}

void Console::update(void)
{
}

void Console::write(const ExitGames::Common::JString& str)
{
#if defined _EG_MARMALADE_PLATFORM
	std::cout << str.UTF8Representation().cstr();
#else
	std::wcout << str;
#endif
#if defined _EG_WINDOWS_PLATFORM || defined _EG_WINDOWSSTORE_PLATFORM || defined _EG_XB1_PLATFORM
	OutputDebugStringW(str.cstr());
#endif
}

void Console::writeLine(const ExitGames::Common::JString& str)
{
#if defined _EG_MARMALADE_PLATFORM
	std::cout << str.UTF8Representation().cstr() << std::endl;
#else
	std::wcout << str << std::endl;
#endif
#if defined _EG_WINDOWS_PLATFORM || defined _EG_WINDOWSSTORE_PLATFORM || defined _EG_XB1_PLATFORM
	OutputDebugStringW((str+L"\n").cstr());
#endif
}

Console& Console::get(void)
{
	static Console console;
	return console;
}