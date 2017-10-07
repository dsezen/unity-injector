///////////////////////////////////////////////////////////////////////////////
// File: console_test_main.cpp
///////////////////////////////////////////////////////////////////////////////
// Purpose: Main entry point for console test program.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Common includes
#include "common_console.h"

// C++ standard library includes
#include <cstdlib>
#include <vector>
#include <string>

// Windows includes
#include <Windows.h>

///////////////////////////////////////////////////////////////////////////////
// Utility functions
///////////////////////////////////////////////////////////////////////////////

/**
 * Tokenizes the given command buffer.
 * @param buffer The buffer to tokenize.
 * @param vecOutTokens Vector to place tokens into.
 * @param delim The delimiter to tokenize on.
 * @return true if something was tokenized.
 */
bool process_cmd_tokenize(
    const std::wstring& buffer,
    std::vector<std::wstring>& vecOutTokens,
    wchar_t* delims = L" ")
{
    wchar_t* pszBuffer = _wcsdup(buffer.c_str());
    wchar_t* context = nullptr;
    wchar_t* token = wcstok_s(pszBuffer, delims, &context);

    while (token != nullptr) {
        vecOutTokens.push_back(token);
        token = wcstok_s(nullptr, delims, &context);
    }

    free(pszBuffer);
    return vecOutTokens.size() > 0;
}

///////////////////////////////////////////////////////////////////////////////
// Main program
///////////////////////////////////////////////////////////////////////////////
int wmain(int argc, wchar_t** argv)
{
	ConsoleCreate();

	wchar_t szCmdBuf[1024];
	while (true) {
        ZeroMemory(szCmdBuf, sizeof(szCmdBuf));
        if (ConsoleRead(szCmdBuf, 1024) > 0) {

            // If the command buffer is just a newline, ignore it.
            if (szCmdBuf[0] == '\n') {
                continue;
            }

            // Tokenize the command
            std::vector<std::wstring> tokens;
            if (process_cmd_tokenize(szCmdBuf, tokens)) {
                ConsolePrintf(L"There are %d tokens..\n", tokens.size());
                OutputDebugString(szCmdBuf);
            }
        }
        Sleep(1000);
	}


	system("PAUSE");
	ConsoleDestroy();

	return EXIT_SUCCESS;
}