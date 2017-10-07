///////////////////////////////////////////////////////////////////////////////
// File: common_error.h
///////////////////////////////////////////////////////////////////////////////
// Purpose: Defines error output functionality
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Common includes
#include "common_error.h"

// Standard C library includes
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Reads the error string from windows into the given buffer.
// ----------------------------------------------------------------------------
bool ErrorIdToString(wchar_t* szErrBuf, size_t nBufSize)
{
    // Sanity checking..
    const DWORD dwErrorId = GetLastError();
    if (dwErrorId == 0) {
        swprintf(szErrBuf, nBufSize, L"None");
        return false;
    }

    if (szErrBuf == NULL) {
        return false;
    }

    if (nBufSize < 1) {
        return false;
    }

    // Read the message out into a buffer.
    return FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwErrorId,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        szErrBuf,
        nBufSize,
        NULL
    );

}

// ----------------------------------------------------------------------------
// Prints an error message out to the debugger.
// ----------------------------------------------------------------------------
void ErrorPrintToDebugger(void)
{
    const size_t nBufSize = 1024;
    wchar_t szErrorBuf[nBufSize];

    if (ErrorIdToString(szErrorBuf, nBufSize)) {
        OutputDebugString(szErrorBuf);
    }
    else {
        OutputDebugString(L"Failed to print error message to debugger!\n");
    }
}