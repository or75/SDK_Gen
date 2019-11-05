#pragma once

#include "../DllMain.hpp"

#define VirtualFn( cast ) using Fn = cast(__thiscall*)

#define CUSTOM_OFFSET( type , offset ) \
	*( type* )( (uintptr_t)this + offset );

#define CUSTOM_OFFSET_RAW( type , offset ) \
	( type* )( (uintptr_t)this + offset );

template< typename T >
inline auto vget( PVOID instance , unsigned int index ) -> T
{
	auto procedure_array = *reinterpret_cast<PDWORD_PTR*>( instance );
	return (T)( procedure_array )[index];
}

auto FindPattern( const char* pattern , uint64_t start , uint64_t end , uint32_t offset = 0 , uint32_t read = 0 ) -> PVOID;
auto FindPattern( const char* module_name , const char* pattern , uint32_t offset = 0 , uint32_t read = 0 ) -> PVOID;

auto GetCallFunction( uint64_t address , uint32_t offset = 1 , uint32_t size = 5 ) -> uint64_t;