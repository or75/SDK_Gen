#include "Factory.hpp"

Factory::Factory( const string& image_name ) : m_hImage( GetModuleHandleA( image_name.c_str() ) )
{
	CreateFactoryMap();
}

auto Factory::CreateFactoryMap() -> void
{
	auto CreateInterfaceFn = reinterpret_cast<uintptr_t>( GetProcAddress( m_hImage , "CreateInterface" ) );

	if ( !CreateInterfaceFn )
		return;

	uintptr_t RelativeAddress = uintptr_t( CreateInterfaceFn ) + 7;
	int32_t RelativeOffset = *(int32_t*)( CreateInterfaceFn + 3 );

	if ( !RelativeOffset )
		return;

	m_pInterfaceArray = *reinterpret_cast<InterfaceReg**>( RelativeAddress + RelativeOffset );

	if ( !m_pInterfaceArray )
		return;

	for ( auto factory = m_pInterfaceArray; factory; factory = factory->m_pNext )
	{
		auto factory_name = string( factory->m_pName );
		auto factory_data = factory->m_CreateFn();

		m_FactoryMap[factory_name] = factory_data;
	}
}