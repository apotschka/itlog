/*  =========================================================================
    il_column - Column data container.

    Copyright (c) the Contributors as noted in the AUTHORS file.       
    This file is part of the IterationLogger library.                  
                                                                       
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.           
    =========================================================================
*/

#ifndef IL_COLUMN_H_INCLUDED
#define IL_COLUMN_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _il_column_t
{
    const char *header_format;  //  Format string for the header line, e.g., "%8s".
    const char *entry_format;  //  Format string for the data, e.g., " %7.2e".
    double data;  //  A representation of data.
    double modifier;  //  A modifier for the representation of data;
    int mode;  //  Data accumulation mode.
    int print_level;  //  Print level of entry.
} il_column_t;

//  @interface
//  Create a new il_column
IL_PRIVATE il_column_t *
    il_column_new (void);

//  Destroy the il_column
IL_PRIVATE void
    il_column_destroy (il_column_t **self_p);

//  Self test of this class
IL_PRIVATE void
    il_column_test (bool verbose);

//  @end

#ifdef __cplusplus
}
#endif

#endif
