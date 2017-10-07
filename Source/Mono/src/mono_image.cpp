///////////////////////////////////////////////////////////////////////////////
// File: mono_image.h
///////////////////////////////////////////////////////////////////////////////
// Purpose: Replicates mono image functionality.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Mono includes
#include "mono_image.h"

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
DEFINE_MONO_FUNC(mono_image_open);
DEFINE_MONO_FUNC(mono_image_open_full);
DEFINE_MONO_FUNC(mono_image_open_from_data);
DEFINE_MONO_FUNC(mono_image_open_from_data_full);
DEFINE_MONO_FUNC(mono_image_get_assembly);
DEFINE_MONO_FUNC(mono_image_loaded);

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Initializes the mono image API.
// ----------------------------------------------------------------------------
bool mono_image_api_init(void)
{
    HMODULE hMono = GetModuleHandle(L"mono.dll");
    if (!hMono) {
        LogPrintf(ELogLevel::ERROR, L"Could not find address of mono.dll!\n");
        return false;
    }

    LogPrintf(ELogLevel::DEBUG, L"Found mono.dll address at: %016" PRIxPTR "\n", (uintptr_t)hMono);

    bool bResult = true;
	{
		MONO_FUNC_GET_ADDR(hMono, mono_image_open, bResult);
		MONO_FUNC_GET_ADDR(hMono, mono_image_open_full, bResult);
		MONO_FUNC_GET_ADDR(hMono, mono_image_open_from_data, bResult);
		MONO_FUNC_GET_ADDR(hMono, mono_image_open_from_data_full, bResult);
		MONO_FUNC_GET_ADDR(hMono, mono_image_get_assembly, bResult);
        MONO_FUNC_GET_ADDR(hMono, mono_image_loaded, bResult);
	}
    return bResult;
}