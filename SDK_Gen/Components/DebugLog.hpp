#pragma once

#include "../DllMain.hpp"

class LogFile
{
public:
	auto Create( string szDllDir , string CustomFile ) -> void;

	~LogFile();

	auto WriteDebugLog( const char* sFormat , ... ) -> void;

private:
	// Log File Path
	string m_sLogFilePath = "";

	// Log File Handle
	FILE* m_pFileHandle = nullptr;
};