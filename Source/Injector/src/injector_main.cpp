///////////////////////////////////////////////////////////////////////////////
// File: injector_main.cpp
///////////////////////////////////////////////////////////////////////////////
// Purpose: Main entry point for injector program.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
// Platform includes
#include <Windows.h>

// Standard library includes
#include <stdio.h>

// Blackbone includes
#include <BlackBone/Process/Process.h>

// Common includes
#include "common_log.h"

///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////

/**
 * Ensures that required program arguments are passed in.
 * @argc Number of arguments given to program.
 * @argv Argument data.
 * @return true if the arguments are valid, false if they are not.
 */
bool CheckArgumentValidity(int argc, wchar_t** argv)
{
	// We expect two additional arguments:
	// - path to payload dll to inject
	// - name of executable process to inject into.
	if (argc < 2) {
		ConsolePrintf(L"Usage: Injector.exe C:/path/to/payload.dll executable-name.exe\n");
		return false;
	}

	if (PathFileExists(argv[1]) == false) {
        ConsolePrintf(L"Payload path %s is not a valid path.\n", argv[1]);
		return false;
	}

	return true;
}

/**
 * Cleans up the program.
 */
bool CleanUpProgram(void)
{
    return ConsoleDestroy();
}

///////////////////////////////////////////////////////////////////////////////
// Main program
///////////////////////////////////////////////////////////////////////////////
int wmain(int argc, wchar_t** argv)
{
    int                 nExitResult = EXIT_SUCCESS;
    blackbone::Process  target_process;
    std::wstring        process_name;

    if (ConsoleCreate() == false) {
        OutputDebugString(L"ConsoleCreate() call failed!\n");
        nExitResult = EXIT_FAILURE;
    }
    else {
        if (CheckArgumentValidity(argc, argv) == false) {
            LogPrintf(ELogLevel::ERROR, L"Aborting program.\n");
            nExitResult = EXIT_FAILURE;
        }
        else {
            // Look for the process with the given name.
            const wchar_t* szProcessName = argv[2];
            LogPrintf(ELogLevel::INFO, L"Searching for process: %s\n", szProcessName);

            bool bFoundProcess = false;
            DWORD dwTargetPID = 0;
            process_name = argv[2];

            while (bFoundProcess == false) {
                auto processes = blackbone::Process::EnumByName(argv[2]);
                if (processes.size() > 0) {
                    LogPrintf(ELogLevel::INFO, L"Found process %s, PID: %d\n", process_name.c_str(), processes[0]);
                    dwTargetPID = processes[0];
                    bFoundProcess = true;
                    break;
                }

                LogPrintf(ELogLevel::ERROR, L"Could not find processes named %s. Trying again..\n", process_name.c_str());
                Sleep(100);
            }

            // Attempt to attach to the process.
            NTSTATUS result = target_process.Attach(dwTargetPID);
            if (NT_SUCCESS(result) == false) {
                LogPrintf(ELogLevel::ERROR, L"Could not attach to %s!\n", process_name.c_str());
            }
            else {
                LogPrintf(ELogLevel::INFO, L"Successfully attached to process %s!\n", process_name.c_str());

                // Now that we're attached to the process, we need to inject our DLL
                auto inject_result = target_process.modules().Inject(argv[1]);
                if (inject_result) {
                    LogPrintf(ELogLevel::INFO, L"Successfully injected %s into %s.",
                        argv[1], process_name.c_str());
                }
                else {
                    LogPrintf(ELogLevel::ERROR, L"Could not inject %s in to %s!\n",
                        argv[1], process_name.c_str());
                }
            }
        }
    }

    CleanUpProgram();
	return EXIT_SUCCESS;
}