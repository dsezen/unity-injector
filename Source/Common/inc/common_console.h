///////////////////////////////////////////////////////////////////////////////
// File: common_console.h
///////////////////////////////////////////////////////////////////////////////
// Purpose: Defines console functionality shared by the injector and the
// payload.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Standard C library includes
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
// Enumerations
///////////////////////////////////////////////////////////////////////////////

/** Different colors that you can print to the console with. */
enum class EConsoleTextColor : uint8_t
{
    DEFAULT,
	RED,
	GREEN,
	BLUE,
    YELLOW,
    MAGENTA,
    CYAN
};

///////////////////////////////////////////////////////////////////////////////
// Defines
///////////////////////////////////////////////////////////////////////////////

/** Maximum number of characters per line. */
#define MAX_CONSOLE_LINE_LENGTH 1024

/** Bullshit I have to do because ELogLevel::ERROR syntax errors. */
#ifdef ERROR
#   undef ERROR
#endif

///////////////////////////////////////////////////////////////////////////////
// Module functions
///////////////////////////////////////////////////////////////////////////////

/**
 * Creates and attaches console for this process or thread.
 * @return true if the console was created successfully. False if it
 * was not.
 */
extern bool ConsoleCreate(void);

/**
 * Frees and detaches the console from the process.
 */
extern bool ConsoleDestroy(void);

/**
 * Prints a line to the currently attached console.
 * @param szFormat String containing control characters. 
 * @param ... Control character replacement strings.
 */
extern void ConsolePrintf(const wchar_t* szFormat, ...);

/**
 * Prints a line to the currently attached console. Non-vararg version.
 * @param szString The string to print.
 */
extern void ConsolePrint(const wchar_t* szString);

/**
 * Exactly like ConsolePrintf but with color.
 * @param eColor Color to print with.
 * @param szFormat String containing control characters.
 * @param ... Control character replacement strings.
 */
extern void ConsoleColorPrintf(const EConsoleTextColor& eColor,
    const wchar_t* szFormat, ...);

/**
 * Reads a line of input from the console.
 * @param szInputBuf Buffer to write the input to.
 * @param nBufSize Size of the input buffer.
 * @return Number of characters read.
 */
extern int ConsoleRead(wchar_t* szInputBuf, const size_t nBufSize);