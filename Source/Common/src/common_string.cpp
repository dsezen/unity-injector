///////////////////////////////////////////////////////////////////////////////
// File: common_string.cpp
///////////////////////////////////////////////////////////////////////////////
// Purpose: Defines common string related functionality.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Common includes
#include "common_string.h"

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Converts UTF8 to Unicode.
// ----------------------------------------------------------------------------
std::wstring StringConvertFromUTF8(const std::string& utf8str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(utf8str);
}

// ----------------------------------------------------------------------------
// Converts Unicode to UTF8.
// ----------------------------------------------------------------------------
std::string StringConvertToUTF8(const std::wstring& unicodeStr)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(unicodeStr);
}

// ----------------------------------------------------------------------------
// Checks to see if a string ends with a given suffix.
// ----------------------------------------------------------------------------
bool StringEndsWith(const std::wstring &str, const std::wstring &suffix)
{
    return str.size() >= suffix.size() &&
        str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}