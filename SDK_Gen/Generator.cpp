#include "Generator.hpp"

CSchemaSystem* m_pSchemaSystem = nullptr;

SDK_Generator::SDK_Generator()
{
	m_pLog = new LogFile();
	m_pLog->Create( DllDir , DEBUG_LOG_FILE );

	m_pIndexGenerator = new IndexFileGenerator();
}

SDK_Generator::~SDK_Generator()
{
	m_pSchemaSystem = nullptr;

	if ( m_pLog )
	{
		m_pLog->WriteDebugLog( "\n[SDK-Gen] End\n" );
		delete m_pLog;
		m_pLog = nullptr;
	}
}

auto SDK_Generator::Generate() -> void
{
	if ( !m_pLog )
		return;

	m_pLog->WriteDebugLog( "[SDK-Gen] Start\n\n" );

	while ( !GetModuleHandleA( "navsystem.dll" ) )
		Sleep( 1000 );

	Factory SchemaFactory( SCHEMASYSTEM_DLL );

	m_pSchemaSystem = SchemaFactory.Get<CSchemaSystem*>( INTERFACE_SCHEMASYSTEM );

	if ( !m_pSchemaSystem )
	{
		m_pLog->WriteDebugLog( "[SDK-Gen] Error m_pSchemaSystem\n" );
		return;
	}

	m_pLog->WriteDebugLog( "[SDK-Gen] m_pSchemaSystem: %p\n" , m_pSchemaSystem );

	if ( !DumpEnumClassOffset() )
		return;

	m_pIndexGenerator->Generate();

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

/*
Index.hpp
*/

vector<VMT_IndexList_t> m_IEngineClient_IndexList =
{
	{ CLIENT_DLL , "IsInGame" , index::IEngineClient::PATTERN_IsInGame, index::IEngineClient::IsInGame },
	{ CLIENT_DLL , "GetNetChannelInfo" , index::IEngineClient::PATTERN_GetNetChannelInfo, index::IEngineClient::GetNetChannelInfo },
	{ CLIENT_DLL , "ClientCmd_Unrestricted" , index::IEngineClient::PATTERN_ClientCmd_Unrestricted, index::IEngineClient::ClientCmd_Unrestricted },
	{ CLIENT_DLL , "GetScreenSize" , index::IEngineClient::PATTERN_GetScreenSize, index::IEngineClient::GetScreenSize },
	{ CLIENT_DLL , "GetLevelName" , index::IEngineClient::PATTERN_GetLevelName, index::IEngineClient::GetLevelName },
	{ CLIENT_DLL , "GetLevelNameShort" , index::IEngineClient::PATTERN_GetLevelNameShort, index::IEngineClient::GetLevelNameShort },
};

vector<VMT_IndexList_t> m_CAnimResourceSequence_IndexList =
{
	{ ANIMATIONSYSTEM_DLL , "GetSequenceName" , index::CAnimResourceSequence::PATTERN_GetSequenceName , index::CAnimResourceSequence::GetSequenceName , true , true } ,
	{ ANIMATIONSYSTEM_DLL , "GetNumActivities" , index::CAnimResourceSequence::PATTERN_GetNumActivities , index::CAnimResourceSequence::GetNumActivities , true } ,
	{ ANIMATIONSYSTEM_DLL , "GetActivityName" , index::CAnimResourceSequence::PATTERN_GetActivityName , index::CAnimResourceSequence::GetActivityName , true } ,
	{ ANIMATIONSYSTEM_DLL , "GetActivityWeight" , index::CAnimResourceSequence::PATTERN_GetActivityWeight , index::CAnimResourceSequence::GetActivityWeight , true } ,
	{ ANIMATIONSYSTEM_DLL , "GetActivityType" , index::CAnimResourceSequence::PATTERN_GetActivityType , index::CAnimResourceSequence::GetActivityType , true } ,
};

vector<VMT_IndexList_t> m_CModelAnimation_IndexList =
{
	{ CLIENT_DLL , "GetModelSequenceList" , index::CModelAnimation::PATTERN_GetModelSequenceList , index::CModelAnimation::GetModelSequenceList , false , true }
};

vector<VMT_IndexList_t> m_IResourceSystem_IndexList =
{
	{ PANORAMA_DLL , "CreateResourceManifestInternal" , index::IResourceSystem::PATTERN_CreateResourceManifestInternal , index::IResourceSystem::CreateResourceManifestInternal } ,
	{ ENGINE_DLL , "FindResourceById" , index::IResourceSystem::PATTERN_FindResourceById , index::IResourceSystem::FindResourceById } ,
	{ RESOURCESYSTEM_DLL , "GetNamedResourceCount" , index::IResourceSystem::PATTERN_GetNamedResourceCount , index::IResourceSystem::GetNamedResourceCount } ,
	{ RESOURCESYSTEM_DLL , "GetAllNamedResourcesByType" , index::IResourceSystem::PATTERN_GetAllNamedResourcesByType , index::IResourceSystem::GetAllNamedResourcesByType } ,
	{ RESOURCESYSTEM_DLL , "ResourceHandleToResourceId" , index::IResourceSystem::PATTERN_ResourceHandleToResourceId , index::IResourceSystem::ResourceHandleToResourceId } ,
	{ RESOURCESYSTEM_DLL , "GetResourceTypeName" , index::IResourceSystem::PATTERN_GetResourceTypeName , index::IResourceSystem::GetResourceTypeName } ,
};

vector<VMT_IndexList_t> m_IInputService_IndexList =
{
	{ CLIENT_DLL , "GetCursorPos" , index::IInputService::PATTERN_GetCursorPos , index::IInputService::GetCursorPos }
};

vector<VMT_IndexList_t> m_IGameUIService_IndexList =
{
	{ CLIENT_DLL , "IsConsoleVisible" , index::IGameUIService::PATTERN_IsConsoleVisible , index::IGameUIService::IsConsoleVisible }
};

vector<VMT_IndexList_t> m_CDotaCameraManager_IndexList =
{
	{ CLIENT_DLL , "GetMapBounds" , index::CDotaCameraManager::PATTERN_GetMapBounds , index::CDotaCameraManager::GetMapBounds , false , true } ,
	{ CLIENT_DLL , "GetAngles" , index::CDotaCameraManager::PATTERN_GetAngles , index::CDotaCameraManager::GetAngles } ,
	{ CLIENT_DLL , "GetFOV" , index::CDotaCameraManager::PATTERN_GetFOV , index::CDotaCameraManager::GetFOV } ,
	{ CLIENT_DLL , "GetDefaultCamera" , index::CDotaCameraManager::PATTERN_GetDefaultCamera , index::CDotaCameraManager::GetDefaultCamera } ,
};

IndexFileGenerator::IndexFileGenerator()
{
	m_pFile = new LogFile();
	m_pFile->Create( DllDir + "Generated" , INDEX_GEN_FILE );
}

IndexFileGenerator::~IndexFileGenerator()
{
	delete m_pFile;
}

void IndexFileGenerator::Generate()
{
	m_pFile->WriteDebugLog( "#pragma once\n\n" );
	m_pFile->WriteDebugLog( "namespace index\n{\n\t" );

	m_pFile->WriteDebugLog( "namespace SchemaBase\n\t{\n\t\tconstexpr auto GetClientClass = 30;\n\t}\n" );

	GenerateNamespace( m_IEngineClient_IndexList , "IEngineClient" );
	GenerateNamespace( m_CAnimResourceSequence_IndexList , "CAnimResourceSequence" );
	GenerateNamespace( m_CModelAnimation_IndexList , "CModelAnimation" );
	GenerateNamespace( m_IResourceSystem_IndexList , "IResourceSystem" );
	GenerateNamespace( m_IInputService_IndexList , "IInputService" );
	GenerateNamespace( m_IGameUIService_IndexList , "IGameUIService" );
	GenerateNamespace( m_CDotaCameraManager_IndexList , "CDotaCameraManager" );

	m_pFile->WriteDebugLog( "}" );
}

void IndexFileGenerator::GenerateNamespace( vector<VMT_IndexList_t>& m_vIndex , string Name )
{
	m_pFile->WriteDebugLog( "\tnamespace %s\n\t{\n" , Name.c_str() );

	auto WriteNewIndex = [&]( VMT_IndexList_t& offset , uint64_t address )
	{
		auto pCallOffset = offset.m_bOneByteCall ? *(uint8_t*)( address + 2 ) : *(int32_t*)( address + 2 );
		auto pNewIndex = pCallOffset / sizeof( int64_t );

		if ( pNewIndex != offset.m_uDefaultIndex )
			m_pFile->WriteDebugLog( "\t\tconstexpr auto %s = %i; // new\n" , offset.m_sOffsetName.c_str() , pNewIndex );
		else
			m_pFile->WriteDebugLog( "\t\tconstexpr auto %s = %i;\n" , offset.m_sOffsetName.c_str() , offset.m_uDefaultIndex );
	};

	for ( auto& offset : m_vIndex )
	{
		auto pFoundCall = (uint64_t)FindPattern( offset.m_sDll.c_str() , offset.m_sOffsetPattern.c_str() );

		if ( pFoundCall )
		{
			if ( offset.m_bJmpRaxPtr )
			{
				uint64_t pPointerJmpRax = ( pFoundCall + 7 ) + *(int32_t*)( pFoundCall + 3 ) + 3; // add offset mov 

				WriteNewIndex( offset , pPointerJmpRax );
			}
			else
			{
				WriteNewIndex( offset , pFoundCall );
			}
		}
		else
		{
			m_pFile->WriteDebugLog( "\t\tconstexpr auto %s = 0; // error find\n" , offset.m_sOffsetName.c_str() );
		}
	}

	m_pFile->WriteDebugLog( "\t}\n" );
}