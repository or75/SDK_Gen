#pragma once

#include "../DllMain.hpp"

using InstantiateInterfaceFn = void* ( * )( );

class InterfaceReg
{
public:
	InstantiateInterfaceFn m_CreateFn = nullptr;
	const char* m_pName = nullptr;
	InterfaceReg* m_pNext = nullptr;
};

class Factory
{
	using factory_map_t = unordered_map< string , void* >;

public:
	Factory( const string& image_name );

public:
	template< typename T >
	inline auto Get( const string& name )
	{
		auto factory = m_FactoryMap[name];

		if ( !factory )
			return (T)0;

		return (T)factory;
	}

private:
	auto CreateFactoryMap() -> void;

private:
	HMODULE m_hImage = nullptr;
	InterfaceReg* m_pInterfaceArray = nullptr;
	factory_map_t m_FactoryMap = { };
};