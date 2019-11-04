#pragma once

#include "../DllMain.hpp"

#define DEBUG_LOG_FILE "debug.log"

class DebugLog
{
public:
	auto Create( string& szDllDir ) -> void;

	~DebugLog();

	auto WriteDebugLog( const char* sFormat , ... ) -> void;

private:
	// Log File Path
	string m_sLogFilePath = "";

	// Log File Handle
	FILE* m_pFileHandle = nullptr;
};