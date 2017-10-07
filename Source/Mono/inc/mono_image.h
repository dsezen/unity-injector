///////////////////////////////////////////////////////////////////////////////
// File: mono_image.h
///////////////////////////////////////////////////////////////////////////////
// Purpose: Replicates mono image functionality.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Mono includes
#include "mono_types.h"
#include "mono_func_helpers.h"

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////
typedef struct _MonoImage MonoImage;
typedef struct _MonoAssembly MonoAssembly;
typedef struct _MonoTableInfo MonoTableInfo;

///////////////////////////////////////////////////////////////////////////////
// Defines
///////////////////////////////////////////////////////////////////////////////
#define MONO_PUBLIC_KEY_TOKEN_LENGTH	17

///////////////////////////////////////////////////////////////////////////////
// Enumerations
///////////////////////////////////////////////////////////////////////////////

/** Image open status. */
typedef enum {
    MONO_IMAGE_OK,
    MONO_IMAGE_ERROR_ERRNO,
    MONO_IMAGE_MISSING_ASSEMBLYREF,
    MONO_IMAGE_IMAGE_INVALID
} MonoImageOpenStatus;

///////////////////////////////////////////////////////////////////////////////
// Structures
///////////////////////////////////////////////////////////////////////////////

/** Name data for an assembly. */
typedef struct {
    const char *name;
    const char *culture;
    const char *hash_value;
    const guint8* public_key;
    // string of 16 hex chars + 1 NULL
    guchar public_key_token[MONO_PUBLIC_KEY_TOKEN_LENGTH];
    guint32 hash_alg;
    guint32 hash_len;
    guint32 flags;
    guint16 major, minor, build, revision;
} MonoAssemblyName;

/** Image metadata. */
struct _MonoImage {
    /*
    * The number of assemblies which reference this MonoImage though their 'image'
    * field plus the number of images which reference this MonoImage through their
    * 'modules' field, plus the number of threads holding temporary references to
    * this image between calls of mono_image_open () and mono_image_close ().
    */
    int   ref_count;
    void *raw_data_handle;
    char *raw_data;
    guint32 raw_data_len;
    guint8 raw_buffer_used : 1;
    guint8 raw_data_allocated : 1;

    /* Module was loaded using LoadLibrary. */
    guint8 is_module_handle : 1;

    /* Module entry point is _CorDllMain. */
    guint8 has_entry_point : 1;

    /* Whenever this is a dynamically emitted module */
    guint8 dynamic : 1;

    /* Whenever this is a reflection only image */
    guint8 ref_only : 1;

    /* Whenever this image contains uncompressed metadata */
    guint8 uncompressed_metadata : 1;

    guint8 checked_module_cctor : 1;
    guint8 has_module_cctor : 1;

    guint8 idx_string_wide : 1;
    guint8 idx_guid_wide : 1;
    guint8 idx_blob_wide : 1;

    char *name;
    const char *assembly_name;
    const char *module_name;
    char *version;
    gint16 md_version_major, md_version_minor;
    char *guid;
    void *image_info;
};

///////////////////////////////////////////////////////////////////////////////
// Image interface
///////////////////////////////////////////////////////////////////////////////
DECLARE_MONO_FUNC(mono_image_open, MonoImage*, const char* fname, MonoImageOpenStatus* status);
DECLARE_MONO_FUNC(mono_image_open_full, MonoImage*, const char* fname, MonoImageOpenStatus* status, gboolean refonly);
DECLARE_MONO_FUNC(mono_image_open_from_data, MonoImage*, char* data, guint32 data_len, gboolean need_copy, MonoImageOpenStatus* status, gboolean refonly);
DECLARE_MONO_FUNC(mono_image_open_from_data_full, MonoImage*, char* data, guint32 data_len, gboolean need_copy, MonoImageOpenStatus* status, gboolean refonly);
DECLARE_MONO_FUNC(mono_image_get_assembly, MonoAssembly*, MonoImage* image);
DECLARE_MONO_FUNC(mono_image_loaded, MonoImage*, const char* name);

///////////////////////////////////////////////////////////////////////////////
// Image interface initialization
///////////////////////////////////////////////////////////////////////////////
bool mono_image_api_init(void);