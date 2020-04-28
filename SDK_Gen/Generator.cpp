#include "Generator.hpp"

vector<VMT_IndexList_t> m_vDotaOffsetList =
{
	{ ENGINE_DLL , "GetMaxClients" , index::IEngineClient::PATTERN_GetMaxClients, index::IEngineClient::GetMaxClients },
	{ CLIENT_DLL , "IsInGame" , index::IEngineClient::PATTERN_IsInGame, index::IEngineClient::IsInGame },
	{ CLIENT_DLL , "Con_NPrintf" , index::IEngineClient::PATTERN_Con_NPrintf, index::IEngineClient::Con_NPrintf },
	{ CLIENT_DLL , "GetNetChannelInfo" , index::IEngineClient::PATTERN_GetNetChannelInfo, index::IEngineClient::GetNetChannelInfo },
	{ CLIENT_DLL , "ClientCmd_Unrestricted" , index::IEngineClient::PATTERN_ClientCmd_Unrestricted, index::IEngineClient::ClientCmd_Unrestricted },
	{ CLIENT_DLL , "OnAddEntity" , index::CGameEntitySystem::PATTERN_OnAddEntity, index::CGameEntitySystem::OnAddEntity , true },
};

SDK_Generator::SDK_Generator( HINSTANCE hinstDLL )
{
	char sDllDir[MAX_PATH] = { 0 };
	
	if ( GetModuleFileNameA( hinstDLL , sDllDir , MAX_PATH ) )
	{
		m_sDllDir = filesystem::path( sDllDir ).parent_path().string();

		m_pLog = new DebugLog();
		m_pLog->Create( m_sDllDir );
	}
}

SDK_Generator::~SDK_Generator()
{
	if ( m_pLog )
	{
		m_pLog->WriteDebugLog( "\n[SDK-Gen] End\n" );
		delete m_pLog;
	}
}

auto SDK_Generator::Generate() -> void
{
	if ( !m_pLog )
		return;

	m_pLog->WriteDebugLog( "[SDK-Gen] Start\n\n" );

	while ( !GetModuleHandleA(  "navsystem.dll" ) )
		Sleep( 1000 );

	Factory SchemaFactory( SCHEMASYSTEM_DLL );

	m_pSchemaSystem = SchemaFactory.Get<CSchemaSystem*>( INTERFACE_SCHEMASYSTEM );

	if ( !m_pSchemaSystem )
	{
		m_pLog->WriteDebugLog( "[SDK-Gen] Error m_pSchemaSystem\n" );
		return;
	}

	m_pLog->WriteDebugLog( "[SDK-Gen] m_pSchemaSystem: %p\n" , m_pSchemaSystem );
	m_pLog->WriteDebugLog( "\n" );

	if ( !DumpEnumClassOffset() )
		return;

	Find_DotaVMT_Index();

#if DUMP_SCHEMA_ALL == 1

	auto GobalScope = m_pSchemaSystem->GlobalTypeScope();
	auto AllScope = m_pSchemaSystem->AllTypeScope();

	m_pLog->WriteDebugLog( "GobalScope: %p\n" , GobalScope );
	m_pLog->WriteDebugLog( "AllScope: %p\n\n" , AllScope );

	if ( GobalScope && AllScope )
	{
		uint8_t idx = 0;
		CSchemaSystemTypeScope* Scope = nullptr;

		DumpAllEnumList( GobalScope );
		DumpAllClassList( GobalScope );

#if DEBUG_SCHEMA == 1

		m_pLog->WriteDebugLog( "\nAllScope DUMP:\n\n" );

#endif

		while ( ( Scope = AllScope[idx++] ) )
		{
			DumpAllEnumList( Scope );
			DumpAllClassList( Scope );
		}
	}

	return;

#endif
}

auto SDK_Generator::DumpAllEnumList( CSchemaSystemTypeScope* Scope ) -> void
{
	auto enum_container = Scope->GetEnumContainer();

#if DEBUG_SCHEMA == 1
	m_pLog->WriteDebugLog( "enum_container: %p\n" , enum_container );
	MessageBoxA( 0 , "enum_container" , "enum_container" , MB_ICONERROR );
#endif

	if ( enum_container )
	{
		int blockIndex = 0;

#if DEBUG_SCHEMA == 1
		m_pLog->WriteDebugLog( "GetBlockContainers: %p\n" , &enum_container->GetBlockContainers() );
#endif

		for ( auto& schema_block : enum_container->GetBlockContainers() )
		{

#if DEBUG_SCHEMA == 1
			m_pLog->WriteDebugLog( "schema_block: %p , %p , %p\n" , &schema_block , schema_block.GetFirstBlock() , schema_block.m_firstBlock );
#endif

			for ( auto block = schema_block.GetFirstBlock(); block && blockIndex < enum_container->GetNumSchema(); block = block->Next() , ++blockIndex )
			{

#if DEBUG_SCHEMA == 1
				m_pLog->WriteDebugLog( "block: %p -> %p -> %p\n" , block , block->GetBinding() , block->Next() );
				MessageBoxA( 0 , "block" , "block" , MB_ICONERROR );
#endif

				auto binding = block->GetBinding();

				if ( !binding )
					continue;

				m_pLog->WriteDebugLog( "enum %s : %s\n{\n" , binding->m_bindingName , binding->GenerateTypeStorage() );

				for ( auto idx = 0; idx < binding->size; idx++ )
				{
					auto enum_data = binding->m_DataArray[idx];

					if ( enum_data.FieldName )
					{
						switch ( binding->m_TypeSize )
						{
							case 1:
								m_pLog->WriteDebugLog( "\t%s = 0x%02X ,\n" , enum_data.FieldName , enum_data.FieldData );
								break;
							case 2:
								m_pLog->WriteDebugLog( "\t%s = 0x%04X ,\n" , enum_data.FieldName , enum_data.FieldData );
								break;
							case 4:
								m_pLog->WriteDebugLog( "\t%s = 0x%08X ,\n" , enum_data.FieldName , enum_data.FieldData );
								break;
							case 8:
								m_pLog->WriteDebugLog( "\t%s = 0x%p ,\n" , enum_data.FieldName , enum_data.FieldData );
								break;
							default:
								break;
						}
					}
				}

				m_pLog->WriteDebugLog( "};\n\n" );
			}
		}
	}
}

auto SDK_Generator::DumpAllClassList( CSchemaSystemTypeScope* Scope ) -> void
{
	auto class_container = Scope->GetClassContainer();

	if ( class_container )
	{
		int blockIndex = 0;

		for ( auto& schema_block : class_container->GetBlockContainers() )
		{
			for ( auto block = schema_block.GetFirstBlock(); block && blockIndex < class_container->GetNumSchema(); block = block->Next() , ++blockIndex )
			{
				auto binding = block->GetBinding();

				if ( !binding )
					continue;

				m_pLog->WriteDebugLog( "class %s" , binding->m_bindingName );

				if ( binding->m_baseClass_num )
				{
					auto BaseClass = binding->m_baseClass;

					if ( BaseClass )
					{
						auto pInfo = BaseClass->m_classInfo;

						m_pLog->WriteDebugLog( " : public %s" , pInfo->m_bindingName );

						int FullSizeOf = binding->m_SizeOf;
						int ThisSizeOf = pInfo->m_SizeOf;
						int StartOffset = FullSizeOf - ThisSizeOf;

						m_pLog->WriteDebugLog( " // (%i , %s , 0x%04X , 0x%04X , 0x%04X)\n{\npublic:\n" , (int)binding->m_baseClass_num , binding->m_dllName ,
												  StartOffset , ThisSizeOf , FullSizeOf );
					}
				}
				else
				{
					m_pLog->WriteDebugLog( " // (%i , %s , 0x%04X)\n{\npublic:\n" , (int)binding->m_baseClass_num , binding->m_dllName , binding->m_SizeOf );
				}

				for ( auto idx = 0; idx < binding->size; idx++ )
				{
					//auto class_ptr = &binding->m_DataArray[idx];
					auto class_data = binding->m_DataArray[idx];

					if ( class_data.FieldName && class_data.FieldType )
					{
						m_pLog->WriteDebugLog( "\t%s %s; // 0x%04X\n" , class_data.FieldType->szTypeName , class_data.FieldName , class_data.FieldOffset /*, class_ptr*/ );
					}
				}

				m_pLog->WriteDebugLog( "};\n\n" );
			}
		}
	}
}

auto SDK_Generator::DumpEnumClassOffset() -> bool
{
	auto pClassEnumFunction = (uint64_t)FindPattern( SCHEMASYSTEM_DLL , GeneratorOffset::PATTERN_ClassEnum );

	if ( pClassEnumFunction )
	{
		m_pLog->WriteDebugLog( "[SDK-Gen] pClassEnumFunction: %p\n" , pClassEnumFunction );

		auto pClassFunction = GetCallFunction( pClassEnumFunction );
		auto pEnumFunction = GetCallFunction( pClassEnumFunction + 0x12 );

		m_pLog->WriteDebugLog( "[SDK-Gen] pClassFunction: %p\n" , pClassFunction );
		m_pLog->WriteDebugLog( "[SDK-Gen] pEnumFunction: %p\n\n" , pEnumFunction );
		
		auto pClassOffset = pClassFunction + 0x4E + 0x3;
		auto pEnumOffset = pEnumFunction + 0x4E + 0x3;
		auto pNumOffset = pClassFunction + 0x17 + 0x3;

		auto GetNumSchema = *(uint8_t*)pNumOffset - *(uint8_t*)pClassOffset;
		auto GetClassContainer = *(uint32_t*)pClassOffset;
		auto GetEnumContainer = *(uint32_t*)pEnumOffset;

		auto Update = false;

		if ( GetNumSchema != GeneratorOffset::GetNumSchema )
		{
			m_pLog->WriteDebugLog( "[SDK-Gen] NEW GetNumSchema: 0x%X\n" , GetNumSchema );
			Update = true;
		}

		if ( GetClassContainer != GeneratorOffset::GetClassContainer )
		{
			m_pLog->WriteDebugLog( "[SDK-Gen] NEW GetClassContainer: 0x%X\n" , GetClassContainer );
			Update = true;
		}

		if ( GetEnumContainer != GeneratorOffset::GetEnumContainer )
		{
			m_pLog->WriteDebugLog( "[SDK-Gen] NEW GetEnumContainer: 0x%X\n" , GetEnumContainer );
			Update = true;
		}

		if ( Update )
			return false;

		m_pLog->WriteDebugLog( "[SDK-Gen] DumpEnumClassOffset OK\n\n" );

		return true;
	}
	else
	{
		m_pLog->WriteDebugLog( "[SDK-Gen] ERROR: pClassEnumFunction\n" );
	}

	return false;
}

auto SDK_Generator::Find_DotaVMT_Index() -> void
{
	for ( auto& offset : m_vDotaOffsetList )
	{
		auto pFoundCall = (uint64_t)FindPattern( offset.m_sDll.c_str() , offset.m_sOffsetPattern.c_str() );

		if ( pFoundCall )
		{
			auto pCallOffset = offset.m_bOneByteCall ? *(uint8_t*)( pFoundCall + 2 ) : *(uint32_t*)( pFoundCall + 2 );
			auto pNewIndex = pCallOffset / sizeof( uint64_t );

			if ( pNewIndex != offset.m_uDefaultIndex )
			{
				m_pLog->WriteDebugLog( "[SDK-Gen] NEW INDEX -> %s %i\n" , offset.m_sOffsetName.c_str() , pNewIndex );
			}
		}
		else
		{
			m_pLog->WriteDebugLog( "[SDK-Gen] ERROR: %s -> %s\n" , offset.m_sDll.c_str() , offset.m_sOffsetName.c_str() );
			break;
		}
	}
}