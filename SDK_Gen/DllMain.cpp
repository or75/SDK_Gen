#include "DllMain.hpp"
#include "Generator.hpp"

auto GeneratorThread( HINSTANCE hinstDLL ) -> void
{
	auto m_pGenerator = new SDK_Generator( hinstDLL );

	m_pGenerator->Generate();

	delete m_pGenerator;
}

auto Initalize( HINSTANCE hinstDLL ) -> void
{
	thread Generator( GeneratorThread , hinstDLL );

	Generator.detach();
}

auto WINAPI DllMain( HINSTANCE hinstDLL , DWORD Reason , PVOID pReserved ) -> BOOL
{
	switch ( Reason )
	{
		case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls( hinstDLL );
			Initalize( hinstDLL );
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
	}

	return TRUE;
}