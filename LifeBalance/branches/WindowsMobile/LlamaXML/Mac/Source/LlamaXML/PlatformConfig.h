/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef LLAMAXML_PLATFORMCONFIG_H
#define LLAMAXML_PLATFORMCONFIG_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif

#if __MACH__
	#include <Carbon/Carbon.h>
#else
    #include <Carbon.h>
#endif

#endif
