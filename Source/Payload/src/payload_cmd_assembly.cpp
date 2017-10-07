///////////////////////////////////////////////////////////////////////////////
// File: payload_cmd_assembly.h
///////////////////////////////////////////////////////////////////////////////
// Purpose: Handles assembly related commands.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Payload includes
#include "payload_cmd_assembly.h"
#include "payload_cmd_base.h"

// Mono includes
#include "mono.h"

// Common includes
#include "common_log.h"
#include "common_string.h"

// C standard library includes
#include <stdint.h>
#include <inttypes.h>

///////////////////////////////////////////////////////////////////////////////
// Forward declarations
///////////////////////////////////////////////////////////////////////////////
PAYLOAD_CMD_PROC_DECL(assembly, list);

///////////////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////////////
PAYLOAD_CMD_PROC_MAP_IMPL(assembly)
{
    PAYLOAD_CMD_PROC_MAP_ENTRY(assembly, list)
};

///////////////////////////////////////////////////////////////////////////////
// Module private functions
///////////////////////////////////////////////////////////////////////////////
PAYLOAD_CMD_PROC_IMPL(assembly, list)
{
    LogPrintf(ELogLevel::INFO, L"Processing assembly command list..\n");

    GFunc func = [](gpointer data, gpointer user_data) {
        MonoAssembly* assembly = static_cast<MonoAssembly*>(data);
        LogPrintf(ELogLevel::INFO, L"Assembly name: %s\n", StringConvertFromUTF8(assembly->aname.name).c_str());
        LogPrintf(ELogLevel::INFO, L"\tAssembly base dir: %s\n", StringConvertFromUTF8(assembly->basedir).c_str());
        if (assembly->image) {
            if (assembly->image->name) {
                LogPrintf(ELogLevel::INFO, L"\t\tImage name: %s\n", StringConvertFromUTF8(assembly->image->module_name).c_str());
            }
        }
    };

    mono_assembly_foreach(func, nullptr);

    LogPrintf(ELogLevel::INFO, L"\nCommand complete.\n");

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Processes assembly commands
// ----------------------------------------------------------------------------
bool payload_cmd_assembly(std::vector<std::wstring>& vecTokens)
{
    return payload_cmd_base_exec(vecTokens, PAYLOAD_CMD_PROC_MAP_REF(assembly));
}