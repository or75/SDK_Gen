#include "Memory.hpp"

#define INRANGE(x,a,b)	(x >= a && x <= b) 
#define getBits( x )	(INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )	(getBits(x[0]) << 4 | getBits(x[1]))

auto GetModuleInfo( const char* module_name ) -> MODULEINFO
{
	MODULEINFO module_info = { 0 };

	auto h_module = GetModuleHandleA( module_name );

	if ( h_module == 0 )
		return module_info;

	GetModuleInformation( GetCurrentProcess() , h_module , &module_info , sizeof( MODULEINFO ) );
	return module_info;
}

auto FindPattern( const char* pattern , uint64_t start , uint64_t end , uint32_t offset  , uint32_t read  )->PVOID
{
	uint64_t found_address = 0;
	auto p_pattern = pattern;

	for ( DWORD_PTR dwPtr = start; dwPtr < end; dwPtr++ )
	{
		if ( !*p_pattern )
			return (PVOID)found_address;

		if ( *(PBYTE)p_pattern == '\?' || *(PBYTE)dwPtr == getByte( p_pattern ) )
		{
			if ( !found_address )
				found_address = (DWORD_PTR)dwPtr;

			if ( !p_pattern[1] || !p_pattern[2] )
				break;

			if ( *(PWORD)p_pattern == '\?\?' || *(PBYTE)p_pattern != '\?' )
				p_pattern += 3;
			else
				p_pattern += 2;
		}
		else
		{
			p_pattern = pattern;
			found_address = 0;
		}
	}

	return (PVOID)found_address;
}

auto FindPattern( const char* module_name , const char* pattern , uint32_t offset , uint32_t read  ) -> PVOID
{
	auto Info = GetModuleInfo( module_name );
	auto Start = (DWORD_PTR)Info.lpBaseOfDll;
	auto Size = Info.SizeOfImage;

	return FindPattern( pattern , Start , Start + Size , offset , read );
}

auto GetCallFunction( uint64_t address , uint32_t offset , uint32_t size ) -> uint64_t
{
	auto Offset = int32_t( *(uint32_t*)( address + offset ) );
	auto Address = address + size;

	return Address + Offset;
}