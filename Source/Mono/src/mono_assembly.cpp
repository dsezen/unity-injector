///////////////////////////////////////////////////////////////////////////////
// File: mono_assembly.cpp
///////////////////////////////////////////////////////////////////////////////
// Purpose: Replicates mono assembly functionality.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Mono includes
#include "mono_assembly.h"

// Windows includes
#include <Windows.h>

// C Standard Library includes
#include <stdint.h>
#include <inttypes.h>

// Common includes
#include "common_log.h"

///////////////////////////////////////////////////////////////////////////////
// Module function implementation
///////////////////////////////////////////////////////////////////////////////
DEFINE_MONO_FUNC(mono_assembly_open);
DEFINE_MONO_FUNC(mono_assembly_open_full);
DEFINE_MONO_FUNC(mono_assembly_load);
DEFINE_MONO_FUNC(mono_assembly_load_full);
DEFINE_MONO_FUNC(mono_assembly_load_from);
DEFINE_MONO_FUNC(mono_assembly_load_from_full);
DEFINE_MONO_FUNC(mono_assembly_loaded);
DEFINE_MONO_FUNC(mono_assembly_loaded_full);
DEFINE_MONO_FUNC(mono_assembly_foreach);
DEFINE_MONO_FUNC(mono_assembly_name_parse);
DEFINE_MONO_FUNC(mono_assembly_invoke_load_hook);
DEFINE_MONO_FUNC(mono_domain_assembly_open);

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Initializes the mono assembly API.
// ----------------------------------------------------------------------------
bool mono_assembly_api_init(void)
{
	HMODULE hMono = GetModuleHandle(L"mono.dll");
	if (!hMono) {
		LogPrintf(ELogLevel::ERROR, L"Could not find address of mono.dll!\n");
		return false;
	}

	LogPrintf(ELogLevel::DEBUG, L"Found mono.dll address at: %016" PRIxPTR "\n", (uintptr_t)hMono);

	bool bResult = true;
	{
		MONO_FUNC_GET_ADDR(hMono, mono_assembly_open, bResult);
		MONO_FUNC_GET_ADDR(hMono, mono_assembly_open_full, bResult);
		MONO_FUNC_GET_ADDR(hMono, mono_assembly_load, bResult);
		MONO_FUNC_GET_ADDR(hMono, mono_assembly_load_full, bResult);
		MONO_FUNC_GET_ADDR(hMono, mono_assembly_load_from, bResult);
		MONO_FUNC_GET_ADDR(hMono, mono_assembly_load_from_full, bResult);
        MONO_FUNC_GET_ADDR(hMono, mono_assembly_loaded, bResult);
        MONO_FUNC_GET_ADDR(hMono, mono_assembly_loaded_full, bResult);
        MONO_FUNC_GET_ADDR(hMono, mono_assembly_foreach, bResult);
        MONO_FUNC_GET_ADDR(hMono, mono_assembly_name_parse, bResult);
        MONO_FUNC_GET_ADDR(hMono, mono_assembly_invoke_load_hook, bResult);
		MONO_FUNC_GET_ADDR(hMono, mono_domain_assembly_open, bResult);
	}
	return bResult;
}