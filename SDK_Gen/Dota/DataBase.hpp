#pragma once

#define ENGINE_DLL					"engine2.dll"
#define CLIENT_DLL					"client.dll"
#define SCHEMASYSTEM_DLL			"schemasystem.dll"
#define INTERFACE_SCHEMASYSTEM		"SchemaSystem_001"

// schema_all_list_bindings
#define PATTERN_AllTypeScope		"48 8B 05 ? ? ? ? 48 8B D6 0F B7 CB 48 8B 3C C8"

namespace GeneratorOffset
{
	constexpr auto PATTERN_ClassEnum = "E8 ? ? ? ? 4C 8D 44 24 30 48 8B D6 48 8B CB 8B E8 E8 ? ? ? ?";

	constexpr auto GetNumSchema = 0xC;

	/*
	00007FFA05BB73F0 | 48:895C24 10               | mov qword ptr ss:[rsp+0x10],rbx                       |
	00007FFA05BB73F5 | 48:897424 18               | mov qword ptr ss:[rsp+0x18],rsi                       |
	00007FFA05BB73FA | 57                         | push rdi                                              |
	00007FFA05BB73FB | 41:54                      | push r12                                              |
	00007FFA05BB73FD | 41:55                      | push r13                                              |
	00007FFA05BB73FF | 41:56                      | push r14                                              | r14:"MainLoop"
	00007FFA05BB7401 | 41:57                      | push r15                                              |
	00007FFA05BB7403 | 48:83EC 20                 | sub rsp,0x20                                          |
	00007FFA05BB7407 | 48:63B1 0C050000           | movsxd rsi,dword ptr ds:[rcx+0x50C]                   | GetNumSchema Offset = GetClassContainer + Offset
	00007FFA05BB740E | 4C:8BFA                    | mov r15,rdx                                           |
	00007FFA05BB7411 | 48:8B05 40FD0200           | mov rax,qword ptr ds:[<&g_pMemAlloc>]                 |
	00007FFA05BB7418 | 48:8BF9                    | mov rdi,rcx                                           |
	00007FFA05BB741B | 49:8BD8                    | mov rbx,r8                                            |
	00007FFA05BB741E | 48:8B08                    | mov rcx,qword ptr ds:[rax]                            |
	00007FFA05BB7421 | B8 08000000                | mov eax,0x8                                           |
	00007FFA05BB7426 | 48:F7E6                    | mul rsi                                               |
	00007FFA05BB7429 | 4C:8B11                    | mov r10,qword ptr ds:[rcx]                            |
	00007FFA05BB742C | 48:C7C2 FFFFFFFF           | mov rdx,0xFFFFFFFFFFFFFFFF                            |
	00007FFA05BB7433 | 48:0F40C2                  | cmovo rax,rdx                                         |
	00007FFA05BB7437 | 48:8BD0                    | mov rdx,rax                                           |
	00007FFA05BB743A | 41:FF52 08                 | call qword ptr ds:[r10+0x8]                           |
	00007FFA05BB743E | 48:8D8F 00050000           | lea rcx,qword ptr ds:[rdi+0x500]                      | GetClassContainer
	*/
	constexpr auto GetClassContainer = 0x558;

	/*
	00007FFA05BB7550 | 48:895C24 10               | mov qword ptr ss:[rsp+0x10],rbx                       |
	00007FFA05BB7555 | 48:897424 18               | mov qword ptr ss:[rsp+0x18],rsi                       |
	00007FFA05BB755A | 57                         | push rdi                                              |
	00007FFA05BB755B | 41:54                      | push r12                                              |
	00007FFA05BB755D | 41:55                      | push r13                                              |
	00007FFA05BB755F | 41:56                      | push r14                                              | r14:"MainLoop"
	00007FFA05BB7561 | 41:57                      | push r15                                              |
	00007FFA05BB7563 | 48:83EC 20                 | sub rsp,0x20                                          |
	00007FFA05BB7567 | 48:63B1 4C250000           | movsxd rsi,dword ptr ds:[rcx+0x254C]                  | GetNumSchema Offset = GetClassContainer + Offset
	00007FFA05BB756E | 4C:8BFA                    | mov r15,rdx                                           |
	00007FFA05BB7571 | 48:8B05 E0FB0200           | mov rax,qword ptr ds:[<&g_pMemAlloc>]                 |
	00007FFA05BB7578 | 48:8BF9                    | mov rdi,rcx                                           |
	00007FFA05BB757B | 49:8BD8                    | mov rbx,r8                                            |
	00007FFA05BB757E | 48:8B08                    | mov rcx,qword ptr ds:[rax]                            |
	00007FFA05BB7581 | B8 08000000                | mov eax,0x8                                           |
	00007FFA05BB7586 | 48:F7E6                    | mul rsi                                               |
	00007FFA05BB7589 | 4C:8B11                    | mov r10,qword ptr ds:[rcx]                            |
	00007FFA05BB758C | 48:C7C2 FFFFFFFF           | mov rdx,0xFFFFFFFFFFFFFFFF                            |
	00007FFA05BB7593 | 48:0F40C2                  | cmovo rax,rdx                                         |
	00007FFA05BB7597 | 48:8BD0                    | mov rdx,rax                                           |
	00007FFA05BB759A | 41:FF52 08                 | call qword ptr ds:[r10+0x8]                           |
	00007FFA05BB759E | 48:8D8F 40250000           | lea rcx,qword ptr ds:[rdi+0x2540]                     | GetEnumContainer
	*/
	constexpr auto GetEnumContainer = 0x2D98;
}

struct VMT_IndexList_t
{
	string m_sDll = "";
	string m_sOffsetName = "";
	string m_sOffsetPattern = "";

	uint32_t m_uDefaultIndex = 0;
	bool m_bOneByteCall = false;
};

extern vector<VMT_IndexList_t> m_vDotaOffsetList;