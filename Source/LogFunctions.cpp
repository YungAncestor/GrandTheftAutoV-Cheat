#include "stdafx.h"

void Cheat::LogFunctions::Init()
{
	std::cout << CheatFunctions::ReturnDateTimeFormatAsString(xorstr_("[%d-%m-%Y - %H:%M:%S]")) << xorstr_(" [Init] ") << xorstr_("Initializing GTAV Cheat") << xorstr_(" | Github Repository: https://github.com/HowYouDoinMate/GrandTheftAutoV-Cheat") << std::endl;
}

void Cheat::LogFunctions::Message(char* Message)
{
	std::cout << CheatFunctions::ReturnDateTimeFormatAsString(xorstr_("[%d-%m-%Y - %H:%M:%S]")) << xorstr_(" [Info] ") << Message << std::endl;
}

void Cheat::LogFunctions::DebugMessage(char* Message)
{
	std::cout << CheatFunctions::ReturnDateTimeFormatAsString(xorstr_("[%d-%m-%Y - %H:%M:%S]")) << xorstr_(" [Debug] ") << Message << std::endl;
}

void Cheat::LogFunctions::Error(char* Message)
{
	std::cout << CheatFunctions::ReturnDateTimeFormatAsString(xorstr_("[%d-%m-%Y - %H:%M:%S]")) << xorstr_(" [Error] ") << Message << " " << std::endl;
	MessageBoxA(NULL, Message, xorstr_("Cheat Error"), MB_TOPMOST | MB_ICONERROR | MB_SETFOREGROUND);
}
