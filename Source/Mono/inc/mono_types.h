///////////////////////////////////////////////////////////////////////////////
// File: mono_types.h
///////////////////////////////////////////////////////////////////////////////
// Purpose: Defines mono-specific types used throughout the mono function
// calls.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// C standard library includes
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

/** Basic types from glib. */
typedef int                 gboolean;
typedef int                 gint;
typedef unsigned int        guint;
typedef short               gshort;
typedef unsigned short      gushort;
typedef long                glong;
typedef unsigned long       gulong;
typedef void *              gpointer;
typedef const void *        gconstpointer;
typedef char                gchar;
typedef unsigned char       guchar;

/** Compiler specific types from glib. */
typedef __int8				gint8;
typedef unsigned __int8		guint8;
typedef __int16				gint16;
typedef unsigned __int16	guint16;
typedef __int32				gint32;
typedef unsigned __int32	guint32;
typedef __int64				gint64;
typedef unsigned __int64	guint64;
typedef float				gfloat;
typedef double				gdouble;
typedef unsigned __int16	gunichar2;
typedef uintptr_t			gsize;

/** Other types */
typedef void (*GFunc)(gpointer data, gpointer user_data);