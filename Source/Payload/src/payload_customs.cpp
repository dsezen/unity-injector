///////////////////////////////////////////////////////////////////////////////
// File: payload_customs.cpp
///////////////////////////////////////////////////////////////////////////////
// Purpose: Handles loading of custom modules.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// OS includes
#include <Windows.h>
#include <Shlwapi.h>

// MinHook includes
#include "MinHook.h"

// Payload includes
#include "payload_customs.h"

// Common includes
#include "common_log.h"
#include "common_error.h"
#include "common_string.h"

// Mono includes
#include "mono.h"

// C++ standard library includes
#include <vector>
#include <string>
#include <map>

// C standard library includes
#include <stdint.h>
#include <inttypes.h>

///////////////////////////////////////////////////////////////////////////////
// Defines
///////////////////////////////////////////////////////////////////////////////

#define PAYLOAD_HOOK_CALL_ORIG(mono_func_name, ...) \
    mono_func_name##_orig(__VA_ARGS__)

#define PAYLOAD_HOOK_IMPL(mono_func_name, ret_type, ...) \
    mono_func_name##_t mono_func_name##_orig = nullptr; \
    ret_type mono_func_name##_hook(__VA_ARGS__)

#define PAYLOAD_HOOK_CREATE(result, hMono, mono_func_name) \
    result = result && (MH_CreateHook(\
        (LPVOID)GetProcAddress(hMono, #mono_func_name), \
        (LPVOID)mono_func_name##_hook, \
        (LPVOID*)&mono_func_name##_orig) == MH_OK);

///////////////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////////////

/** Maps assembly names to file paths. */
// std::map<std::wstring, std::wstring> gAsmNameToPathMap;
std::map<std::wstring, MonoAssembly*> gMappedAssemblies;

///////////////////////////////////////////////////////////////////////////////
// Hook redirects
///////////////////////////////////////////////////////////////////////////////
PAYLOAD_HOOK_IMPL(mono_assembly_load_full, MonoAssembly*, MonoAssemblyName* aname,
	const char* basedir, MonoImageOpenStatus* status, gboolean refonly)
{
	LogPrintf(ELogLevel::INFO, L"mono_assembly_load_full called.\n");

	// Debug stuff
	if (aname) {
		if (aname->name) {
			LogPrintf(ELogLevel::INFO, L"\tAssembly name: %s\n", StringConvertFromUTF8(aname->name).c_str());
		}
		if (basedir) {
			//LogPrintf(ELogLevel::INFO, L"\tAssembly basedir: %s\n", StringConvertFromUTF8(basedir).c_str());
		}
	}

	// Return the mapped assembly if we have one already.
	if (aname) {
		if (aname->name) {
			const std::wstring sz_assembly_name = StringConvertFromUTF8(aname->name);
			LogPrintf(ELogLevel::INFO, L"\tChecking to see if assembly %s is in cache.\n", sz_assembly_name.c_str());
			auto it = gMappedAssemblies.find(sz_assembly_name);
			if (it != gMappedAssemblies.end()) {
				LogPrintf(ELogLevel::INFO, L"\tFound assembly in cache. Returning replacement.\n");
				return it->second;
			}
		}
	}

	// Otherwise, return the original assembly.
	return PAYLOAD_HOOK_CALL_ORIG(mono_assembly_load_full, aname, basedir, status, refonly);
}

PAYLOAD_HOOK_IMPL(mono_image_open_full, MonoImage*, const char* fname, MonoImageOpenStatus* status, gboolean refonly)
{
    LogPrintf(ELogLevel::INFO, L"mono_image_open_full called %s.\n", StringConvertFromUTF8(fname).c_str());
    return PAYLOAD_HOOK_CALL_ORIG(mono_image_open_full, fname, status, refonly);
}

PAYLOAD_HOOK_IMPL(mono_assembly_load, MonoAssembly*, MonoAssemblyName* aname,
    const char* basedir, MonoImageOpenStatus* status)
{
    LogPrintf(ELogLevel::INFO, L"mono_assembly_load called.\n");

    // Debug stuff
    if (aname) {
        if (aname->name) {
            LogPrintf(ELogLevel::INFO, L"\tAssembly name: %s\n", StringConvertFromUTF8(aname->name).c_str());
        }
        if (basedir) {
            LogPrintf(ELogLevel::INFO, L"\tAssembly basedir: %s\n", StringConvertFromUTF8(basedir).c_str());
        }
    }

    // Return the mapped assembly if we have one already.
    if (aname) {
        if (aname->name) {
            const std::wstring sz_assembly_name = StringConvertFromUTF8(aname->name);
            LogPrintf(ELogLevel::INFO, L"\tChecking for assembly %s replacement.\n",
                sz_assembly_name.c_str());
            
            auto it = gMappedAssemblies.find(sz_assembly_name);
            if (it != gMappedAssemblies.end()) {
                LogPrintf(ELogLevel::INFO, L"\tASSEMBLY REPLACED!\n");
                return it->second;
            }
        }
    }

    // Otherwise, return the original assembly.
    return PAYLOAD_HOOK_CALL_ORIG(mono_assembly_load, aname, basedir, status);
}

///////////////////////////////////////////////////////////////////////////////
// Private functions
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Gets a list of files in the customs directory.
// ----------------------------------------------------------------------------
bool payload_customs_get_files(const std::wstring& dir, std::vector<std::wstring>& outFiles)
{
    if (PathFileExists(dir.c_str()) == false) {
        LogPrintf(ELogLevel::ERROR, L"Custom directory path %s does not exist!\n",
            dir.c_str());
        return false;
    }

    // Setup the search parameters.
    LogPrintf(ELogLevel::DEBUG, L"Checking folder %s for custom assemblies.\n", dir.c_str());
    WIN32_FIND_DATA fdata; HANDLE fHandle; bool bResult = true;
    ZeroMemory(&fdata, sizeof(WIN32_FIND_DATA));
    std::wstring search_dir = dir;
    search_dir.append(L"*.dll");

    fHandle = FindFirstFile(search_dir.c_str(), &fdata);
    do
    {
        LogPrintf(ELogLevel::DEBUG, L"Found match: %s, %s\n", fdata.cFileName, fdata.cAlternateFileName);
        outFiles.push_back(fdata.cFileName);
    } while (FindNextFile(fHandle, &fdata) != 0);

    FindClose(fHandle);

    return outFiles.size() > 0;
}

// ----------------------------------------------------------------------------
// Hooks functions required for assembly loading.
// ----------------------------------------------------------------------------
bool payload_customs_do_hooks(void)
{
    bool bResult = true;

    // Wait for mono to load.
    HMODULE hMono = NULL;
    while ((hMono = GetModuleHandle(L"mono.dll")) == NULL) Sleep(50);

    // Do the hooks.
    PAYLOAD_HOOK_CREATE(bResult, hMono, mono_assembly_load);
	PAYLOAD_HOOK_CREATE(bResult, hMono, mono_image_open_full);
	PAYLOAD_HOOK_CREATE(bResult, hMono, mono_assembly_load_full);
    bResult = bResult && (MH_EnableHook(MH_ALL_HOOKS) == MH_OK);

    return bResult;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Initializes the customs package.
// ----------------------------------------------------------------------------
bool payload_customs_init(void)
{
    // REPLACE THIS WITH THE PATH TO WHERE YOUR CUSTOM UNITY ASSEMBLIES ARE
    static const std::wstring customs_dir = L"";

    // Initialize minhook
    if (MH_Initialize() != MH_OK) {
        LogPrintf(ELogLevel::ERROR, L"Could not initialize MinHook.\n");
        return false;
    }

    // Disable security
    mono_security_set_mode(MONO_SECURITY_MODE_NONE);

    // Read out all the custom assemblies from this directory.
    std::vector<std::wstring> custom_assemblies;
    if (payload_customs_get_files(customs_dir, custom_assemblies) == false) {
        LogPrintf(ELogLevel::ERROR, L"Could not read out files from customs directory %s.\n",
            customs_dir.c_str());
        return false;
    }

    // Collect all the assemblies.
    for (const std::wstring& assembly_file : custom_assemblies) {
        // Construct path
        const std::wstring sz_assembly_path = customs_dir + assembly_file;

        // Load the assembly.
        MonoAssemblyName aname; MonoImageOpenStatus status;
        if (mono_assembly_name_parse(StringConvertToUTF8(assembly_file).c_str(), &aname)) {
            MonoAssembly* assembly = mono_assembly_load_full(
                &aname, 
                StringConvertToUTF8(customs_dir).c_str(),
                &status,
                false);

            if (assembly) {
                /*LogPrintf(ELogLevel::INFO, L"Successfully loaded custom assembly %s.\n", sz_assembly_path.c_str());
                LogPrintf(ELogLevel::DEBUG, L"\tImage: %016" PRIxPTR "\n", (uintptr_t)assembly->image);
                LogPrintf(ELogLevel::DEBUG, L"\tImage module name: %s\n", StringConvertFromUTF8(assembly->image->module_name).c_str());*/
                
				std::wstring assembly_name = StringConvertFromUTF8(assembly->aname.name);
				LogPrintf(ELogLevel::INFO, L"Adding assembly %s to mapped assembly cache.\n", assembly_name.c_str());
                gMappedAssemblies.insert(std::make_pair(assembly_name, assembly));
            }
            else {
                LogPrintf(ELogLevel::ERROR, L"Could not load assembly %s.\n", sz_assembly_path.c_str());
            }
        }        
    }

    // Perform the hooks.
    if (payload_customs_do_hooks() == false) {
        LogPrintf(ELogLevel::ERROR, L"Could not hook necessary functions to load customs!\n");
        return false;
    }

    return true;
}