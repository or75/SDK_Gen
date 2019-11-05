#pragma once

#include "Dota/SDK.hpp"
#include "Dota/Factory.hpp"
#include "Dota/DataBase.hpp"
#include "Components/DebugLog.hpp"

#define DUMP_SCHEMA_ALL 0

class SDK_Generator
{
public:
	SDK_Generator( HINSTANCE hinstDLL );
	~SDK_Generator();

	auto Generate() -> void;

	auto DumpAllEnumList( CSchemaSystemTypeScope* Scope ) -> void;
	auto DumpAllClassList( CSchemaSystemTypeScope* Scope ) -> void;

	auto DumpEnumClassOffset() -> bool;

	auto Find_DotaVMT_Index() -> void;

private:
	// Dll Directory
	string m_sDllDir = "";
	// Log
	DebugLog* m_pLog = nullptr;
	// Schema System
	CSchemaSystem* m_pSchemaSystem;
};