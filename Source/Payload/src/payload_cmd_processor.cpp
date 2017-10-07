///////////////////////////////////////////////////////////////////////////////
// File: payload_cmd_processor.cpp
///////////////////////////////////////////////////////////////////////////////
// Purpose: Handles commands being sent through the console.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Payload includes
#include "payload_cmd_base.h"
#include "payload_cmd_processor.h"
#include "payload_cmd_assembly.h"
#include "payload_cmd_image.h"

// Mono includes
#include "mono.h"

// Common includes
#include "common_log.h"

// C++ standard library includes
#include <algorithm>
#include <map>
#include <string>
#include <vector>

// C standard library includes
#include <stdint.h>
#include <inttypes.h>

///////////////////////////////////////////////////////////////////////////////
// Forward declarations
///////////////////////////////////////////////////////////////////////////////
PAYLOAD_CMD_PROC_DECL(main, assembly);
PAYLOAD_CMD_PROC_DECL(main, image);

///////////////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////////////
PAYLOAD_CMD_PROC_MAP_IMPL(main_commands)
{
    PAYLOAD_CMD_PROC_MAP_ENTRY(main, assembly),
    PAYLOAD_CMD_PROC_MAP_ENTRY(main, image)
};

///////////////////////////////////////////////////////////////////////////////
// Module private functions
///////////////////////////////////////////////////////////////////////////////
PAYLOAD_CMD_PROC_IMPL(main, assembly)
{
    LogPrintf(ELogLevel::INFO, L"Processing assembly command..\n");
    return payload_cmd_assembly(inTokens);
}

PAYLOAD_CMD_PROC_IMPL(main, image)
{
    LogPrintf(ELogLevel::INFO, L"Processing image command..\n");
    return payload_cmd_image(inTokens);
}

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Main entry point for command processor.
// ----------------------------------------------------------------------------
void payload_process_cmd(const std::wstring& strCmdBuf, const size_t nCmdBufSize)
{
    // If the command buffer is just a newline, ignore it.
    if (strCmdBuf[0] == '\n') {
        return;
    }

    // If command buffer is blank, also ignore it.
    if (strCmdBuf.length() <= 1) {
        return;
    }

    // Tokenize the string.
    std::vector<std::wstring> vecTokens;
    if (process_cmd_base_tokenize(strCmdBuf, vecTokens)) {
        LogPrintf(ELogLevel::DEBUG, L"There are %d tokens..\n", vecTokens.size());
        payload_cmd_base_exec(vecTokens, PAYLOAD_CMD_PROC_MAP_REF(main_commands));
    }
}