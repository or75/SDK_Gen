#pragma once

#include "../DllMain.hpp"
#include "../Components/Memory.hpp"
#include "DataBase.hpp"

namespace index
{
	namespace CSchemaType
	{
		constexpr auto GetAtomicCategory = 6;
	}
	namespace CSchemaSystemTypeScope
	{
		constexpr auto FindDeclaredClass = 2;
		constexpr auto FindDeclaredEnum = FindDeclaredClass + 1;
		constexpr auto FindSchemaTypeByName = FindDeclaredEnum + 1;
		constexpr auto FindType_DeclaredClass = 21;
		constexpr auto FindType_DeclaredEnum = FindType_DeclaredClass + 1;
		constexpr auto GetScopeName = 31;
		constexpr auto IsGlobalScope = GetScopeName + 1;
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
	enum ETypeCategory
	{
		Schema_Builtin = 0 ,
		Schema_Ptr = 1 ,
		Schema_Bitfield = 2 ,
		Schema_FixedArray = 3 ,
		Schema_Atomic = 4 ,
		Schema_DeclaredClass = 5 ,
		Schema_DeclaredEnum = 6 ,
		Schema_None = 7
	};

	enum EAtomicCategory
	{
		Atomic_Basic = 0 ,
		Atomic_T = 1 ,
		Atomic_TT = 2 ,
		Atomic_I = 3 ,
		Atomic_None = 4
	};

	virtual ETypeCategory GetTypeCategory() = 0;

	auto GetAtomicCategory() -> EAtomicCategory
	{
		VirtualFn( EAtomicCategory )( CSchemaType* );
		return vget< Fn >( this , index::CSchemaType::GetAtomicCategory )( this );
	}

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
	auto FindDeclaredClass( const char* name ) -> CSchemaClassBinding*
	{
		VirtualFn( CSchemaClassBinding* )( CSchemaSystemTypeScope* , const char* );
		return vget< Fn >( this , index::CSchemaSystemTypeScope::FindDeclaredClass )( this , name );
	}
	auto FindDeclaredEnum( const char* name ) -> CSchemaEnumBinding*
	{
		VirtualFn( CSchemaEnumBinding* )( CSchemaSystemTypeScope* , const char* );
		return vget< Fn >( this , index::CSchemaSystemTypeScope::FindDeclaredEnum )( this , name );
	}
	auto FindSchemaTypeByName( const char* name ) -> CSchemaType*
	{
		VirtualFn( CSchemaType* )( CSchemaSystemTypeScope* , const char* );
		return vget< Fn >( this , index::CSchemaSystemTypeScope::FindSchemaTypeByName )( this , name );
	}
	auto FindType_DeclaredClass( const char* name ) -> CSchemaType*
	{
		VirtualFn( CSchemaType* )( CSchemaSystemTypeScope* , const char* );
		return vget< Fn >( this , index::CSchemaSystemTypeScope::FindType_DeclaredClass )( this , name );
	}
	auto FindType_DeclaredEnum( const char* name ) -> CSchemaType*
	{
		VirtualFn( CSchemaType* )( CSchemaSystemTypeScope* , const char* );
		return vget< Fn >( this , index::CSchemaSystemTypeScope::FindType_DeclaredEnum )( this , name );
	}
	auto GetScopeName() -> const char*
	{
		VirtualFn( const char* )( CSchemaSystemTypeScope* );
		return vget< Fn >( this , index::CSchemaSystemTypeScope::GetScopeName )( this );
	}
	auto IsGlobalScope() -> bool
	{
		VirtualFn( bool )( CSchemaSystemTypeScope* );
		return vget< Fn >( this , index::CSchemaSystemTypeScope::IsGlobalScope )( this );
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