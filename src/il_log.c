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
    int64_t output_interval;  //  Interval between line outputs [ms].
    int print_level;
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
    self->output_interval = 0;
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
//  Set output interval.

void
il_log_set_output_interval (il_log_t *self, int64_t interval)
{
    assert (self);
    self->output_interval = interval;
}


//  --------------------------------------------------------------------------
//  Add or update an entry.

bool
il_log_entry (il_log_t *self, int print_level, const char *header_format, const char *header_name, 
        const char *entry_format, double value, int mode)
{
    assert (self);
    if (print_level > self->print_level) return false;

    bool new_column = false;
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
    col->print_level = print_level;
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
//  Set print level of the logger.

void
il_log_set_print_level (il_log_t *self, int print_level)
{
    assert (self);
    self->print_level = print_level;
}


//  --------------------------------------------------------------------------
//  Print out accumulated data.

void
il_log_output_line (il_log_t *self)
{
    assert (self);

    if (self->output_interval > 0) {
        int64_t now = zclock_mono ();
        if (now < self->last_time + self->output_interval)
            return;
        self->last_time = now;
    }

    bool printed_something = false;
    il_column_t *col;
    if (self->n_lines_printed++ % 10 == 0) {
        //  Print header
        char *header_name;
        header_name = (char *) zlist_first (self->header_list);
        col = (il_column_t *) zlist_first (self->column_list);
        while (header_name && col) {
            if (col->print_level <= self->print_level) {
                if (!printed_something)
                    fprintf (self->file, "\n");
                fprintf (self->file, col->header_format, header_name);
                printed_something = true;
            }
            header_name = (char *) zlist_next (self->header_list);
            col = (il_column_t *) zlist_next (self->column_list);
        }
        if (printed_something)
            fprintf (self->file, "\n");
        assert (header_name == NULL && col == NULL);
    }
    //  Print and reset accumulated data
    col = (il_column_t *) zlist_first (self->column_list);
    double data;
    while (col) {
        if (col->print_level <= self->print_level) {
            printed_something = true;
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
        }
        col->data = 0.0;
        col->modifier = 0.0;
        col = (il_column_t *) zlist_next (self->column_list);
    }
    if (printed_something)
        fprintf (self->file, "\n");
}

//  --------------------------------------------------------------------------
//  Self test of this class

void
il_log_test (bool verbose)
{
    printf (" * il_log: ");

    //  @selftest
    //  Log two runs of the same data with different output intervals and print levels.
    il_log_t *self = il_log_new ();
    assert (self);
    il_log_set_print_level (self, 3);

    const double data_max = 30.0;
    double data;
    int run;
    for (run = 0; run < 2; run++) {
        fprintf(stdout, "\nRun %s print interval.\n", run == 0? "without": "with");
        //  Loop with synthetic data
        for (data = 0.0; data < data_max; data += 1.0) {
            zclock_sleep (30);
            il_log_entry (self, 3, "%10s", "last", "%10.0f", data, IL_LOG_USE_LAST);
            il_log_entry (self, 0, "%10s", "average", "%10.1f", data, IL_LOG_USE_AVERAGE);
            il_log_entry (self, 0, "%10s", "minimum", "%10.0f", data, IL_LOG_USE_MIN);
            il_log_entry (self, 0, "%10s", "maximum", "%10.0f", data, IL_LOG_USE_MAX);
            il_log_entry (self, 0, "%10s", "[0, 1]", "%7.1e", data / (data_max-1.0),
                    IL_LOG_UNIT_INTERVAL | IL_LOG_USE_LAST);
            il_log_output_line (self);
        }
        //  Switch on output interval
        il_log_set_output_interval (self, 50);
        //  Change print level
        il_log_set_print_level (self, 2);
    }
    il_log_destroy (&self);
    assert (self == NULL);
    //  @end
    printf ("OK\n");
}
