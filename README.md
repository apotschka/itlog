
<A name="toc1-3" title="libitlog"></A>
libitlog
========

A portable logger for monitoring iterative numerical algorithms.

<A name="toc2-9" title="Purpose"></A>
Purpose
-------

The library libitlog helps you to visualize the iterations of your numerical
algorithm in a tabular manner by supporting and encouraging you to monitor
important quantities of your computation. It is written in C with a
focus on portability.

<A name="toc2-18" title="License"></A>
License
-------

This Source Code Form is subject to the terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not distributed with this file, you can obtain one
at <http://mozilla.org/MPL/2.0/>.

<A name="toc2-26" title="Contribution policy"></A>
Contribution policy
-------------------

Please fork your own repository and supply updates via pull requests.

<A name="toc2-32" title="Style guide"></A>
Style guide
-----------

Read the book [Scalable C](https://www.gitbook.com/book/hintjens/scalable-c/details).

<A name="toc2-38" title="API reference"></A>
API reference
-------------

The main functionality of libitlog is in its `il_log` class, which becomes
accessible by including `#include "itlog.h"`. The `il_log` class has the
following interface:

```h
    //  This API is a draft, and may change without notice.
    #ifdef IL_BUILD_DRAFT_API
    #define IL_LOG_USE_LAST 1                   // Log entry supplied last.
    #define IL_LOG_USE_AVERAGE 2                // Log average of entries since last log line.
    #define IL_LOG_USE_MIN 3                    // Log minimum of entries since last log line.
    #define IL_LOG_USE_MAX 4                    // Log maximum of entries since last log line.
    #define IL_LOG_USE_SUM 5                    // Log sum of entries since last log line.
    #define IL_LOG_UNIT_INTERVAL 0x100          // Flag for special printing of values in [0,1].
    
    //  *** Draft method, for development use, may change without warning ***
    //  Constructor.
    IL_EXPORT il_log_t *
        il_log_new (void);
    
    //  *** Draft method, for development use, may change without warning ***
    //  Destructor.
    IL_EXPORT void
        il_log_destroy (il_log_t **self_p);
    
    //  *** Draft method, for development use, may change without warning ***
    //  Set the output interval in microseconds. The default is 0.
    IL_EXPORT void
        il_log_set_output_interval (il_log_t *self, int msecs);
    
    //  *** Draft method, for development use, may change without warning ***
    //  Set the frequency of header lines. The default is 10.
    IL_EXPORT void
        il_log_set_header_frequency (il_log_t *self, int freq);
    
    //  *** Draft method, for development use, may change without warning ***
    //  Add or update an entry of the logger provided that the print level argument does
    //  not exceed the print level of the logger. If entries of the same header name    
    //  have been used before, the method accumulates the data and returns false. If no 
    //  entry exists yet, the method creates it and returns true. Possible modes for    
    //  accumulation of data are                                                        
    //                                                                                  
    //  * IL_LOG_USE_LAST: Entry supplied last                                          
    //  * IL_LOG_USE_AVERAGE: Average of entries since last printing.                   
    //  * IL_LOG_USE_MIN: Minimum of entries since last printing.                       
    //  * IL_LOG_USE_MAX: Maximum of entries since last printing.                       
    //  * IL_LOG_USE_SUM: Sum of entries since last printing.                           
    //                                                                                  
    //  An additional flag IL_LOG_UNIT_INTERVAL can be set using "|" for special        
    //  printing of values between 0 and 1. In this case, the entry format should be    
    //  shorter than the header format by three characters.                             
    IL_EXPORT bool
        il_log_entry (il_log_t *self, int print_level, const char *header_format, const char *header_name, const char *entry_format, double value, int mode);
    
    //  *** Draft method, for development use, may change without warning ***
    //  Set the print level of the logger. All columns exceeding the current print level
    //  will not get printed.                                                           
    IL_EXPORT void
        il_log_set_print_level (il_log_t *self, int print_level);
    
    //  *** Draft method, for development use, may change without warning ***
    //  If the time specified by the output interval has passed since the last time this
    //  method has printed a line, the accumulated data will be formatted and printed.  
    //  Every 10th line, a newline and a header will be printed (the number can be      
    //  changed by setting the header frequency). The accumulated data of all entries   
    //  will be reset. If not enough time has passed, nothing happens. The output will  
    //  be printed to all files registerd with `il_log_add_file`. Standard output is    
    //  registered by default.                                                          
    IL_EXPORT void
        il_log_output_line (il_log_t *self);
    
    //  *** Draft method, for development use, may change without warning ***
    //  Add another file descriptor to the list of output streams.
    IL_EXPORT void
        il_log_add_file (il_log_t *self, FILE *fid);
    
    //  *** Draft method, for development use, may change without warning ***
    //  Remove file descriptor from the list of output streams.
    IL_EXPORT void
        il_log_remove_file (il_log_t *self, FILE *fid);
    
    //  *** Draft method, for development use, may change without warning ***
    //  Usage example and self test.
    IL_EXPORT void
        il_log_test (bool verbose);
    
    #endif // IL_BUILD_DRAFT_API
```

<A name="toc2-130" title="Example"></A>
Example
-------

The following code is the self test of the `il_log` class.

```c
        //  Log two runs of the same data with different output intervals and print levels.
        il_log_t *self = il_log_new ();
        assert (self);
        il_log_set_print_level (self, 3);
    
        //  Reroute output to a temporary file and, if verbose, add stderr
        FILE *tmp_file = tmpfile ();
        assert (tmp_file);
        il_log_add_file (self, tmp_file);
        il_log_remove_file (self, stdout);
        if (verbose)
            il_log_add_file (self, stderr);
    
        const double data_max = 30.0;
        double data;
        int run;
        for (run = 0; run < 2; run++) {
            if (verbose)
                fprintf (stderr, "\nRun %s output interval.\n", run == 0? "without": "with");
            //  Loop with synthetic data
            for (data = 0.0; data < data_max; data += 1.0) {
                zclock_sleep (3);
                il_log_entry (self, 3, "%10s", "last", "%10.0f", data, IL_LOG_USE_LAST);
                il_log_entry (self, 0, "%10s", "average", "%10.1f", data, IL_LOG_USE_AVERAGE);
                il_log_entry (self, 0, "%10s", "minimum", "%10.0f", data, IL_LOG_USE_MIN);
                il_log_entry (self, 0, "%10s", "maximum", "%10.0f", data, IL_LOG_USE_MAX);
                il_log_entry (self, 0, "%10s", "sum", "%10.0f", data, IL_LOG_USE_SUM);
                il_log_entry (self, 0, "%10s", "[0, 1]", "%7.1e", data / (data_max-1.0),
                        IL_LOG_UNIT_INTERVAL | IL_LOG_USE_LAST);
                il_log_output_line (self);
            }
            //  Switch on output interval
            il_log_set_output_interval (self, 5);
            //  Change print level
            il_log_set_print_level (self, 2);
        }
        il_log_destroy (&self);
        assert (self == NULL);
        fclose (tmp_file);
```

If `verbose` is `true`, it produces the following output on `stderr`:

```txt
    
    Run without output interval.
    
          last   average   minimum   maximum       sum    [0, 1]
             0       0.0         0         0         0   0.0e+00
             1       1.0         1         1         1   3.4e-02
             2       2.0         2         2         2   6.9e-02
             3       3.0         3         3         3   1.0e-01
             4       4.0         4         4         4   1.4e-01
             5       5.0         5         5         5   1.7e-01
             6       6.0         6         6         6   2.1e-01
             7       7.0         7         7         7   2.4e-01
             8       8.0         8         8         8   2.8e-01
             9       9.0         9         9         9   3.1e-01
    
          last   average   minimum   maximum       sum    [0, 1]
            10      10.0        10        10        10   3.4e-01
            11      11.0        11        11        11   3.8e-01
            12      12.0        12        12        12   4.1e-01
            13      13.0        13        13        13   4.5e-01
            14      14.0        14        14        14   4.8e-01
            15      15.0        15        15        15 1-4.8e-01
            16      16.0        16        16        16 1-4.5e-01
            17      17.0        17        17        17 1-4.1e-01
            18      18.0        18        18        18 1-3.8e-01
            19      19.0        19        19        19 1-3.4e-01
    
          last   average   minimum   maximum       sum    [0, 1]
            20      20.0        20        20        20 1-3.1e-01
            21      21.0        21        21        21 1-2.8e-01
            22      22.0        22        22        22 1-2.4e-01
            23      23.0        23        23        23 1-2.1e-01
            24      24.0        24        24        24 1-1.7e-01
            25      25.0        25        25        25 1-1.4e-01
            26      26.0        26        26        26 1-1.0e-01
            27      27.0        27        27        27 1-6.9e-02
            28      28.0        28        28        28 1-3.4e-02
            29      29.0        29        29        29 1-0.0e+00
    
    Run with output interval.
    
       average   minimum   maximum       sum    [0, 1]
           0.0         0         0         0   0.0e+00
           1.5         1         2         3   6.9e-02
           3.5         3         4         7   1.4e-01
           5.5         5         6        11   2.1e-01
           7.5         7         8        15   2.8e-01
           9.5         9        10        19   3.4e-01
          11.5        11        12        23   4.1e-01
          13.5        13        14        27   4.8e-01
          15.5        15        16        31 1-4.5e-01
          17.5        17        18        35 1-3.8e-01
    
       average   minimum   maximum       sum    [0, 1]
          19.5        19        20        39 1-3.1e-01
          21.5        21        22        43 1-2.4e-01
          23.5        23        24        47 1-1.7e-01
          25.5        25        26        51 1-1.0e-01
          27.5        27        28        55 1-3.4e-02
```

<A name="toc2-242" title="Python bindings"></A>
Python bindings
---------------

TODO

<A name="toc2-248" title="History"></A>
History
-------

My original motivation to start libitlog was to gather hands-on experience with
Scalable-C style software engineering. I needed a small project to work on. It
was the birth of libitlog.


_This documentation was generated from itlog/README.txt using [Gitdown](https://github.com/zeromq/gitdown)_
