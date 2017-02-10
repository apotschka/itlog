/*  =========================================================================
    il_fid_list - List of FILE descriptors for il_log output.

    Copyright (c) the Contributors as noted in the AUTHORS file.       
    This file is part of the IterationLogger library.                  
                                                                       
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.           
    =========================================================================
*/

/*
@header
    il_fid_list - List of FILE descriptors for il_log output.
@discuss
@end
*/

#include "il_classes.h"

//  Structure of our class

struct _il_fid_list_t {
    zlist_t *fids;    //  List of FILE descriptors
};


//  --------------------------------------------------------------------------
//  Create a new il_fid_list

il_fid_list_t *
il_fid_list_new (void)
{
    il_fid_list_t *self = (il_fid_list_t *) zmalloc (sizeof (il_fid_list_t));
    assert (self);
    //  Initialize class properties here
    self->fids = zlist_new ();
    assert (self->fids);
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the il_fid_list

void
il_fid_list_destroy (il_fid_list_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        il_fid_list_t *self = *self_p;
        //  Free class properties here
        zlist_destroy (&self->fids);
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Add file descriptor to list.

void
il_fid_list_add (il_fid_list_t *self, FILE *fid)
{
    assert (self && self->fids);
    int rv = zlist_append (self->fids, (void *) fid);
    assert ("Could not append fid to list" && rv == 0);
}


//  --------------------------------------------------------------------------
//  Remove file descriptor from list.

void
il_fid_list_remove (il_fid_list_t *self, FILE *fid)
{
    assert (self && self->fids);
    zlist_remove (self->fids, (void *) fid);
}


//  --------------------------------------------------------------------------
//  Print formatted string with variable argument list to all FILE descriptors in list.

void
il_fid_list_printf (il_fid_list_t *self, const char *format, ...)
{
    assert (self && self->fids);

    // process format arguments
    va_list args;
    char output [1000];
    va_start (args, format);
    vsnprintf (output, sizeof (output) - 1, format, args);
    va_end (args);
    output [sizeof (output) - 1] = 0;

    // print strings to FILE descriptors
    FILE *fid = (FILE *) zlist_first (self->fids);
    while (fid) {
        fputs (output, fid);
        fid = (FILE *) zlist_next (self->fids);
    }
}


//  --------------------------------------------------------------------------
//  Self test of this class

void
il_fid_list_test (bool verbose)
{
    printf (" * il_fid_list: ");

    //  @selftest
    //  Simple create/destroy test
    il_fid_list_t *self = il_fid_list_new ();
    assert (self);
    il_fid_list_destroy (&self);

    //  Add, print, remove, print test
    int ntest = 1;
    self = il_fid_list_new ();
    il_fid_list_add (self, stdout);
    il_fid_list_printf (self, "\nTest %d: Only to %s\n", ntest++, "stdout");
    il_fid_list_add (self, stderr);
    il_fid_list_printf (self, "\nTest %d: To %s and %s\n", ntest++, "stdout", "stderr");
    il_fid_list_remove (self, stdout);
    il_fid_list_printf (self, "\nTest %d: Only to %s\n", ntest++, "stderr");
    il_fid_list_destroy (&self);
    //  @end
    printf ("OK\n");
}
