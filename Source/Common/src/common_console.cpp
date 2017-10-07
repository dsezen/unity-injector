///////////////////////////////////////////////////////////////////////////////
// File: common_console.cpp
///////////////////////////////////////////////////////////////////////////////
// Purpose: Defines console functionality shared by the injector and the
// payload.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Common includes
#include "common_console.h"
#include "common_error.h"

// Windows includes
#include <windows.h>

// Standard C library includes
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////////////

/** Handles for input and output. */
static HANDLE ghConsoleInput = nullptr;
static HANDLE ghConsoleOutput = nullptr;
static HANDLE ghConsoleError = nullptr;

///////////////////////////////////////////////////////////////////////////////
// Module private functions
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Returns the correct combination of console attributes to the text foreground
// color.
// ----------------------------------------------------------------------------
WORD EConsoleTextColorToTextAttribute(const EConsoleTextColor& color)
{
    WORD wResult = (WORD)0;

    switch (color)
    {
    case EConsoleTextColor::RED:
        wResult = FOREGROUND_RED;
        break;
    case EConsoleTextColor::GREEN:
        wResult = FOREGROUND_GREEN;
        break;
    case EConsoleTextColor::BLUE:
        wResult = FOREGROUND_BLUE;
        break;
    case EConsoleTextColor::YELLOW:
        wResult = FOREGROUND_RED | FOREGROUND_GREEN;
        break;
    case EConsoleTextColor::MAGENTA:
        wResult = FOREGROUND_RED | FOREGROUND_BLUE;
        break;
    case EConsoleTextColor::CYAN:
        wResult = FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    case EConsoleTextColor::DEFAULT:
    default:
        break;
    }

    // Intensify the colors.
    return wResult | FOREGROUND_INTENSITY;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Creates and attaches a console to the calling process.
// ----------------------------------------------------------------------------
bool ConsoleCreate(void)
{
	bool bResult = false;
	bool bShouldInitialize = false;

    if (GetConsoleWindow()) {
        OutputDebugString(L"WARNING: ConsoleCreate() with already attached console.\n");
        bResult = true;		// Assume good since we've got a console already.
        bShouldInitialize = true;
    }
    else {
        if (AllocConsole()) {
            if (AttachConsole(GetCurrentProcessId())) {
                OutputDebugString(L"Successfully attached console.\n");
                bResult = true;
                bShouldInitialize = true;
            }
            else {
                OutputDebugString(L"Failed to attach console.\n");
                ErrorPrintToDebugger();
                ConsoleDestroy();
                bShouldInitialize = true;
            }
        }
        else {
            OutputDebugString(L"Failed to allocate console.\n");
            ErrorPrintToDebugger();
        }
    }

	if (bShouldInitialize) {
		ghConsoleError = GetStdHandle(STD_ERROR_HANDLE);
		ghConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		ghConsoleInput = GetStdHandle(STD_INPUT_HANDLE);

        DWORD dwConsoleMode;
        if (GetConsoleMode(ghConsoleInput, &dwConsoleMode) == false) {
            OutputDebugString(L"Could not read console mode.\n");
        }
        else {
            dwConsoleMode = dwConsoleMode & (ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT);
            if (SetConsoleMode(ghConsoleInput, dwConsoleMode) == false) {
                OutputDebugString(L"Could not set console mode.\n");
                ErrorPrintToDebugger();
            }
            else {
                OutputDebugString(L"Set console mode successfully!\n");
            }
        }
	}

	return bResult;
}

// ----------------------------------------------------------------------------
// Destroys and detaches from the current console.
// ----------------------------------------------------------------------------
bool ConsoleDestroy(void)
{
	bool bResult = false;

	if (ghConsoleError || ghConsoleOutput || ghConsoleInput) {
		if (FreeConsole()) {
			OutputDebugString(L"Successfully free'd console.\n");
			bResult = true;
		}
		else {
			OutputDebugString(L"Failed to free console.\n");
            ErrorPrintToDebugger();
		}
	}
	else {
		OutputDebugString(L"ConsoleDestroy() called without active console.\n");
	}

	ghConsoleError = nullptr;
	ghConsoleOutput = nullptr;
	ghConsoleInput = nullptr;

	return bResult;
}

// ----------------------------------------------------------------------------
// Prints a line out to the console (non varargs).
// ----------------------------------------------------------------------------
void ConsolePrint(const wchar_t* szString)
{
    if (!szString) {
        OutputDebugString(L"ConsolePrint() called with an invalid string!\n");
        return;
    }

    DWORD dwCharsWritten = 0;

    if (!WriteConsole(
        ghConsoleOutput,
        (LPVOID)szString,
        (DWORD)wcslen(szString),
        &dwCharsWritten,
        NULL))
    {
        OutputDebugString(L"ConsolePrintf() failed!\n");
        ErrorPrintToDebugger();
    }
}

// ----------------------------------------------------------------------------
// Prints a line out to the console.
// ----------------------------------------------------------------------------
void ConsolePrintf(const wchar_t* szFormat, ...)
{
    wchar_t szBuffer[MAX_CONSOLE_LINE_LENGTH];

	va_list args;
	va_start(args, szFormat);
    _vsnwprintf_s(szBuffer, sizeof(szBuffer) / sizeof(wchar_t), szFormat, args);
	va_end(args);

    ConsolePrint(szBuffer);
}

// ----------------------------------------------------------------------------
// Prints a line in color to the console
// ----------------------------------------------------------------------------
void ConsoleColorPrintf(const EConsoleTextColor& eColor, const wchar_t* szFormat, ...)
{
    if (ghConsoleOutput == nullptr) {
        OutputDebugString(L"Process console handle not valid! Did you call ConsoleCreate()?\n");
        return;
    }

    // Get current text attribute (to reset later).
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(ghConsoleOutput, &csbi) == false) {
        OutputDebugString(L"Warning: Could not get console screen buffer info.\n");
        OutputDebugString(L"Warning: Console text color may be wrong.\n");
    }

    const WORD wPrevAttribs = csbi.wAttributes;
    const WORD wColor = EConsoleTextColorToTextAttribute(eColor);
    
    if (eColor != EConsoleTextColor::DEFAULT) {
        if (SetConsoleTextAttribute(ghConsoleOutput, wColor) == false) {
            OutputDebugString(L"WARNING: Could not set console color attribute.\n");
            OutputDebugString(L"WARNING: Console text color may be wrong.\n");
        }
    }

    va_list args;
    va_start(args, szFormat);
    ConsolePrintf(szFormat, args);
    va_end(args);
    
    if (eColor != EConsoleTextColor::DEFAULT) {
        if (SetConsoleTextAttribute(ghConsoleOutput, wPrevAttribs) == false) {
            OutputDebugString(L"WARNING: Could not reset console color attribute.\n");
            OutputDebugString(L"WARNING: Console text color may be wrong.\n");
        }
    }
}

// ----------------------------------------------------------------------------
// Reads out a line from the console.
// ----------------------------------------------------------------------------
int ConsoleRead(wchar_t* szInputBuf, const size_t nBufSize)
{
	if (ghConsoleInput == nullptr) {
		OutputDebugString(L"ConsoleRead() called without valid input handle.\n");
		return false;
	}

	DWORD dwInputEvents = 0;
	if (!GetNumberOfConsoleInputEvents(ghConsoleInput, &dwInputEvents)) {
		OutputDebugString(L"Could not read out console input events from handle!\n");
		return false;
	}

	// If there have been no input events, don't continue.
	if (dwInputEvents <= 0) {
		return false;
	}

    // Read out the next line.
    DWORD dwReadChars = 0;
    CONSOLE_READCONSOLE_CONTROL crcc;
    ReadConsole(ghConsoleInput, (LPVOID)szInputBuf, nBufSize, &dwReadChars, &crcc);
    return dwReadChars;
}