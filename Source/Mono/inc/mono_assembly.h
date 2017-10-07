///////////////////////////////////////////////////////////////////////////////
// File: mono_image.h
///////////////////////////////////////////////////////////////////////////////
// Purpose: Replicates mono assembly functionality.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Mono includes
#include "mono_types.h"
#include "mono_glib.h"
#include "mono_image.h"
#include "mono_func_helpers.h"

///////////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////////
typedef struct _MonoDomain MonoDomain;

///////////////////////////////////////////////////////////////////////////////
// Structures
///////////////////////////////////////////////////////////////////////////////

/** Assembly metadata. */
struct _MonoAssembly {
	/*
	* The number of appdomains which have this assembly loaded plus the number of
	* assemblies referencing this assembly through an entry in their image->references
	* arrays. The later is needed because entries in the image->references array
	* might point to assemblies which are only loaded in some appdomains, and without
	* the additional reference, they can be freed at any time.
	* The ref_count is initially 0.
	*/
	int ref_count; /* use atomic operations only */
	char *basedir;
	MonoAssemblyName aname;
	MonoImage *image;
	GSList *friend_assembly_names; /* Computed by mono_assembly_load_friends () */
	guint8 friend_assembly_names_inited;
	guint8 in_gac;
	guint8 dynamic;
	guint8 corlib_internal;
	gboolean ref_only;
	/* security manager flags (one bit is for lazy initialization) */
	guint32 ecma : 2;		/* Has the ECMA key */
	guint32 aptc : 2;		/* Has the [AllowPartiallyTrustedCallers] attributes */
	guint32 fulltrust : 2;	/* Has FullTrust permission */
	guint32 unmanaged : 2;	/* Has SecurityPermissionFlag.UnmanagedCode permission */
	guint32 skipverification : 2;	/* Has SecurityPermissionFlag.SkipVerification permission */
};

///////////////////////////////////////////////////////////////////////////////
// Assembly interface
///////////////////////////////////////////////////////////////////////////////
DECLARE_MONO_FUNC(mono_assembly_open, MonoAssembly*, const char* filename, MonoImageOpenStatus* status);
DECLARE_MONO_FUNC(mono_assembly_open_full, MonoAssembly*, const char* filename, MonoImageOpenStatus* status, gboolean refonly);
DECLARE_MONO_FUNC(mono_assembly_load, MonoAssembly*, MonoAssemblyName* aname, const char* basedir, MonoImageOpenStatus* status);
DECLARE_MONO_FUNC(mono_assembly_load_full, MonoAssembly*, MonoAssemblyName* aname, const char* basedir, MonoImageOpenStatus* status, gboolean refonly);
DECLARE_MONO_FUNC(mono_assembly_load_from, MonoAssembly*, MonoImage* image, const char* fname, MonoImageOpenStatus* status);
DECLARE_MONO_FUNC(mono_assembly_load_from_full, MonoAssembly*, MonoImage*, const char* fname, MonoImageOpenStatus* status, gboolean refonly);
DECLARE_MONO_FUNC(mono_assembly_loaded, MonoAssembly*, MonoAssemblyName* aname);
DECLARE_MONO_FUNC(mono_assembly_loaded_full, MonoAssembly*, MonoAssemblyName* aname, gboolean refonly);
DECLARE_MONO_FUNC(mono_assembly_foreach, void, GFunc func, gpointer user_data);
DECLARE_MONO_FUNC(mono_assembly_name_parse, gboolean, const char* name, MonoAssemblyName* aname);
DECLARE_MONO_FUNC(mono_assembly_invoke_load_hook, void, MonoAssembly* ass);
DECLARE_MONO_FUNC(mono_domain_assembly_open, MonoAssembly*, MonoDomain *domain, const char *name);

///////////////////////////////////////////////////////////////////////////////
// Assembly interface initialization
///////////////////////////////////////////////////////////////////////////////
bool mono_assembly_api_init(void);