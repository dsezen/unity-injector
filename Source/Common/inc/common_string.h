///////////////////////////////////////////////////////////////////////////////
// File: common_string.h
///////////////////////////////////////////////////////////////////////////////
// Purpose: Defines common string related functionality.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Standard library includes
#include <locale>
#include <codecvt>
#include <string>

///////////////////////////////////////////////////////////////////////////////
// Module functions
///////////////////////////////////////////////////////////////////////////////

/**
 * Takes a regular string and converts it to a wide char string.
 * @param utf8str UTF8 string buffer to convert.
 * @return Wide char string object.
 */
extern std::wstring StringConvertFromUTF8(const std::string& utf8str);

/**
 * Takes a wide char string and converts it to utf8.
 * @param unicodeStr UTF8 string buffer to convert.
 * @return UTF8 string object.
 */
extern std::string StringConvertToUTF8(const std::wstring& unicodeStr);

/**
 * Takes a string and a suffix. Checks to see if the given string ends with
 * the given suffix.
 * @param str The string to check.
 * @param suffix The suffix to check for.
 * @return true if the string ends with the given suffix.
 */
bool StringEndsWith(const std::wstring &str, const std::wstring &suffix);