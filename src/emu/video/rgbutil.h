// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/***************************************************************************

    rgbutil.h

    Utility definitions for RGB manipulation. Allows RGB handling to be
    performed in an abstracted fashion and optimized with SIMD.

***************************************************************************/

#ifndef MAME_EMU_VIDEO_RGBUTIL_H
#define MAME_EMU_VIDEO_RGBUTIL_H

#if !defined(MAME_DEBUG) || defined(__OPTIMIZE__)

// use SSE on 64-bit implementations, where it can be assumed
#if defined(__SSE2__) || defined(__x86_64__) || defined(_M_X64)
#include "rgbsse.h"
#elif defined(__ALTIVEC__)
#include "rgbvmx.h"
#else
#include "rgbgen.h"
#endif

#else // !defined(MAME_DEBUG) || defined(__OPTIMIZE__)

#include "rgbgen.h"

#endif // !defined(MAME_DEBUG) || defined(__OPTIMIZE__)

#endif // MAME_EMU_VIDEO_RGBUTIL_H
