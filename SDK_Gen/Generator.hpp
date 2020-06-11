#pragma once

#include "Dota/SDK.hpp"
#include "Dota/Factory.hpp"
#include "Dota/DataBase.hpp"
#include "Components/DebugLog.hpp"

#define DUMP_SCHEMA_ALL 1
#define DEBUG_SCHEMA	0
#define DEBUG_LOG_FILE "debug.log"

class IndexFileGenerator;

extern CSchemaSystem* m_pSchemaSystem;

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
	namespace CVGuiPaintSurfaceMainThread
	{
		// CVGuiPaintSurfaceMainThread* = "? ? ? ? ? ? ? E8 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 49 8B 5B 30"

		constexpr auto PATTERN_DrawSetTextFont = "40 53 48 83 EC 30 89 ? ? ? ? ? 8B DA 8B ? ? ? ? ? BA ? ? ? ? FF 15 ? ? ? ?"; // function
		constexpr auto PATTERN_DrawSetColor = "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 30 8B ? ? ? ? ? 8B F2"; // function
		constexpr auto PATTERN_DrawFilledRect = "41 ? ? ? ? ? ? 80 7D 70 00 4C 8B 13 44 8B 7D 58 75 ? 8B D7 EB ? 8B 4D 30 8B D6"; // index
		constexpr auto PATTERN_DrawSetTextColor = "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 30 8B 6C 24 60"; // function
		constexpr auto PATTERN_DrawSetTextPos = "48 89 5C 24 08 57 48 83 EC 30 89 ? ? ? ? ? 8B DA ? ? ? ? ? ? ?"; // function
		constexpr auto PATTERN_DrawPrintText = "44 89 4C 24 20 44 89 44 24 18 48 89 54 24 10 55 56 57 41 54 41 55 41 56 41 57 48 ? ? ? ? ? ? 48 8D 6C 24 30"; // function 
		constexpr auto PATTERN_DrawSetTexture = "40 53 48 83 EC 20 8B ? ? ? ? ? 8B DA BA ? ? ? ? FF 15 ? ? ? ? 84 C0 74 1F"; // function
		constexpr auto PATTERN_DrawTexturedRect = "41 ? ? ? ? ? ? EB 22 8B 4D 30 44 8B C6 44 8B 4D F0 03 CE 4C 8B 13"; // index

		// "DrawSetAlphaMultiplier"
		constexpr auto DrawSetAlphaMultiplier = 3;
		// "DrawFilledRectFastFade"
		constexpr auto DrawFilledRectFastFade = 8;
		// "DrawFilledRectFade"
		constexpr auto DrawFilledRectFade = 9;
		// "DrawOutlinedCircle"
		constexpr auto DrawOutlinedCircle = 10;
		// "DrawTexturedPolyLine"
		constexpr auto DrawTexturedPolyLine = 11;
		// "DrawTexturedSubRect"
		constexpr auto DrawTexturedSubRect = 12;
		// "DrawSetTextFont"
		constexpr auto DrawSetTextFont = 13;
		// "DrawSetColor"
		constexpr auto DrawSetColor = 17;
		// "DrawFilledRect"
		constexpr auto DrawFilledRect = 19;
		// "DrawOutlinedRect" = no index
		constexpr auto DrawOutlinedRect = 21;
		// "DrawLine" = no index
		constexpr auto DrawLine = 22;
		// "DrawSetTextColor"
		constexpr auto DrawSetTextColor = 25;
		// "DrawSetTextPos"
		constexpr auto DrawSetTextPos = 26;
		// "DrawPrintText"
		constexpr auto DrawPrintText = 27;
		// "DrawUnicodeChar"
		constexpr auto DrawUnicodeChar = 28;
		// "DrawUnicodeString"
		constexpr auto DrawUnicodeString = 30;
		// "DrawFlushText"
		constexpr auto DrawFlushText = 31;
		// "DrawWordBubble"
		constexpr auto DrawWordBubble = 32;
		// "DrawSetTexture"
		constexpr auto DrawSetTexture = 33;
		// "DrawTexturedRect"
		constexpr auto DrawTexturedRect = 34;
		// "DrawTexturedRectEx"
		constexpr auto DrawTexturedRectEx = 35;
		// "DrawTexturedLine"
		constexpr auto DrawTexturedLine = 36;
		// "DrawTexturedPolygon"
		constexpr auto DrawTexturedPolygon = 38;
		// "DrawGradientFilledRect"
		constexpr auto DrawGradientFilledRect = 39;
	}
	namespace IVPanel
	{
		constexpr auto GetName = 50; // VPANEL panel + 0x130
	}
	namespace CVGuiRenderSurface
	{
		constexpr auto PATTERN_DrawSetTextureRGBA = "48 83 EC 38 8B 44 24 60 4C 8B 11 C7 44 ? ? ? ? ? ? 89 44 24 20 41 ? ? ? ? ? ? 48 83 C4 38 C3"; // function
		constexpr auto PATTERN_CreateNewTextureID = "48 83 EC 28 ? ? ? ? ? ? ? 48 8D 54 24 40 48 ? ? ? ? ? ? ? ? 48 8B 01 FF 10 48 83 C4 28 C3"; // function
		constexpr auto PATTERN_CreateFont = "48 89 5C 24 08 57 48 83 EC 20 48 63 79 28 48 8B D9 3B 79 38 75 09"; // ptr function
		constexpr auto PATTERN_SetFontGlyphSet = "48 89 5C 24 10 48 89 6C 24 18 44 89 4C 24 20 56 41 54 41 55 41 56 41 57 48 83 EC 60 48 8B E9"; // ptr function
		constexpr auto PATTERN_GetTextSize = "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 50 0F B6 44 24 48 48 8D 4C 24 30"; // function
		constexpr auto DrawSetTextureRGBA = 17;
		constexpr auto IsTextureIDValid = 21; // all return true
		constexpr auto DeleteTextureByID = 22; // all return false
		constexpr auto CreateNewTextureID = 23;
		// PTR = "48 ? ? ? ? ? ? ? 48 8B FA 48 89 74 24 70 48 8B F1 0F 84 ? ? ? ?"
		constexpr auto LoadFont = 51; // CreateFont = PTR + 0x58
		constexpr auto SetFontGlyphSet = 58; // PTR + 0xD8
		constexpr auto GetTextSize = 66; // jmp pattern
	}
	namespace ICvar
	{
		constexpr auto PATTERN_FindCommandBase = "48 83 EC 38 8B 44 24 60 4C 8B 11 C7 44 ? ? ? ? ? ? 89 44 24 20 41 ? ? ? ? ? ? 48 83 C4 38 C3"; // function
		constexpr auto PATTERN_FindVar = "40 53 48 83 EC 20 48 8B 01 FF 90 ? ? ? ? 48 8B D8 48 85 C0 75 06 48 83 C4 20 5B C3"; // function call + 0x80 FindCommandBase
		constexpr auto PATTERN_ConsoleColorPrintf = "4C 89 44 24 18 4C 89 4C 24 20 53 55 57 B8 ? ? ? ? E8 ? ? ? ?"; // function

		// ""Convars/CCvar::FindCommandBase""
		constexpr auto FindCommandBase = 16; // and 17
		constexpr auto FindVar = 18; // call FindCommandBase
		constexpr auto ConsoleColorPrintf = 27;
		constexpr auto ConsolePrintf = 28;
		constexpr auto ConsoleDPrintf = 29;
	}
	namespace IInputService
	{
		constexpr auto PATTERN_GetCursorPos = "FF 90 ? ? ? ? ? ? ? ? ? ? ? 4C 8D 44 24 24 48 8D 54 24 48 48 8B 01";

		constexpr auto GetCursorPos = 61;
	}
	namespace IGameUIService
	{
		constexpr auto PATTERN_IsConsoleVisible = "FF 90 ? ? ? ? 84 C0 75 41 ? ? ? ? ? ? ? 39 70 58 74 35";

		constexpr auto IsConsoleVisible = 27;
	}
	namespace IUIInputEngine
	{
		constexpr auto GetInputCapture = 40; // 41 all
	}
	namespace IUIEngine
	{
		constexpr auto UIInputEngine = 19; // mov rax, qword ptr ds:[rcx+0x1B0]
	}
	namespace CPanoramaUIEngine
	{
		constexpr auto AccessUIEngine = 13; // mov rax, qword ptr ds:[rcx+0x28]
	}
	namespace CDotaMapUtils
	{
		constexpr auto PATTERN_GetPositionHeight = "0F 28 C3 0F 28 CA 83 FA FF 74 25 0F B7 C2 48 8D 14 40 48 8B 41 48 48 8B 4C D0 08 48 85 C9 74 10 F3 ? ? ? ? ? ? ? 0F 28 D0 E9 ? ? ? ? 0F 57 C0 C3"; // function

		/*
		00007FFA9FA47C70 | 0F28C3                        | movaps xmm0,xmm3                                        |
		00007FFA9FA47C73 | 0F28CA                        | movaps xmm1,xmm2                                        |
		00007FFA9FA47C76 | 83FA FF                       | cmp edx,0xFFFFFFFF                                      |
		00007FFA9FA47C79 | 74 25                         | je host.7FFA9FA47CA0                                    |
		00007FFA9FA47C7B | 0FB7C2                        | movzx eax,dx                                            |
		00007FFA9FA47C7E | 48:8D1440                     | lea rdx,qword ptr ds:[rax+rax*2]                        |
		00007FFA9FA47C82 | 48:8B41 48                    | mov rax,qword ptr ds:[rcx+0x48]                         |
		00007FFA9FA47C86 | 48:8B4CD0 08                  | mov rcx,qword ptr ds:[rax+rdx*8+0x8]                    |
		00007FFA9FA47C8B | 48:85C9                       | test rcx,rcx                                            |
		00007FFA9FA47C8E | 74 10                         | je host.7FFA9FA47CA0                                    |
		00007FFA9FA47C90 | F3:0F101D 40031200            | movss xmm3,dword ptr ds:[0x7FFA9FB67FD8]                |
		00007FFA9FA47C98 | 0F28D0                        | movaps xmm2,xmm0                                        |
		00007FFA9FA47C9B | E9 C0190C00                   | jmp host.7FFA9FB09660                                   |
		00007FFA9FA47CA0 | 0F57C0                        | xorps xmm0,xmm0                                         |
		00007FFA9FA47CA3 | C3                            | ret                                                     |
		*/

		constexpr auto GetPositionHeight = 16; // 19 index = return -16384.0 default
	}
	namespace CDotaCameraManager
	{
		constexpr auto PATTERN_GetMapBounds = "FF 51 ? ? ? ? ? ? ? ? 48 8D 4C 24 60 E8 ? ? ? ? 84 C0";

		constexpr auto GetMapBounds = 4;
		constexpr auto GetAngles = 22;
		constexpr auto GetFOV = 24;
		constexpr auto GetDefaultCamera = 29;
	}
}