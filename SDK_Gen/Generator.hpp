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
		constexpr auto PATTERN_GetLevelName = "FF 90 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 80 38 00 0F 84 ? ? ? ? ? ? ? ? ? ? ? 48 85 C9 74 0A";

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
		constexpr auto GetLevelName = 49;
		constexpr auto GetLevelNameShort = GetLevelName + 1;
	}
}