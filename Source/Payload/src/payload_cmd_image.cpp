///////////////////////////////////////////////////////////////////////////////
// File: payload_cmd_image.h
///////////////////////////////////////////////////////////////////////////////
// Purpose: Handles image related commands.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// OS includes
#include <Shlwapi.h>

// Payload includes
#include "payload_cmd_image.h"
#include "payload_cmd_base.h"

// Mono includes
#include "mono.h"

// Common includes
#include "common_log.h"
#include "common_string.h"

// C standard library includes
#include <stdint.h>
#include <inttypes.h>

// C++ standard library includes
#include <sstream>

///////////////////////////////////////////////////////////////////////////////
// Forward declarations
///////////////////////////////////////////////////////////////////////////////
PAYLOAD_CMD_PROC_DECL(image, dump);

///////////////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////////////
PAYLOAD_CMD_PROC_MAP_IMPL(image)
{
    PAYLOAD_CMD_PROC_MAP_ENTRY(image, dump)
};

///////////////////////////////////////////////////////////////////////////////
// Module private functions
///////////////////////////////////////////////////////////////////////////////
PAYLOAD_CMD_PROC_IMPL(image, dump)
{
    if (inTokens.size() < 2) {
        LogPrintf(ELogLevel::ERROR, L"Syntax error. Usage: image dump <image-name> <output-folder>\n");
        return false;
    }

    const std::string strImageName = StringConvertToUTF8(inTokens[0]);
    const std::wstring strImageNameW = inTokens[0];

    const std::string strOutputPath = StringConvertToUTF8(inTokens[1]);
    const std::wstring strOutputPathW = inTokens[1];

    if (PathFileExists(strOutputPathW.c_str()) == false) {
        LogPrintf(ELogLevel::ERROR, L"Output path %s is not valid!\n", strOutputPathW.c_str());
        return false;
    }

    LogPrintf(ELogLevel::INFO, L"Processing image dump command..\n");
    MonoImage* image = mono_image_loaded(strImageName.c_str());
    if (image == nullptr) {
        LogPrintf(ELogLevel::ERROR, L"Could not find image object!\n");
        return false;
    }

    // Construct the path to the file to write.
    std::wstringstream ss;
    ss << strOutputPathW;
    const std::wstring separator = L"\\";
    if (StringEndsWith(strOutputPathW, separator) == false) {
        ss << "\\";
    }
    ss << strImageNameW << L".dll";
    
    const std::wstring finalPath = ss.str();
    if (PathFileExists(finalPath.c_str())) {
        if (!DeleteFile(finalPath.c_str())) {
            LogPrintf(ELogLevel::ERROR, L"Could not delete file %s. Unable to continue.\n",
                finalPath.c_str());
            return false;
        }
    }
    
    // Write the file data out
    FILE* pFile = _wfopen(finalPath.c_str(), L"w");
    if (!pFile) {
        LogPrintf(ELogLevel::ERROR, L"Unable to open file path %s!\n", finalPath.c_str());
        return false;
    }

    const size_t nBytesWritten = fwrite(image->raw_data, sizeof(char), 
        image->raw_data_len / sizeof(char), pFile);

    LogPrintf(ELogLevel::INFO, L"Wrote %d bytes to disk.\n", nBytesWritten);
    fclose(pFile);

    LogPrintf(ELogLevel::INFO, L"\nCommand complete.\n");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Processes assembly commands
// ----------------------------------------------------------------------------
bool payload_cmd_image(std::vector<std::wstring>& vecTokens)
{
    return payload_cmd_base_exec(vecTokens, PAYLOAD_CMD_PROC_MAP_REF(image));
}