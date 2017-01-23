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
    zhash_t *columns;  //  Column entries are saved in a hash.
    int n_lines_printed;  //  Number of lines printed.
    FILE *file;  //  Output file (default: stdout).
};


//  --------------------------------------------------------------------------
//  Create a new il_log

il_log_t *
il_log_new (void)
{
    il_log_t *self = (il_log_t *) zmalloc (sizeof (il_log_t));
    assert (self);
    //  Initialize class properties here
    self->columns = zhash_new ();
    self->n_lines_printed = 0;
    self->file = stdout;
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
        zhash_destroy (&self->columns);
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Add or update an entry.

bool
il_log_entry (il_log_t *self, const char *header_format, const char *header_name, const char *entry_format, float value, int mode)
{
    bool new_column = false;
    assert (self);
    il_column_t *col = (il_column_t *) zhash_lookup (self->columns, header_name);
    if (!col) {
        new_column = true;
        col = il_column_new ();
        int rc = zhash_insert (self->columns, header_name, col);
        assert (rc == 0);
        col->header_format = header_format;
        col->entry_format = entry_format;
        col->mode = mode;
        zhash_freefn (self->columns, header_name, free);
    }
    assert ("You cannot change the mode of a column" && mode == col->mode);
    switch (mode) {
        case IL_LOG_USE_LAST: col->data = value; break;
        case IL_LOG_USE_AVERAGE: col->data += value; col->modifier += 1.0; break;
        case IL_LOG_USE_MIN: 
            if (col->modifier == 1.0)
                col->data = value < col->data? value: col->data;
            else
            {
                col->data = value;
                col->modifier = 1.0;
            }
            break;
        case IL_LOG_USE_MAX: 
            if (col->modifier == 1.0)
                col->data = value > col->data? value: col->data;
            else
            {
                col->data = value;
                col->modifier = 1.0;
            }
            break;
        default: assert ("Unknown IL_LOG_USE_<.> constant"); break;
    }
    return new_column;
}


//  --------------------------------------------------------------------------
//  Print out accumulated data.

void
il_log_print (il_log_t *self)
{
    il_column_t *col;
    assert (self);
    if (self->n_lines_printed++ % 10 == 0) {
        //  Print header
        char *header_name;
        fprintf (self->file, "\n");
        zlist_t *keys = zhash_keys (self->columns);
        assert (keys);
        header_name = (char *) zlist_first (keys);
        col = (il_column_t *) zhash_first (self->columns);
        while (header_name && col) {
            fprintf (self->file, col->header_format, header_name);
            header_name = (char *) zlist_next (keys);
            col = (il_column_t *) zhash_next (self->columns);
        }
        fprintf (self->file, "\n");
        assert (header_name == NULL && col == NULL);
        zlist_destroy (&keys);
    }
    //  Print and reset accumulated data
    col = (il_column_t *) zhash_first (self->columns);
    while (col) {
        if (col->mode != IL_LOG_USE_AVERAGE)
            fprintf (self->file, col->entry_format, col->data);
        else
            fprintf (self->file, col->entry_format, col->data / col->modifier);
        col->data = 0.0;
        col->modifier = 0.0;
        col = (il_column_t *) zhash_next (self->columns);
    }
    fprintf (self->file, "\n");
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

    //  Loop with synthetic data
    double data;
    for (data = 0.0; data < 20.0; data += 1.0)
    {
        il_log_entry (self, "%10s", "last", "%10.0f", data, IL_LOG_USE_LAST);
        il_log_entry (self, "%10s", "average", "%10.1f", data, IL_LOG_USE_AVERAGE);
        il_log_entry (self, "%10s", "minimum", "%10.0f", data, IL_LOG_USE_MIN);
        il_log_entry (self, "%10s", "maximum", "%10.0f", data, IL_LOG_USE_MAX);

        il_log_print (self);
    }

    il_log_destroy (&self);
    //  @end
    printf ("OK\n");
}
