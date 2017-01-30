/*  =========================================================================
    il_classes - private header file

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

#ifndef IL_CLASSES_H_INCLUDED
#define IL_CLASSES_H_INCLUDED

//  Platform definitions, must come first
#include "platform.h"

//  External API
#include "../include/itlog.h"

//  Extra headers

//  Opaque class structures to allow forward references
//#ifndef IL_COLUMN_T_DEFINED
//typedef struct _il_column_t il_column_t;
//#define IL_COLUMN_T_DEFINED
//#endif

//  Internal API
#include "il_column.h"

//  *** To avoid double-definitions, only define if building without draft ***
#ifndef IL_BUILD_DRAFT_API

//  *** Draft method, defined for internal use only ***
//  Self test of this class.
IL_PRIVATE void
    il_column_test (bool verbose);

//  Self test for private classes
IL_PRIVATE void
    il_private_selftest (bool verbose);

#endif // IL_BUILD_DRAFT_API

#endif
