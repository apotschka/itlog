/*  =========================================================================
    il_log - Iteration logger class.

    Copyright (c) the Contributors as noted in the AUTHORS file.       
    This file is part of the IterationLogger library.                  
                                                                       
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.           
    =========================================================================
*/

/*
@header
    il_log - Iteration logger class.
@discuss
@end
*/

#include "il_classes.h"

//  Structure of our class

struct _il_log_t {
    int filler;     //  Declare class properties here
};


//  --------------------------------------------------------------------------
//  Create a new il_log

il_log_t *
il_log_new (void)
{
    il_log_t *self = (il_log_t *) zmalloc (sizeof (il_log_t));
    assert (self);
    //  Initialize class properties here
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the il_log

void
il_log_destroy (il_log_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        il_log_t *self = *self_p;
        //  Free class properties here
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}

//  --------------------------------------------------------------------------
//  Self test of this class

void
il_log_test (bool verbose)
{
    printf (" * il_log: ");

    //  @selftest
    //  Simple create/destroy test
    il_log_t *self = il_log_new ();
    assert (self);
    il_log_destroy (&self);
    //  @end
    printf ("OK\n");
}
