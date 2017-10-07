///////////////////////////////////////////////////////////////////////////////
// File: mono_domain.h
///////////////////////////////////////////////////////////////////////////////
// Purpose: Replicates mono domain functionality.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Mono includes
#include "mono_domain.h"

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
DEFINE_MONO_FUNC(mono_get_root_domain);
DEFINE_MONO_FUNC(mono_thread_attach);

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Initializes the mono assembly API.
// ----------------------------------------------------------------------------
bool mono_domain_api_init(void)
{
	HMODULE hMono = GetModuleHandle(L"mono.dll");
	if (!hMono) {
		LogPrintf(ELogLevel::ERROR, L"Could not find address of mono.dll!\n");
		return false;
	}

	LogPrintf(ELogLevel::DEBUG, L"Found mono.dll address at: %016" PRIxPTR "\n", (uintptr_t)hMono);

	bool bResult = true;
	{
		MONO_FUNC_GET_ADDR(hMono, mono_get_root_domain, bResult);
		MONO_FUNC_GET_ADDR(hMono, mono_thread_attach, bResult);
	}
	return bResult;
}