#pragma once

#include "Dota/SDK.hpp"
#include "Dota/Factory.hpp"
#include "Dota/DataBase.hpp"
#include "Components/DebugLog.hpp"

#define DUMP_SCHEMA_ALL 0
#define DEBUG_SCHEMA	0
#define DEBUG_LOG_FILE "debug.log"

class IndexFileGenerator;

class SDK_Generator
{
public:
	SDK_Generator();
	~SDK_Generator();

	auto Generate() -> void;

	auto DumpAllEnumList( CSchemaSystemTypeScope* Scope ) -> void;
	auto DumpAllClassList( CSchemaSystemTypeScope* Scope ) -> void;

	auto DumpEnumClassOffset() -> bool;

private:
	// Log
	LogFile* m_pLog = nullptr;
	// Schema System
	CSchemaSystem* m_pSchemaSystem = nullptr;

private:
	IndexFileGenerator* m_pIndexGenerator = nullptr;
};

#define INDEX_GEN_FILE "Index.hpp"

class IndexFileGenerator
{
public:
	IndexFileGenerator();
	~IndexFileGenerator();

public:
	void Generate();
	void GenerateNamespace( vector<VMT_IndexList_t>& m_vIndex , string Name );

private:
	LogFile* m_pFile = nullptr;
};

namespace index
{
	namespace IEngineClient
	{
		constexpr auto PATTERN_IsInGame = "FF 90 ? ? ? ? 3A ? ? ? ? ? 74 1F 48 8B 4E 08 ? ? ? ? ? ? ?";
		constexpr auto PATTERN_GetNetChannelInfo = "FF 90 ? ? ? ? 48 8B D8 ? ? ? ? ? ? ? 48 85 C0";
		constexpr auto PATTERN_ClientCmd_Unrestricted = "FF 90 ? ? ? ? BA ? ? ? ? 48 8B CB 48 83 C4 20 5B E9 ? ? ? ?";
		constexpr auto PATTERN_GetScreenSize = "FF 90 ? ? ? ? 66 ? ? ? ? ? ? ? F3 0F E6 C0 66 0F 6E CB";
		constexpr auto PATTERN_GetLevelName = "FF 90 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 80 38 00 0F 84 ? ? ? ? ? ? ? ? ? ? ? 48 85 C9 74";
		constexpr auto PATTERN_GetLevelNameShort = "FF 90 ? ? ? ? 4C 8B C0 ? ? ? ? ? ? ? 48 8D 4C 24 70";

		/*
		// PATTERN:
		00007FFCE3BE90DA | FF90 D0000000          | call qword ptr ds:[rax+0xD0]                                            | // offset
		00007FFCE3BE90E0 | 3A86 B8010000          | cmp al,byte ptr ds:[rsi+0x1B8]                                          |
		00007FFCE3BE90E6 | 74 1F                  | je client.7FFCE3BE9107                                                  |
		00007FFCE3BE90E8 | 48:8B4E 08             | mov rcx,qword ptr ds:[rsi+0x8]                                          |
		00007FFCE3BE90EC | 48:8D15 25AE6501       | lea rdx,qword ptr ds:[0x7FFCE5243F18]                                   | 00007FFCE5243F18:"IsInGame"
		*/
		constexpr auto IsInGame = 26;
		/*
		// PATTERN:
		00007FFAF9F07DC0:"PanoramaUI"
		00007FFAFA9838A8:"CDOTA_Hud_NetGraph::Update"
		...
		00007FFAF97AFCA8 | FF90 F0000000              | call qword ptr ds:[rax+0xF0]                          | // offset
		00007FFAF97AFCAE | 48:8BD8                    | mov rbx,rax                                           |
		00007FFAF97AFCB1 | 48:8B05 C0149401           | mov rax,qword ptr ds:[0x7FFAFB0F1178]                 |
		00007FFAF97AFCB8 | 48:85C0                    | test rax,rax                                          |
		*/
		constexpr auto GetNetChannelInfo = 28;
		/*
		// PATTERN:
		00007FFCE3969201 | 48:8B0D F08A1502       | mov rcx,qword ptr ds:[0x7FFCE5AC1CF8]                             |
		00007FFCE3969208 | 48:8D15 21603E01       | lea rdx,qword ptr ds:[0x7FFCE4D4F230]                             | 00007FFCE4D4F230:"gameui_hide"
		00007FFCE396920F | 48:8B01                | mov rax,qword ptr ds:[rcx]                                        |
		00007FFCE3969212 | FF90 00010000          | call qword ptr ds:[rax+0x100]                                     | // offset
		00007FFCE3969218 | BA 02000000            | mov edx,0x2                                                       |
		00007FFCE396921D | 48:8BCB                | mov rcx,rbx                                                       |
		00007FFCE3969220 | 48:83C4 20             | add rsp,0x20                                                      |
		00007FFCE3969224 | 5B                     | pop rbx                                                           |
		00007FFCE3969225 | E9 C6000000            | jmp <client.sub_7FFCE39692F0>                                     |
		*/
		constexpr auto ClientCmd_Unrestricted = 32;
		/*
		// PATTERN:
		00007FFCE35C50D7 | FF90 60010000          | call qword ptr ds:[rax+0x160]                                     | // offset
		00007FFCE35C50DD | 66:0F6E85 28100000     | movd xmm0,dword ptr ss:[rbp+0x1028]                               |
		00007FFCE35C50E5 | F3:0FE6C0              | cvtdq2pd xmm0,xmm0                                                |
		00007FFCE35C50E9 | 66:0F6ECB              | movd xmm1,ebx                                                     |
		*/
		constexpr auto GetScreenSize = 44;
		/*
		// FUNCTION: 48 83 EC 28 E8 ? ? ? ? 84 C0 74 0C ? ? ? ? ? ? ? 48 83 C4 28 C3
		00007FFB0A73C1B0 | 48:83EC 28                 | sub rsp,0x28                                          |
		00007FFB0A73C1B4 | E8 D7B32800                | call <engine2.sub_7FFB0A9C7590>                       |
		00007FFB0A73C1B9 | 84C0                       | test al,al                                            |
		00007FFB0A73C1BB | 74 0C                      | je engine2.7FFB0A73C1C9                               |
		00007FFB0A73C1BD | 48:8D05 7C973F00           | lea rax,qword ptr ds:[0x7FFB0AB35940]                 | 00007FFB0AB35940:"Dedicated Server"
		00007FFB0A73C1C4 | 48:83C4 28                 | add rsp,0x28                                          |
		00007FFB0A73C1C8 | C3                         | ret                                                   |
		*/
		constexpr auto GetLevelName = 47;
		/*
		00007FFCE4EB9AD4 | FF90 80010000          | call qword ptr ds:[rax+0x180]                                     | // offset
		00007FFCE4EB9ADA | 4C:8BC0                | mov r8,rax                                                        |
		00007FFCE4EB9ADD | 48:8D15 144A7C00       | lea rdx,qword ptr ds:[0x7FFCE567E4F8]                             | 00007FFCE567E4F8:"scripts/shops/%s_shops.txt"
		00007FFCE4EB9AE4 | 48:8D4C24 70           | lea rcx,qword ptr ss:[rsp+0x70]                                   |
		*/
		constexpr auto GetLevelNameShort = 48;
	}
	namespace CAnimResourceSequence
	{
		constexpr auto PATTERN_GetSequenceName = "? ? ? ? ? ? ? 48 89 41 40 48 63 73 20 3B 73 30 75 ? 48 8D 4B 28 E8 ? ? ? ? FF 43 20 48 8D 14 B6 48 8B 4B 28 48 C1 E2 04 48 03 CA 74 ? 48 89 39 48 89 79 08 48 89 79 10 66 C7 41 18 1D FF C6 41 1A 00 48 89 79 28 48 89 79 30 48 8B 4B 28 ? ? ? ? ? ? ? 48 03 CA 48 89 41 10 ? ? ? ? ? ? ? 48 89 01 ? ? ? ? ? ? ? 48 89 41 08 48 89 59 30 66 C7 41 18 1F 00";
		constexpr auto PATTERN_GetNumActivities = "? ? ? ? ? ? ? 48 89 41 40 C7 41 ? ? ? ? ? 48 63 73 20 3B 73 30 75 ? 48 8D 4B 28 E8 ? ? ? ? FF 43 20 48 8D 14 B6 48 8B 4B 28 48 C1 E2 04 48 03 CA 74 ? 48 89 39 48 89 79 08 48 89 79 10 66 C7 41 18 1D FF C6 41 1A 00 48 89 79 28 48 89 79 30 48 8B 4B 28 ? ? ? ? ? ? ? 48 03 CA 48 89 41 10 ? ? ? ? ? ? ? 48 89 01 48 89 41 08 48 89 59 30 48 0F BE 41 1A C6 41 18 1E C6 44 08 1B 05 FE 41 1A 0F BE 41 1A 83 E8 01 C6 41 19 00 48 63 D0 78 ? 66 90 80 7C 11 1B 20";
		constexpr auto PATTERN_GetActivityName = "? ? ? ? ? ? ? 48 89 41 40 48 63 73 20 3B 73 30 75 ? 48 8D 4B 28 E8 ? ? ? ? FF 43 20 48 8D 14 B6 48 8B 4B 28 48 C1 E2 04 48 03 CA 74 ? 48 89 39 48 89 79 08 48 89 79 10 66 C7 41 18 1D FF C6 41 1A 00 48 89 79 28 48 89 79 30 48 8B 4B 28 ? ? ? ? ? ? ? 48 03 CA 48 89 41 10 ? ? ? ? ? ? ? 48 89 01 48 89 41 08 48 89 59 30 48 0F BE 41 1A C6 41 18 01";
		constexpr auto PATTERN_GetActivityWeight = "? ? ? ? ? ? ? 48 89 41 40 48 63 73 20 3B 73 30 75 ? 48 8D 4B 28 E8 ? ? ? ? FF 43 20 48 8D 14 B6 48 8B 4B 28 48 C1 E2 04 48 03 CA 74 ? 48 89 39 48 89 79 08 48 89 79 10 66 C7 41 18 1D FF C6 41 1A 00 48 89 79 28 48 89 79 30 48 8B 4B 28 ? ? ? ? ? ? ? 48 03 CA 48 89 41 10 ? ? ? ? ? ? ? 48 89 01 48 89 41 08 48 89 59 30 48 0F BE 41 1A C6 41 18 05 C6 44 08 1B 05";
		constexpr auto PATTERN_GetActivityType = "? ? ? ? ? ? ? 48 89 41 40 48 63 73 20 3B 73 30 75 ? 48 8D 4B 28 E8 ? ? ? ? FF 43 20 48 8D 14 B6 48 8B 4B 28 48 C1 E2 04 48 03 CA 74 ? 48 89 39 48 89 79 08 48 89 79 10 66 C7 41 18 1D FF C6 41 1A 00 48 89 79 28 48 89 79 30 48 8B 4B 28 ? ? ? ? ? ? ? 48 03 CA 48 89 41 10 ? ? ? ? ? ? ? 48 89 01 ? ? ? ? ? ? ? 48 89 41 08 48 89 59 30 48 0F BE 41 1A C6 41 18 00";

		// Reverse GetSequenceName + CAnimResourceSequence indexes !!!
		// GetName,NumActivities,GetActivityNamemGetActivityWeight,GetActivityType string find in animationsystem.dll

		constexpr auto GetSequenceName = 5;
		constexpr auto GetNumActivities = 23;
		constexpr auto GetActivityName = 24;
		constexpr auto GetActivityWeight = 25;
		constexpr auto GetActivityType = 26;
	}
	namespace CModelAnimation
	{
		// Reverse GetSequenceName
		constexpr auto PATTERN_GetModelSequenceList = "FF 50 ? 3B 18 7D ? B8 ? ? ? ? 48 83 C4 20 5B C3";

		constexpr auto GetModelSequenceList = 12;
	}
	namespace IResourceSystem
	{
		constexpr auto PATTERN_CreateResourceManifestInternal = "FF 90 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 48 ? ? ? ? ? ? 75 ? ? ? ? ? ? ? ? 48 8D 4D 80 E8 ? ? ? ?";
		constexpr auto PATTERN_FindResourceById = "FF 90 ? ? ? ? 48 8B E8 48 85 C0 0F 84 ? ? ? ? 4C 8B 0B";
		constexpr auto PATTERN_GetNamedResourceCount = "FF 90 ? ? ? ? 8B F8 83 F8 01 7C ? 8B D0 48 8D 4C 24 40";
		constexpr auto PATTERN_GetAllNamedResourcesByType = "? ? ? ? ? ? 48 63 F8 45 8B E7 89 7C 24 30 4C 8B F7 48 89 7D 88";
		constexpr auto PATTERN_ResourceHandleToResourceId = "FF 90 ? ? ? ? ? ? ? ? ? ? ? BA ? ? ? ? 48 8B F8 8B 09";
		constexpr auto PATTERN_GetResourceTypeName = "? ? ? ? ? ? 48 89 44 24 20 45 8B CC ? ? ? ? ? ? ?";

		/*
		Find resourcesystem.dll -> "An attempt was made to create a resource manifest with debug name \"%s\" while the resource system was shutting down.\n"
		*/
		constexpr auto CreateResourceManifestInternal = 24;
		/*
		Find resourcesystem.dll -> "00007FFD168CD730:"ERROR: %s resource %016llX requested but not resident. (Missing from from a manifest?)\n""
		*/
		constexpr auto FindResourceById = 33;
		/*
		cmd -> resource_list
		*/
		constexpr auto GetNamedResourceCount = 34;
		constexpr auto GetAllNamedResourcesByType = 35;
		constexpr auto ResourceHandleToResourceId = 43;
		constexpr auto GetResourceTypeName = 45;
	}
}