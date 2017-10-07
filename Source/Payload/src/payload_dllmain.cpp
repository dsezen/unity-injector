///////////////////////////////////////////////////////////////////////////////
// File: payload_dllmain.cpp
///////////////////////////////////////////////////////////////////////////////
// Purpose: This is the main entry point for the payload.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
// Platform includes
#include <Windows.h>

// C Standard library includes
#include <stdio.h>

// C++ standard library includes
#include <algorithm>

// Common includes
#include "common_log.h"
#include "common_error.h"
#include "common_console.h"

// Mono includes
#include "mono.h"

// Payload includes
#include "payload_cmd_processor.h"
#include "payload_customs.h"

///////////////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////////////

/** Whether or not we were able to initialize the mono api. */
bool	gbMonoInitialized = false;

/** Command processor thread. */
HANDLE	ghCmdProcessorThread = NULL;

///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////

/**
 * This processes commands coming in from the user.
 */
void CmdProcessThread(void)
{
	LogPrintf(ELogLevel::DEBUG, L"CmdProcessThread started.\n");
 
    // Register this thread with mono, otherwise we can't make
    // mono thread calls.
    mono_thread_attach(mono_get_root_domain());

    // Create command buffer object
    std::wstring strCmdBuf; const size_t nCmdBufSize = 1024;
    strCmdBuf.resize(nCmdBufSize);

	// Loop forever until we're killed by the dll unload process.
	while (true) {
        if (ConsoleRead(&strCmdBuf[0], nCmdBufSize) > 0) {
            // Convert the command buffer to a string. Strip off the newline and return characters.
            strCmdBuf.erase(std::remove(strCmdBuf.begin(), strCmdBuf.end(), L'\r'), strCmdBuf.end());
            strCmdBuf.erase(std::remove(strCmdBuf.begin(), strCmdBuf.end(), L'\n'), strCmdBuf.end());

            // Process the command.
            payload_process_cmd(strCmdBuf, nCmdBufSize);
            strCmdBuf.clear();
        }

		Sleep(500);
	}
}

/**
 * Called when this DLL is loaded.
 */
void OnAttach(void)
{
    ConsoleCreate();

	// Setup mono
    LogPrintf(ELogLevel::INFO, L"Successfully attached console to process.\n");
    LogPrintf(ELogLevel::INFO, L"\n");
    LogPrintf(ELogLevel::INFO, L"=== INITIALIZING FUNCTIONS ===\n\n");
    if (mono_api_init()) {
        LogPrintf(ELogLevel::INFO, L"=== INITIALIZATION COMPLETE ===\n\n");
        gbMonoInitialized = true;
    }
    else {
        LogPrintf(ELogLevel::ERROR, L"=== INITIALIZATION FAILED ===\n\n");
    }

    LogPrintf(ELogLevel::INFO, L"=== Loading customs.. ===\n\n");
    if (payload_customs_init()) {
        LogPrintf(ELogLevel::INFO, L"=== Customs loaded successfully ===\n\n");
    }
    else {
        LogPrintf(ELogLevel::ERROR, L"=== Customs failed to load ===\n\n");
    }

	// Setup command processor thread
	ghCmdProcessorThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CmdProcessThread, NULL, NULL, NULL);
	if (ghCmdProcessorThread) {
		if (ResumeThread(ghCmdProcessorThread) != (DWORD)-1) {
			LogPrintf(ELogLevel::INFO, L"Started command input processor thread.\n");
		}
		else {
			LogPrintf(ELogLevel::ERROR, L"Failed to create command processor thread.\n");
			const size_t nErrorBufSize = 1024;
			wchar_t szErrBuf[nErrorBufSize];
			if (ErrorIdToString(szErrBuf, nErrorBufSize)) {
				LogPrintf(ELogLevel::ERROR, szErrBuf);
			}
			else {
				LogPrintf(ELogLevel::ERROR, L"Unable to extended error information.\n");
			}
		}
	}
}

/**
 * Called when this DLL is unloaded.
 */
void OnDetach(void)
{
	LogPrintf(ELogLevel::INFO, L"Unloading DLL..\n");

	if (ghCmdProcessorThread) {
		if (TerminateThread(ghCmdProcessorThread, 0)) {
			LogPrintf(ELogLevel::INFO, L"Successfully shut down command processor thread.\n");
		}
		else {
			LogPrintf(ELogLevel::ERROR, L"Failed to shut down command processor thread!\n");
		}
	}

	LogPrintf(ELogLevel::INFO, L"Destroying the console..\n");
	if (ConsoleDestroy()) {
		OutputDebugString(L"Console destroyed.\n");
	}
	else {
		OutputDebugString(L"Failed to destroy console!\n");
	}
}

///////////////////////////////////////////////////////////////////////////////
// Main program
///////////////////////////////////////////////////////////////////////////////
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OnAttach, NULL, NULL, NULL);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}