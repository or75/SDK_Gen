#include "DebugLog.hpp"

auto DebugLog::Create( string& szDllDir ) -> void
{
	m_sLogFilePath = szDllDir + "\\" + DEBUG_LOG_FILE;
	m_pFileHandle = _fsopen( m_sLogFilePath.c_str() , "a+" , SH_DENYNO );
}

auto DebugLog::WriteDebugLog( const char* sFormat , ... ) -> void
{
	if ( m_pFileHandle )
	{
		va_list vaArgs;
		va_start( vaArgs , sFormat );

		va_list vaCopy;
		va_copy( vaCopy , vaArgs );
		const int iLen = std::vsnprintf( NULL , 0 , sFormat , vaCopy );
		va_end( vaCopy );

		vector<char> zc( iLen + 1 );
		vsnprintf( zc.data() , zc.size() , sFormat , vaArgs );
		va_end( vaArgs );

		auto buffer = string( zc.data() , zc.size() );

		fputs( buffer.c_str() , m_pFileHandle );
		fflush( m_pFileHandle );
	}
}

DebugLog::~DebugLog()
{
	fclose( m_pFileHandle );
	m_pFileHandle = nullptr;
}