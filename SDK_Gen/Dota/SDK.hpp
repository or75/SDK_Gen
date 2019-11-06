#pragma once

#include "../DllMain.hpp"
#include "../Components/Memory.hpp"
#include "DataBase.hpp"

namespace index
{
	namespace CSchemaSystemTypeScope
	{
		constexpr auto FindDeclaredClass = 2;
		constexpr auto FindDeclaredEnum = FindDeclaredClass + 1;
		constexpr auto FindSchemaTypeByName = FindDeclaredEnum + 1;
	}
	namespace CSchemaSystem
	{
		constexpr auto GlobalTypeScope = 11;
		constexpr auto FindTypeScopeForModule = 13;
	}
	namespace C_DOTAPlayer
	{
		constexpr auto PATTERN_PrepareUnitOrders = "FF 90 ? ? ? ? 45 33 C0 33 D2 48 8B CF E8 ? ? ? ? E9 ? ? ? ? CC";
		constexpr auto PATTERN_CreateMove = "FF 90 ? ? ? ? 48 8B 5C 24 38 48 8B 74 24 40 48 83 C4 20 5F C3 48 89 5C 24 10";

		/*
		// PATTERN:
		00007FFAF905AEC3 | FF90 580D0000              | call qword ptr ds:[rax+0xD58]                         | // offset
		00007FFAF905AEC9 | 45:33C0                    | xor r8d,r8d                                           |
		00007FFAF905AECC | 33D2                       | xor edx,edx                                           |
		00007FFAF905AECE | 48:8BCF                    | mov rcx,rdi                                           |
		00007FFAF905AED1 | E8 FA73FFFF                | call <client.sub_7FFAF90522D0>                        |
		00007FFAF905AED6 | E9 25FCFFFF                | jmp client.7FFAF905AB00                               |
		00007FFAF905AEDB | CC                         | int3                                                  |
		*/
		constexpr auto PrepareUnitOrders = 427;

		/*
		// PATTERN:
		00007FFAF8F6826A | FF90 200A0000              | call qword ptr ds:[rax+0xA20]                         | // offset
		00007FFAF8F68270 | 48:8B5C24 38               | mov rbx,qword ptr ss:[rsp+0x38]                       |
		00007FFAF8F68275 | 48:8B7424 40               | mov rsi,qword ptr ss:[rsp+0x40]                       |
		00007FFAF8F6827A | 48:83C4 20                 | add rsp,0x20                                          |
		00007FFAF8F6827E | 5F                         | pop rdi                                               |
		00007FFAF8F6827F | C3                         | ret                                                   |
		00007FFAF8F68280 | 48:895C24 10               | mov qword ptr ss:[rsp+0x10],rbx                       |
		// FUNCTION: 
		48 89 5C 24 18 48 89 74 24 20 57 48 83 EC 30 8B ? ? ? ? ? 48 8B DA 48 8B F1 83 F8 FD
		*/
		constexpr auto CreateMove = 324;
	}
	namespace IEngineClient
	{
		constexpr auto PATTERN_GetMaxClients = "FF 92 ? ? ? ? 83 F8 01 7E 11";
		constexpr auto PATTERN_IsInGame = "FF 90 ? ? ? ? 3A ? ? ? ? ? 74 1F 48 8B 4E 08";
		constexpr auto PATTERN_Con_NPrintf = "FF 90 ? ? ? ? 44 ? ? ? ? ? ? ? ? 44 ? ? ? ? ? ? ? ? F3 44 0F 11 54 33 30";
		constexpr auto PATTERN_GetNetChannelInfo = "FF 90 ? ? ? ? 48 8B D8 ? ? ? ? ? ? ? 48 85 C0";
		constexpr auto PATTERN_ClientCmd_Unrestricted = "FF 90 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 48 8B 01 48 83 C4 28 48 ? ? ? ? ? ? CC CC CC 48 89 5C 24 08";
		constexpr auto PATTERN_GetScreenSize = "FF 90 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 40 0F 95 C5";
		constexpr auto PATTERN_GetLevelName = "FF 90 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 80 38 00 0F 84 ? ? ? ? ? ? ? ? ? ? ? 48 85 C9 74 0A 48 8B 01";

		/*
		// PATTERN:
		00007FFB0AB47BA8:"unpause\n"
		...
		00007FFB0A79735B | FF92 C8000000              | call qword ptr ds:[rdx+0xC8]                          | [rdx+C8]:sub_7FFB0A73BF60 offset
		00007FFB0A797361 | 83F8 01                    | cmp eax,0x1                                           |
		00007FFB0A797364 | 7E 11                      | jle engine2.7FFB0A797377                              |
		00007FFB0A797366 | 48:85DB                    | test rbx,rbx                                          |
		00007FFB0A797369 | 74 47                      | je engine2.7FFB0A7973B2                               |
		00007FFB0A79736B | 48:8BCB                    | mov rcx,rbx                                           |
		*/
		constexpr auto GetMaxClients = 25;
		/*
		// PATTERN:
		00007FFAF9587EAF | FF90 D0000000              | call qword ptr ds:[rax+0xD0]                          | [rax+D0]:sub_7FFB0A73C020 offset
		00007FFAF9587EB5 | 3A86 98010000              | cmp al,byte ptr ds:[rsi+0x198]                        |
		00007FFAF9587EBB | 74 1F                      | je client.7FFAF9587EDC                                |
		00007FFAF9587EBD | 48:8B4E 08                 | mov rcx,qword ptr ds:[rsi+0x8]                        |
		00007FFAF9587EC1 | 48:8D15 50263801           | lea rdx,qword ptr ds:[0x7FFAFA90A518]                 | 00007FFAFA90A518:"IsInGame"
		00007FFAF9587EC8 | 8886 98010000              | mov byte ptr ds:[rsi+0x198],al                        |
		*/
		constexpr auto IsInGame = 26;
		/*
		// PATTERN:
		00007FFAF8F6545D | 4C:8D05 8CC61301           | lea r8,qword ptr ds:[0x7FFAFA0A1AF0]                  | 00007FFAFA0A1AF0:"Pitch: %6.1f   Yaw: %6.1f   Dist: %6.1f %16s"
		00007FFAF8F65464 | F3:0F104C33 34             | movss xmm1,dword ptr ds:[rbx+rsi+0x34]                |
		00007FFAF8F6546A | 48:8B0D CF3E1202           | mov rcx,qword ptr ds:[0x7FFAFB089340]                 |
		00007FFAF8F65471 | 48:895424 30               | mov qword ptr ss:[rsp+0x30],rdx                       |
		00007FFAF8F65476 | BA 08000000                | mov edx,0x8                                           |
		00007FFAF8F6547B | 0F5AC0                     | cvtps2pd xmm0,xmm0                                    |
		00007FFAF8F6547E | 48:8B01                    | mov rax,qword ptr ds:[rcx]                            |
		00007FFAF8F65481 | 0F5ADB                     | cvtps2pd xmm3,xmm3                                    |
		00007FFAF8F65484 | 0F5AC9                     | cvtps2pd xmm1,xmm1                                    |
		00007FFAF8F65487 | F2:0F114424 28             | movsd qword ptr ss:[rsp+0x28],xmm0                    |
		00007FFAF8F6548D | 6649:0F7ED9                | movq r9,xmm3                                          |
		00007FFAF8F65492 | F2:0F114C24 20             | movsd qword ptr ss:[rsp+0x20],xmm1                    |
		00007FFAF8F65498 | FF90 E0000000              | call qword ptr ds:[rax+0xE0]                          | // offset
		*/
		constexpr auto Con_NPrintf = 28;
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
		constexpr auto GetNetChannelInfo = 30;
		/*
		// PATTERN:
		00007FFAF8FA2501 | 48:8D15 40D21001           | lea rdx,qword ptr ds:[0x7FFAFA0AF748]                 | 00007FFAFA0AF748:"host_timescale 0"
		00007FFAF8FA2508 | 48:8B01                    | mov rax,qword ptr ds:[rcx]                            |
		00007FFAF8FA250B | FF90 20010000              | call qword ptr ds:[rax+0x120]                         |
		00007FFAF8FA2511 | 48:8B0D 286E0E02           | mov rcx,qword ptr ds:[0x7FFAFB089340]                 |
		00007FFAF8FA2518 | 48:8D15 B1D11001           | lea rdx,qword ptr ds:[0x7FFAFA0AF6D0]                 | 00007FFAFA0AF6D0:"toggleconsole"
		00007FFAF8FA251F | 48:8B01                    | mov rax,qword ptr ds:[rcx]                            |
		00007FFAF8FA2522 | 48:83C4 28                 | add rsp,0x28                                          |
		00007FFAF8FA2526 | 48:FFA0 20010000           | jmp qword ptr ds:[rax+0x120]                          | // offset
		00007FFAF8FA252D | CC                         | int3                                                  |
		00007FFAF8FA252E | CC                         | int3                                                  |
		00007FFAF8FA252F | CC                         | int3                                                  |
		00007FFAF8FA2530 | 48:895C24 08               | mov qword ptr ss:[rsp+0x8],rbx                        |
		00007FFAF8FA2535 | 57                         | push rdi                                              |
		00007FFAF8FA2536 | 48:83EC 20                 | sub rsp,0x20                                          |
		00007FFAF8FA253A | 48:8D05 67F31001           | lea rax,qword ptr ds:[<&sub_7FFAF8FA2580>]            |
		*/
		constexpr auto ClientCmd_Unrestricted = 36;
		/*
		// PATTERN:
		00007FFAF908E110 | 48:895C24 08               | mov qword ptr ss:[rsp+0x8],rbx                        |
		00007FFAF908E115 | 48:896C24 10               | mov qword ptr ss:[rsp+0x10],rbp                       |
		00007FFAF908E11A | 48:897424 18               | mov qword ptr ss:[rsp+0x18],rsi                       |
		00007FFAF908E11F | 48:897C24 20               | mov qword ptr ss:[rsp+0x20],rdi                       |
		00007FFAF908E124 | 41:56                      | push r14                                              |
		00007FFAF908E126 | 48:83EC 20                 | sub rsp,0x20                                          |
		00007FFAF908E12A | 48:8B5C24 50               | mov rbx,qword ptr ss:[rsp+0x50]                       |
		00007FFAF908E12F | 48:8BF9                    | mov rdi,rcx                                           |
		00007FFAF908E132 | 48:8B0D 07B2FF01           | mov rcx,qword ptr ds:[0x7FFAFB089340]                 |
		00007FFAF908E139 | 49:8BF0                    | mov rsi,r8                                            |
		00007FFAF908E13C | 45:33F6                    | xor r14d,r14d                                         |
		00007FFAF908E13F | 4C:8BC3                    | mov r8,rbx                                            |
		00007FFAF908E142 | 44:8932                    | mov dword ptr ds:[rdx],r14d                           |
		00007FFAF908E145 | 49:8BD1                    | mov rdx,r9                                            |
		00007FFAF908E148 | 48:8B01                    | mov rax,qword ptr ds:[rcx]                            |
		00007FFAF908E14B | FF90 70010000              | call qword ptr ds:[rax+0x170]                         | [rax+170]:sub_7FFB0A73BA20 offset
		00007FFAF908E151 | 44:3835 F829FB01           | cmp byte ptr ds:[0x7FFAFB040B50],r14b                 |
		00007FFAF908E158 | 48:8D0D 5130FB01           | lea rcx,qword ptr ds:[0x7FFAFB0411B0]                 |
		00007FFAF908E15F | 40:0F95C5                  | setne bpl                                             |
		00007FFAF908E163 | E8 080D0B00                | call <client.sub_7FFAF913EE70>                        |
		*/
		constexpr auto GetScreenSize = 46;
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
	namespace CGameEntitySystem
	{
		constexpr auto PATTERN_OnAddEntity = "FF 50 ? C6 05 ? ? ? ? ? 48 8B 0B";

		/*
		FUNCTION: 48 89 5C 24 10 56 48 83 EC 20 48 8B D9 41 8B C0 B9 ? ? ? ? 48 8B F2 23 C1 41 83 F8 FF
		00007FFAF8E88E70 | 48:895C24 10               | mov qword ptr ss:[rsp+0x10],rbx                       |
		00007FFAF8E88E75 | 56                         | push rsi                                              |
		00007FFAF8E88E76 | 48:83EC 20                 | sub rsp,0x20                                          |
		00007FFAF8E88E7A | 48:8BD9                    | mov rbx,rcx                                           |
		00007FFAF8E88E7D | 41:8BC0                    | mov eax,r8d                                           |
		00007FFAF8E88E80 | B9 FF7F0000                | mov ecx,0x7FFF                                        |
		00007FFAF8E88E85 | 48:8BF2                    | mov rsi,rdx                                           |
		00007FFAF8E88E88 | 23C1                       | and eax,ecx                                           |
		00007FFAF8E88E8A | 41:83F8 FF                 | cmp r8d,0xFFFFFFFF                                    |
		*/
		constexpr auto OnAddEntity = 14;
		constexpr auto OnRemoveEntity = OnAddEntity + 1;
	}
}

#pragma region region_schema

class CSchemaClassBinding;
class CSchemaEnumBinding;
class CSchemaType;
class CSchemaSystemTypeScope;

#pragma endregion

#pragma region region_schema_impl

struct SchemaClassFieldDataArray_t
{
	char* FieldName;
	CSchemaType* FieldType;
	int FieldOffset;
	int FieldUnk;
	void* unkn0;
};

struct SchemaEnumFieldDataArray_t
{
	char* FieldName;
	uint64_t FieldData;
	void* unkn0;
	void* unkn1;
};

class CSchemaClassBinding
{
public:
	class CBaseClass_t
	{
	public:
		void* unk;
		CSchemaClassBinding* m_classInfo;
	};
	void* pthis;
	const char* m_bindingName;
	char* m_dllName;
	int m_SizeOf;
	unsigned short int size;
	unsigned short int unkn1;
	unsigned short int unkn2;
	BYTE m_Align;
	BYTE unkn3;
	unsigned short int unkn4;
	unsigned short int m_baseClass_num;
	SchemaClassFieldDataArray_t* m_DataArray;
	void* unkn6;
	CBaseClass_t* m_baseClass;
	void* unkn7;
	void* unkn8;
	CSchemaSystemTypeScope* m_TypeScope;
	CSchemaType* m_Type;
	unsigned short int m_Flags;
};

class CSchemaEnumBinding
{
public:
	void* pthis;
	const char* m_bindingName;
	char* m_dllName;
	unsigned char m_TypeSize;
	unsigned char m_TypeAlign;
	unsigned short int unkn0;
	int size;
	SchemaEnumFieldDataArray_t* m_DataArray;
	void* unkn1;
	CSchemaSystemTypeScope* m_TypeScope;

	const char* GenerateTypeStorage()
	{
		const char* TypeStorage = nullptr;

		switch ( m_TypeSize )
		{
			case 1:
				TypeStorage = "uint8_t";
				break;
			case 2:
				TypeStorage = "uint16_t";
				break;
			case 4:
				TypeStorage = "uint32_t";
				break;
			case 8:
				TypeStorage = "uint64_t";
				break;
			default:
				TypeStorage = "INVALID_TYPE";;
				break;
		}

		return TypeStorage;
	}
};

class CSchemaType
{
public:
	char* szTypeName;
};

template <class T = CSchemaClassBinding>
class CSchemaList
{
public:
	class SchemaBlock {
	public:
		SchemaBlock* Next() const {
			return m_nextBlock;
		}
		T* GetBinding() const {
			return m_classBinding;
		}
	private:
		void* unkn0;
		SchemaBlock* m_nextBlock;
		T* m_classBinding;
	};

	class BlockContainer {
	public:
		SchemaBlock* GetFirstBlock() const {
			return m_firstBlock;
		}
	private:
		void* unkn0[3];
		SchemaBlock* m_firstBlock;
	};

	typedef array<BlockContainer , 256> BlockContainers;

	int GetNumSchema()
	{
		return CUSTOM_OFFSET( int , GeneratorOffset::GetNumSchema );
	}

	const BlockContainers& GetBlockContainers()
	{
		return CUSTOM_OFFSET( const BlockContainers , 0x38 );
	}
};

class CSchemaSystemTypeScope
{
public:
	// CFingerBone etc
	/*
	00007FFB3462E7E0 | 48:8B01                    | mov rax,qword ptr ds:[rcx]                            |
	00007FFB3462E7E3 | 48:FFA0 C8000000           | jmp qword ptr ds:[rax+0xC8]                           |

	00007FFB3462B5E0 | 40:53                      | push rbx                                              |
	00007FFB3462B5E2 | 48:83EC 20                 | sub rsp,0x20                                          |
	00007FFB3462B5E6 | 4C:8BCA                    | mov r9,rdx                                            |
	00007FFB3462B5E9 | 48:8BD9                    | mov rbx,rcx                                           |
	00007FFB3462B5EC | 48:85D2                    | test rdx,rdx                                          |
	00007FFB3462B5EF | 74 3C                      | je schemasystem.7FFB3462B62D                          |
	00007FFB3462B5F1 | 803A 00                    | cmp byte ptr ds:[rdx],0x0                             |
	00007FFB3462B5F4 | 74 37                      | je schemasystem.7FFB3462B62D                          |
	00007FFB3462B5F6 | 48:83C8 FF                 | or rax,0xFFFFFFFFFFFFFFFF                             |
	00007FFB3462B5FA | 66:0F1F4400 00             | nop word ptr ds:[rax+rax],ax                          |
	00007FFB3462B600 | 48:FFC0                    | inc rax                                               |
	00007FFB3462B603 | 803C02 00                  | cmp byte ptr ds:[rdx+rax],0x0                         |
	00007FFB3462B607 | 75 F7                      | jne schemasystem.7FFB3462B600                         |
	00007FFB3462B609 | 41:B8 EDFEADBA             | mov r8d,0xBAADFEED                                    |
	00007FFB3462B60F | 8BD0                       | mov edx,eax                                           |
	00007FFB3462B611 | 49:8BC9                    | mov rcx,r9                                            |
	00007FFB3462B614 | E8 37550200                | call <schemasystem.sub_7FFB34650B50>                  |
	00007FFB3462B619 | 8BD0                       | mov edx,eax                                           |
	00007FFB3462B61B | 48:8BCB                    | mov rcx,rbx                                           |
	00007FFB3462B61E | 48:8B03                    | mov rax,qword ptr ds:[rbx]                            |
	00007FFB3462B621 | 48:83C4 20                 | add rsp,0x20                                          |
	00007FFB3462B625 | 5B                         | pop rbx                                               |
	00007FFB3462B626 | 48:FFA0 C0000000           | jmp qword ptr ds:[rax+0xC0]                           |
	00007FFB3462B62D | 48:8B03                    | mov rax,qword ptr ds:[rbx]                            |
	00007FFB3462B630 | 33D2                       | xor edx,edx                                           |
	00007FFB3462B632 | 48:8BCB                    | mov rcx,rbx                                           |
	00007FFB3462B635 | 48:83C4 20                 | add rsp,0x20                                          |
	00007FFB3462B639 | 5B                         | pop rbx                                               |
	00007FFB3462B63A | 48:FFA0 C0000000           | jmp qword ptr ds:[rax+0xC0]                           |
	*/
	auto FindDeclaredClass( const char* name ) -> CSchemaClassBinding*
	{
		VirtualFn( CSchemaClassBinding* )( CSchemaSystemTypeScope* , const char* );
		return vget< Fn >( this , index::CSchemaSystemTypeScope::FindDeclaredClass )( this , name );
	}
	/*
	00007FFB3462E7F0 | 48:8B01                    | mov rax,qword ptr ds:[rcx]                            |
	00007FFB3462E7F3 | 48:FFA0 D8000000           | jmp qword ptr ds:[rax+0xD8]                           |

	00007FFB3462B6B0 | 40:53                      | push rbx                                              |
	00007FFB3462B6B2 | 48:83EC 20                 | sub rsp,0x20                                          |
	00007FFB3462B6B6 | 4C:8BCA                    | mov r9,rdx                                            |
	00007FFB3462B6B9 | 48:8BD9                    | mov rbx,rcx                                           |
	00007FFB3462B6BC | 48:85D2                    | test rdx,rdx                                          |
	00007FFB3462B6BF | 74 3C                      | je schemasystem.7FFB3462B6FD                          |
	00007FFB3462B6C1 | 803A 00                    | cmp byte ptr ds:[rdx],0x0                             |
	00007FFB3462B6C4 | 74 37                      | je schemasystem.7FFB3462B6FD                          |
	00007FFB3462B6C6 | 48:83C8 FF                 | or rax,0xFFFFFFFFFFFFFFFF                             |
	00007FFB3462B6CA | 66:0F1F4400 00             | nop word ptr ds:[rax+rax],ax                          |
	00007FFB3462B6D0 | 48:FFC0                    | inc rax                                               |
	00007FFB3462B6D3 | 803C02 00                  | cmp byte ptr ds:[rdx+rax],0x0                         |
	00007FFB3462B6D7 | 75 F7                      | jne schemasystem.7FFB3462B6D0                         |
	00007FFB3462B6D9 | 41:B8 EDFEADBA             | mov r8d,0xBAADFEED                                    |
	00007FFB3462B6DF | 8BD0                       | mov edx,eax                                           |
	00007FFB3462B6E1 | 49:8BC9                    | mov rcx,r9                                            |
	00007FFB3462B6E4 | E8 67540200                | call <schemasystem.sub_7FFB34650B50>                  |
	00007FFB3462B6E9 | 8BD0                       | mov edx,eax                                           |
	00007FFB3462B6EB | 48:8BCB                    | mov rcx,rbx                                           |
	00007FFB3462B6EE | 48:8B03                    | mov rax,qword ptr ds:[rbx]                            |
	00007FFB3462B6F1 | 48:83C4 20                 | add rsp,0x20                                          |
	00007FFB3462B6F5 | 5B                         | pop rbx                                               |
	00007FFB3462B6F6 | 48:FFA0 D0000000           | jmp qword ptr ds:[rax+0xD0]                           |
	00007FFB3462B6FD | 48:8B03                    | mov rax,qword ptr ds:[rbx]                            |
	00007FFB3462B700 | 33D2                       | xor edx,edx                                           |
	00007FFB3462B702 | 48:8BCB                    | mov rcx,rbx                                           |
	00007FFB3462B705 | 48:83C4 20                 | add rsp,0x20                                          |
	00007FFB3462B709 | 5B                         | pop rbx                                               |
	00007FFB3462B70A | 48:FFA0 D0000000           | jmp qword ptr ds:[rax+0xD0]                           |
	*/
	// DOTAKeybindCommand_t etc
	auto FindDeclaredEnum( const char* name ) -> CSchemaEnumBinding*
	{
		VirtualFn( CSchemaEnumBinding* )( CSchemaSystemTypeScope* , const char* );
		return vget< Fn >( this , index::CSchemaSystemTypeScope::FindDeclaredEnum )( this , name );
	}
	/*
	48 89 5C 24 08 57 48 8B DA 48 8B F9 48 85 D2 74 76
	*/
	// float32,uint32 etc
	auto FindSchemaTypeByName( const char* name ) -> CSchemaType*
	{
		VirtualFn( CSchemaType* )( CSchemaSystemTypeScope* , const char* );
		return vget< Fn >( this , index::CSchemaSystemTypeScope::FindSchemaTypeByName )( this , name );
	}

	// schema_all_list_bindings function
	// 41 8B 00 45 33 C9 83 F8 01 7E 07 ? ? ? ? ? ? ? 45 33 C0 ? ? ? ? ? ? ? E9 ? ? ? ?
	/*
	// 48 8B C4 48 83 EC 78 48 89 58 08 49 8B D8 48 89 68 10 48 89 70 18
	00007FFDBA1668B0 | 48:8BC4                     | mov rax,rsp                                             |
	00007FFDBA1668B3 | 48:83EC 78                  | sub rsp,0x78                                            |
	00007FFDBA1668B7 | 48:8958 08                  | mov qword ptr ds:[rax+0x8],rbx                          |
	00007FFDBA1668BB | 49:8BD8                     | mov rbx,r8                                              |
	00007FFDBA1668BE | 48:8968 10                  | mov qword ptr ds:[rax+0x10],rbp                         |
	00007FFDBA1668C2 | 48:8970 18                  | mov qword ptr ds:[rax+0x18],rsi                         |
	00007FFDBA1668C6 | 49:8BF1                     | mov rsi,r9                                              |
	00007FFDBA1668C9 | 4C:8960 F0                  | mov qword ptr ds:[rax-0x10],r12                         |
	00007FFDBA1668CD | 45:33E4                     | xor r12d,r12d                                           |
	00007FFDBA1668D0 | 4C:8970 E0                  | mov qword ptr ds:[rax-0x20],r14                         |
	00007FFDBA1668D4 | 4D:85C0                     | test r8,r8                                              |
	00007FFDBA1668D7 | 4C:8978 D8                  | mov qword ptr ds:[rax-0x28],r15                         |
	00007FFDBA1668DB | 4C:8D40 B8                  | lea r8,qword ptr ds:[rax-0x48]                          |
	00007FFDBA1668DF | 4C:8BFA                     | mov r15,rdx                                             |
	00007FFDBA1668E2 | 44:8960 B8                  | mov dword ptr ds:[rax-0x48],r12d                        |
	00007FFDBA1668E6 | 49:8BD1                     | mov rdx,r9                                              |
	00007FFDBA1668E9 | 4C:8960 C0                  | mov qword ptr ds:[rax-0x40],r12                         |
	00007FFDBA1668ED | 4C:8960 C8                  | mov qword ptr ds:[rax-0x38],r12                         |
	00007FFDBA1668F1 | 74 22                       | je schemasystem.7FFDBA166915                            |
	00007FFDBA1668F3 | 48:8BCB                     | mov rcx,rbx                                             |
	00007FFDBA1668F6 | E8 E5F5FFFF                 | call <schemasystem.sub_7FFDBA165EE0>                    | 0x500 = class + GetNumSchema
	00007FFDBA1668FB | 4C:8D4424 30                | lea r8,qword ptr ss:[rsp+0x30]                          |
	00007FFDBA166900 | 48:8BD6                     | mov rdx,rsi                                             |
	00007FFDBA166903 | 48:8BCB                     | mov rcx,rbx                                             |
	00007FFDBA166906 | 8BE8                        | mov ebp,eax                                             |
	00007FFDBA166908 | E8 33F7FFFF                 | call <schemasystem.sub_7FFDBA166040>                    | 0x2540 = enum + GetNumSchema
	*/

	CSchemaList<CSchemaClassBinding>* GetClassContainer()
	{
		// schema_list_bindings
		return CUSTOM_OFFSET_RAW( CSchemaList<CSchemaClassBinding> , GeneratorOffset::GetClassContainer );
	}

	CSchemaList<CSchemaEnumBinding>* GetEnumContainer()
	{
		// schema_list_bindings
		return CUSTOM_OFFSET_RAW( CSchemaList<CSchemaEnumBinding> , GeneratorOffset::GetEnumContainer );
	}
};

class CSchemaSystem
{
public:
	auto GlobalTypeScope() -> CSchemaSystemTypeScope*
	{
		VirtualFn( CSchemaSystemTypeScope* )( CSchemaSystem* );
		return vget< Fn >( this , index::CSchemaSystem::GlobalTypeScope )( this );
	}
	/*
	00007FFB3462BDE0 | 40:53                      | push rbx                                              |
	00007FFB3462BDE2 | 48:83EC 20                 | sub rsp,0x20                                          |
	00007FFB3462BDE6 | 48:8BD9                    | mov rbx,rcx                                           |
	00007FFB3462BDE9 | 4C:8BC2                    | mov r8,rdx                                            |
	00007FFB3462BDEC | 48:81C1 30440000           | add rcx,0x4430                                        |
	00007FFB3462BDF3 | 48:8D5424 30               | lea rdx,qword ptr ss:[rsp+0x30]                       |
	00007FFB3462BDF8 | E8 53580200                | call <schemasystem.sub_7FFB34651650>                  |
	00007FFB3462BDFD | 0FB708                     | movzx ecx,word ptr ds:[rax]                           |
	00007FFB3462BE00 | B8 FFFF0000                | mov eax,0xFFFF                                        |
	00007FFB3462BE05 | 66:3BC8                    | cmp cx,ax                                             |
	00007FFB3462BE08 | 75 0F                      | jne schemasystem.7FFB3462BE19                         |
	00007FFB3462BE0A | 48:8B03                    | mov rax,qword ptr ds:[rbx]                            |
	00007FFB3462BE0D | 48:8BCB                    | mov rcx,rbx                                           |
	00007FFB3462BE10 | 48:83C4 20                 | add rsp,0x20                                          |
	00007FFB3462BE14 | 5B                         | pop rbx                                               |
	00007FFB3462BE15 | 48:FF60 58                 | jmp qword ptr ds:[rax+0x58]                           |
	00007FFB3462BE19 | 48:8B83 20440000           | mov rax,qword ptr ds:[rbx+0x4420]                     |
	00007FFB3462BE20 | 0FB7C9                     | movzx ecx,cx                                          |
	00007FFB3462BE23 | 48:8B04C8                  | mov rax,qword ptr ds:[rax+rcx*8]                      |
	00007FFB3462BE27 | 48:83C4 20                 | add rsp,0x20                                          |
	00007FFB3462BE2B | 5B                         | pop rbx                                               |
	00007FFB3462BE2C | C3                         | ret                                                   |
	*/
	auto FindTypeScopeForModule( const char* szModuleName ) -> CSchemaSystemTypeScope*
	{
		VirtualFn( CSchemaSystemTypeScope* )( CSchemaSystem* , const char* );
		return vget< Fn >( this , index::CSchemaSystem::FindTypeScopeForModule )( this , szModuleName );
	}
	auto AllTypeScope() -> CSchemaSystemTypeScope**
	{
		CSchemaSystemTypeScope** pResult = nullptr;

		auto pFind = FindPattern( SCHEMASYSTEM_DLL , PATTERN_AllTypeScope , 0 );

		if ( pFind )
			pResult = *(CSchemaSystemTypeScope***)( (uintptr_t)pFind + 7 + *(uint32_t*)( (uintptr_t)pFind + 3 ) );
		else
			MessageBoxA( 0 , "AllTypeScope" , "Error" , MB_ICONERROR );

		return pResult;
	}
};