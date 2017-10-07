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

// Windows includes
#include <windows.h>

// Common includes
#include "common_log.h"
#include "common_console.h"

// Standard C library includes
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
// Module private functions
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Converts the log level to text.
// ----------------------------------------------------------------------------
const wchar_t* ELogLevelToString(const ELogLevel& eLogLevel)
{
    const wchar_t* szResult = L"(unknown)";

    switch (eLogLevel)
    {
    case ELogLevel::DEBUG:
        szResult = L"DEBUG";
        break;
    case ELogLevel::ERROR:
        szResult = L"ERROR";
        break;
    case ELogLevel::INFO:
        szResult = L"INFO";
        break;
    case ELogLevel::WARNING:
        szResult = L"WARNING";
        break;
    }

    return szResult;
}

// ----------------------------------------------------------------------------
// Returns the color for a log level.
// ----------------------------------------------------------------------------
EConsoleTextColor ELogLevelToColor(const ELogLevel& eLogLevel)
{
    EConsoleTextColor color = EConsoleTextColor::RED;
    switch (eLogLevel)
    {
    case ELogLevel::DEBUG:
        color = EConsoleTextColor::MAGENTA;
        break;
    case ELogLevel::ERROR:
        color = EConsoleTextColor::RED;
        break;
    case ELogLevel::WARNING:
        color = EConsoleTextColor::BLUE;
        break;
    case ELogLevel::INFO:
        color = EConsoleTextColor::CYAN;
        break;
    }

    return color;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Logs a message
// ----------------------------------------------------------------------------
void LogPrintf(const ELogLevel& eLogLevel, const wchar_t* szFormat, ...)
{
    const EConsoleTextColor color = ELogLevelToColor(eLogLevel);
    const wchar_t* szPrefix = ELogLevelToString(eLogLevel);
    wchar_t szBuffer[MAX_CONSOLE_LINE_LENGTH];

    // Write the log prefix and the format string into a single
    // buffer.
    va_list args;
    va_start(args, szFormat);
    _vsnwprintf_s(szBuffer, sizeof(szBuffer) / sizeof(wchar_t), szFormat, args);
    ConsoleColorPrintf(color, szBuffer);
    va_end(args);
}