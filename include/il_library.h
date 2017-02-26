/*  =========================================================================
    itlog - generated layer of public API

    Copyright (c) the Contributors as noted in the AUTHORS file.       
    This file is part of the IterationLogger library.                  
                                                                       
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.           

################################################################################
#  THIS FILE IS 100% GENERATED BY ZPROJECT; DO NOT EDIT EXCEPT EXPERIMENTALLY  #
#  Read the zproject/README.md for information about making permanent changes. #
################################################################################
    =========================================================================
*/

#ifndef IL_LIBRARY_H_INCLUDED
#define IL_LIBRARY_H_INCLUDED

//  Set up environment for the application

//  External dependencies
#include <czmq.h>

//  IL version macros for compile-time API detection
#define IL_VERSION_MAJOR 0
#define IL_VERSION_MINOR 3
#define IL_VERSION_PATCH 0

#define IL_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define IL_VERSION \
    IL_MAKE_VERSION(IL_VERSION_MAJOR, IL_VERSION_MINOR, IL_VERSION_PATCH)

#if defined (__WINDOWS__)
#   if defined IL_STATIC
#       define IL_EXPORT
#   elif defined IL_INTERNAL_BUILD
#       if defined DLL_EXPORT
#           define IL_EXPORT __declspec(dllexport)
#       else
#           define IL_EXPORT
#       endif
#   elif defined IL_EXPORTS
#       define IL_EXPORT __declspec(dllexport)
#   else
#       define IL_EXPORT __declspec(dllimport)
#   endif
#   define IL_PRIVATE
#else
#   define IL_EXPORT
#   if (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER
#       define IL_PRIVATE __attribute__ ((visibility ("hidden")))
#   else
#       define IL_PRIVATE
#   endif
#endif

//  Opaque class structures to allow forward references
//  These classes are stable or legacy and built in all releases
typedef struct _il_log_t il_log_t;
#define IL_LOG_T_DEFINED


//  Public classes, each with its own header file
#include "il_log.h"

#ifdef IL_BUILD_DRAFT_API
//  Self test for private classes
IL_EXPORT void
    il_private_selftest (bool verbose);
#endif // IL_BUILD_DRAFT_API

#endif
/*
################################################################################
#  THIS FILE IS 100% GENERATED BY ZPROJECT; DO NOT EDIT EXCEPT EXPERIMENTALLY  #
#  Read the zproject/README.md for information about making permanent changes. #
################################################################################
*/
