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
    zlist_t *header_list;  //  Column headers are saved in a list.
    zlist_t *column_list;  //  Column entries are saved in a list.
    zhash_t *columns;  //  Column entries are also saved in a hash.
    uint64_t n_lines_printed;  //  Number of lines printed.
    int64_t last_time;  //  Time of last line printing [ms].
    int64_t print_interval;  //  Interval between line printings [ms].
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
    self->header_list = zlist_new ();
    self->column_list = zlist_new ();
    self->columns = zhash_new ();
    self->n_lines_printed = 0;
    self->last_time = 0;
    self->print_interval = 0;
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
        zlist_destroy (&self->column_list);
        zlist_destroy (&self->header_list);
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Set print interval.

void
il_log_set_print_interval (il_log_t *self, int64_t _print_interval)
{
    assert (self);
    self->print_interval = _print_interval;
}


//  --------------------------------------------------------------------------
//  Add or update an entry.

bool
il_log_entry (il_log_t *self, const char *header_format, const char *header_name, 
        const char *entry_format, float value, int mode)
{
    bool new_column = false;
    assert (self);
    il_column_t *col = (il_column_t *) zhash_lookup (self->columns, header_name);
    if (!col) {
        int rc;
        new_column = true;
        col = il_column_new ();
        col->header_format = header_format;
        col->entry_format = entry_format;
        col->mode = mode;
        rc = zlist_append (self->header_list, (void *) header_name); assert (rc == 0);
        rc = zlist_append (self->column_list, col); assert (rc == 0);
        rc = zhash_insert (self->columns, header_name, col); assert (rc == 0);
        zlist_freefn (self->column_list, col, free, true);
    }
    assert ("You cannot change the mode of a column" && mode == col->mode);
    switch (mode & 0xFF) {
        case IL_LOG_USE_LAST: col->data = value; break;
        case IL_LOG_USE_AVERAGE: col->data += value; col->modifier += 1.0; break;
        case IL_LOG_USE_MIN: 
            if (col->modifier == 1.0)
                col->data = value < col->data? value: col->data;
            else {
                col->data = value;
                col->modifier = 1.0;
            }
            break;
        case IL_LOG_USE_MAX: 
            if (col->modifier == 1.0)
                col->data = value > col->data? value: col->data;
            else {
                col->data = value;
                col->modifier = 1.0;
            }
            break;
        default: assert ("Unknown IL_LOG_USE_<.> constant" && 0); break;
    }
    return new_column;
}


//  --------------------------------------------------------------------------
//  Print out accumulated data.

void
il_log_print (il_log_t *self)
{
    assert (self);

    if (self->print_interval > 0) {
        int64_t now = zclock_mono ();
        if (now < self->last_time + self->print_interval)
            return;
        self->last_time = now;
    }

    il_column_t *col;
    if (self->n_lines_printed++ % 10 == 0) {
        //  Print header
        char *header_name;
        fprintf (self->file, "\n");
        header_name = (char *) zlist_first (self->header_list);
        col = (il_column_t *) zlist_first (self->column_list);
        while (header_name && col) {
            fprintf (self->file, col->header_format, header_name);
            header_name = (char *) zlist_next (self->header_list);
            col = (il_column_t *) zlist_next (self->column_list);
        }
        fprintf (self->file, "\n");
        assert (header_name == NULL && col == NULL);
    }
    //  Print and reset accumulated data
    col = (il_column_t *) zlist_first (self->column_list);
    double data;
    while (col) {
        data = col->data;
        if ((col->mode & 0xFF) == IL_LOG_USE_AVERAGE) 
            data /= col->modifier;
        if (col->mode & IL_LOG_UNIT_INTERVAL) {
            if (data <= 0.5)
                fprintf (self->file, "   ");
            else {
                fprintf (self->file, " 1-");
                data = 1.0 - data;
            }
        }
        fprintf (self->file, col->entry_format, data);
        col->data = 0.0;
        col->modifier = 0.0;
        col = (il_column_t *) zlist_next (self->column_list);
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

    const double data_max = 30.0;
    double data;
    int run;
    for (run = 0; run < 2; run++) {
        fprintf(stdout, "\nRun %s print interval.\n", run == 0? "without": "with");
        //  Loop with synthetic data
        for (data = 0.0; data < data_max; data += 1.0) {
            zclock_sleep (30);
            il_log_entry (self, "%10s", "last", "%10.0f", data, IL_LOG_USE_LAST);
            il_log_entry (self, "%10s", "average", "%10.1f", data, IL_LOG_USE_AVERAGE);
            il_log_entry (self, "%10s", "minimum", "%10.0f", data, IL_LOG_USE_MIN);
            il_log_entry (self, "%10s", "maximum", "%10.0f", data, IL_LOG_USE_MAX);
            il_log_entry (self, "%10s", "[0, 1]", "%7.1e", data / (data_max-1.0),
                    IL_LOG_UNIT_INTERVAL | IL_LOG_USE_LAST);
            il_log_print (self);
        }

        //  Switch on print interval
        il_log_set_print_interval (self, 50);
    }

    il_log_destroy (&self);
    //  @end
    printf ("OK\n");
}
