///////////////////////////////////////////////////////////////////////////////
// File: payload_cmd_base.h
///////////////////////////////////////////////////////////////////////////////
// Purpose: Reusable code for defining commands.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Standard C++ library includes
#include <string>
#include <vector>
#include <map>

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

/** Payload command processing typedef. */
typedef bool(*payload_cmd_t)(std::vector<std::wstring>& inTokens);

/** Map type for dispatching command functions. */
typedef std::map<const std::wstring, payload_cmd_t> command_proc_map_t;

///////////////////////////////////////////////////////////////////////////////
// Macros
///////////////////////////////////////////////////////////////////////////////

/** Forward declares a payload command processing procedure. */
#define PAYLOAD_CMD_PROC_DECL(parent, cmd_name) \
    bool parent##_##cmd_name##_proc(std::vector<std::wstring>& inTokens);

/** Begins implementation of a payload command processing procedure. */
#define PAYLOAD_CMD_PROC_IMPL(parent, cmd_name) \
    bool parent##_##cmd_name##_proc(std::vector<std::wstring>& inTokens)

/** Reference to a command processor procedure. */
#define PAYLOAD_CMD_PROC_REF(parent, cmd_name) \
    parent##_##cmd_name##_proc(inTokens)

/** Reference name to a command map. */
#define PAYLOAD_CMD_PROC_MAP_REF(name) \
    g##name##CommandMap

/** Begins the definition of a command procedure map. */
#define PAYLOAD_CMD_PROC_MAP_IMPL(name) \
    static command_proc_map_t g##name##CommandMap =

/** Creates a map entry for a payload command processing procedure. */
#define PAYLOAD_CMD_PROC_MAP_ENTRY(parent, cmd_name) \
    {L#cmd_name, parent##_##cmd_name##_proc}
    
///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////

/**
 * Tokenizes the given command buffer.
 * @param buffer The buffer to tokenize.
 * @param vecOutTokens Vector to place tokens into.
 * @param delim The delimiter to tokenize on.
 * @return true if something was tokenized.
 */
extern bool process_cmd_base_tokenize(const std::wstring& buffer,
    std::vector<std::wstring>& vecOutTokens, wchar_t* delims = L" ");

/**
 * Given a set of command tokens plus a command dispatch map, will execute
 * the request command.
 * @param inTokens Vector of tokens to process. First token must be the command.
 * @param inProcMap Map containing command to function relationships.
 * @return true if the command was executed successfully. false if it was not.
 */
extern bool payload_cmd_base_exec(std::vector<std::wstring>& inTokens, const command_proc_map_t& inProcMap);