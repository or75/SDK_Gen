#include "Generator.hpp"

vector<OffsetList_t> m_vDotaOffsetList;

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
		m_pLog->WriteDebugLog( "[SDK-Gen] End\n" );
		delete m_pLog;
	}
}

auto SDK_Generator::Generate() -> void
{
	if ( !m_pLog )
		return;

	m_pLog->WriteDebugLog( "[SDK-Gen] Start\n" );

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

		while ( ( Scope = AllScope[idx++] ) )
		{
			DumpAllEnumList( Scope );
			DumpAllClassList( Scope );
		}
	}

#endif
}

auto SDK_Generator::DumpAllEnumList( CSchemaSystemTypeScope* Scope ) -> void
{
	auto enum_container = Scope->GetEnumContainer();

	if ( enum_container )
	{
		int blockIndex = 0;

		for ( auto& schema_block : enum_container->GetBlockContainers() )
		{
			for ( auto block = schema_block.GetFirstBlock(); block && blockIndex < enum_container->GetNumSchema(); block = block->Next() , ++blockIndex )
			{
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
					auto class_data = binding->m_DataArray[idx];

					if ( class_data.FieldName && class_data.FieldType )
					{
						m_pLog->WriteDebugLog( "\t%s %s; // 0x%04X\n" , class_data.FieldType->szTypeName , class_data.FieldName , class_data.FieldOffset );
					}
				}

				m_pLog->WriteDebugLog( "};\n\n" );
			}
		}
	}
}