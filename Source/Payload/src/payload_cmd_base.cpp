///////////////////////////////////////////////////////////////////////////////
// File: payload_cmd_base.cpp
///////////////////////////////////////////////////////////////////////////////
// Purpose: Reusable code for defining commands.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Payload includes
#include "payload_cmd_base.h"

// Common includes
#include "common_log.h"

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Tokenizes the given command buffer.
// ----------------------------------------------------------------------------
bool process_cmd_base_tokenize(const std::wstring& buffer,
    std::vector<std::wstring>& vecOutTokens, wchar_t* delims)
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

// ----------------------------------------------------------------------------
// Runs the given command against its given proc map.
// ----------------------------------------------------------------------------
bool payload_cmd_base_exec(std::vector<std::wstring>& inTokens,
    const command_proc_map_t& inProcMap)
{
    // We're done if there are no more commands left.
    if (inTokens.size() <= 0) {
        return true;
    }

    // Pull the command out of the tokens.
    const std::wstring& theCommand = inTokens[0];

    // Find the command function (if there is one).
    LogPrintf(ELogLevel::DEBUG, L"Looking for command dispatch for %s:\n", theCommand.c_str());
    auto findResult = inProcMap.find(theCommand);
    
    if (findResult != inProcMap.end()) {
        // Strip off the first element in the vector now that we've
        // processed it.
        if (inTokens.size() > 0) {
            inTokens.erase(inTokens.begin());
        }

        // Process the next command.
        if (findResult->second(inTokens)) {
            LogPrintf(ELogLevel::DEBUG, L"Command successfully processed.\n");
            return true;
        }
        else {
            LogPrintf(ELogLevel::DEBUG, L"Failed to process command.\n");
            return false;
        }
    }

    LogPrintf(ELogLevel::WARNING, L"Command not found.\n");
    return false;
}