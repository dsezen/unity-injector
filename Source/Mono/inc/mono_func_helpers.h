///////////////////////////////////////////////////////////////////////////////
// File: mono_func_helpers.h
///////////////////////////////////////////////////////////////////////////////
// Purpose: Defines a bunch of macros that help generate function prototypes
// and definitions for mono-2.0.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Macros
///////////////////////////////////////////////////////////////////////////////

/** Declares a mono function pointer and prototype. */
#define DECLARE_MONO_FUNC(mono_func_name, rettype, ...) \
    typedef rettype (__cdecl* mono_func_name##_t)(__VA_ARGS__); \
    extern mono_func_name##_t mono_func_name;

/** Implements the actual static object for a mono function. */
#define DEFINE_MONO_FUNC(mono_func_name) \
    mono_func_name##_t mono_func_name = nullptr;

/** Generates code to retrieve the address of a mono function. */
#define MONO_FUNC_GET_ADDR(h_mono_module, mono_func_name, prev_result) \
    if(prev_result) { \
        mono_func_name = (mono_func_name##_t)((mono_func_name##_t)GetProcAddress(h_mono_module, #mono_func_name)); \
        LogPrintf(ELogLevel::DEBUG, L"Found address of %s at: %016" PRIxPTR "\n", L#mono_func_name, (uintptr_t)mono_func_name); \
        prev_result = prev_result && (mono_func_name != nullptr); \
    }
