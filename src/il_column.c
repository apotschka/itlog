/*  =========================================================================
    il_column - Column data container.

    Copyright (c) the Contributors as noted in the AUTHORS file.       
    This file is part of the IterationLogger library.                  
                                                                       
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.           
    =========================================================================
*/

/*
@header
    il_column - Column data container.
@discuss
@end
*/

#include "il_classes.h"

//  Structure of our class is defined in header.


//  --------------------------------------------------------------------------
//  Create a new il_column

il_column_t *
il_column_new (void)
{
    il_column_t *self = (il_column_t *) zmalloc (sizeof (il_column_t));
    assert (self);
    //  Initialize class properties here
    self->header_format = NULL;
    self->entry_format = NULL;
    self->data = 0.0;
    self->modifier = 0.0;
    self->mode = 0;
    self->print_level = 0;
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the il_column

void
il_column_destroy (il_column_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        il_column_t *self = *self_p;
        //  Free class properties here
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}

//  --------------------------------------------------------------------------
//  Self test of this class

void
il_column_test (bool verbose)
{
    printf (" * il_column: ");

    //  @selftest
    //  Simple create/destroy test
    il_column_t *self = il_column_new ();
    assert (self);
    il_column_destroy (&self);
    //  @end
    printf ("OK\n");
}
