///////////////////////////////////////////////////////////////////////////////
// File: payload_customs.h
///////////////////////////////////////////////////////////////////////////////
// Purpose: Handles loading of custom modules.
///////////////////////////////////////////////////////////////////////////////
// Author: Deniz Sezen
///////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

// Mono includes
#include "mono.h"

///////////////////////////////////////////////////////////////////////////////
// Module functions
///////////////////////////////////////////////////////////////////////////////

/**
 * Initializes the customs module.
 * @return true if customs are initialized successfully.
 */
extern bool payload_customs_init(void);