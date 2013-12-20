#ifndef __included_erbium_h
#define __included_erbium_h

#include "platform_detect.h"

#if defined(TARGET_OS_ANDROID)
#   define TARGET_OS_MOBILE
#else
#   define TARGET_OS_DESKTOP
#endif

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#if defined(TARGET_OS_WINDOWS)
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   include <shlobj.h>
#else
#   include <sys/stat.h>
#   include <sys/types.h>
#   include <unistd.h>
#   include <signal.h>
#endif

#include "erbium_version.h"

#endif /* __included_erbium_h */
