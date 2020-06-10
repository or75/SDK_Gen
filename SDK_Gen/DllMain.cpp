#include "DllMain.hpp"
#include "Generator.hpp"

string DllDir = "";

auto GeneratorThread( HINSTANCE hinstDLL ) -> DWORD
{
	char sDllDir[MAX_PATH] = { 0 };

	if ( GetModuleFileNameA( hinstDLL , sDllDir , MAX_PATH ) )
		DllDir = filesystem::path( sDllDir ).parent_path().string() + "\\";

	auto m_pGenerator = new SDK_Generator();

	m_pGenerator->Generate();

	delete m_pGenerator;

	FreeLibraryAndExitThread( hinstDLL , 0 );

	return 0;
}

auto WINAPI DllMain( HINSTANCE hinstDLL , DWORD Reason , PVOID pReserved ) -> BOOL
{
	switch ( Reason )
	{
		case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls( hinstDLL );
			CreateThread( NULL , NULL , (LPTHREAD_START_ROUTINE)GeneratorThread , hinstDLL , NULL , NULL );
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
	}

	return TRUE;
}