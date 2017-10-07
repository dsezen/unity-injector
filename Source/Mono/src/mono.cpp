///////////////////////////////////////////////////////////////////////////////
// File: mono.cpp
///////////////////////////////////////////////////////////////////////////////
// Purpose: Main gateway into mono. Initializes the API.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// OS includes
#include <Windows.h>

// Mono includes
#include "mono.h"

// Common includes
#include "common_log.h"

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Initializes the mono api
// ----------------------------------------------------------------------------
bool mono_api_init(void)
{
    // Gotta wait for mono to become available.
    HMODULE hMono = GetModuleHandle(L"mono.dll");
    while ((hMono = GetModuleHandle(L"mono.dll")) == nullptr) { Sleep(10); }

	// Initialize the API
	if (mono_image_api_init()) {
		LogPrintf(ELogLevel::INFO, L"Successfully initialized the mono image api.\n\n");
	}
	else {
		LogPrintf(ELogLevel::ERROR, L"Could not initialize the mono image api!\n\n");
		return false;
	}

	if (mono_assembly_api_init()) {
		LogPrintf(ELogLevel::INFO, L"Successfully initialized the mono assembly api.\n\n");
	}
	else {
		LogPrintf(ELogLevel::ERROR, L"Could not initialize the mono assembly api!\n\n");
		return false;
	}

	if (mono_domain_api_init()) {
		LogPrintf(ELogLevel::INFO, L"Successfully initialized the mono domain api.\n\n");
	}
	else {
		LogPrintf(ELogLevel::ERROR, L"Could not initialize the mono domain api!\n\n");
		return false;
	}

    if (mono_security_api_init()) {
        LogPrintf(ELogLevel::INFO, L"Successfully initialized the mono security api.\n\n");
    }
    else {
        LogPrintf(ELogLevel::ERROR, L"Could not initialize the mono security api!\n\n");
        return false;
    }

	// Register the calling thread with mono. This is required or else we
	// crash.
	if (mono_thread_attach(mono_get_root_domain()) != nullptr) {
		LogPrintf(ELogLevel::INFO, L"Successfully attached current thread to mono.\n");
	}
	else {
		LogPrintf(ELogLevel::ERROR, L"Could not attach current thread to mono!\n");
		return false;
	}

	return true;
}