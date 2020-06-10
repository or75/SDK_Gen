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
	void* ptable;
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
		void* unkn0[4];
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