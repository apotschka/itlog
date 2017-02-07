
<A name="toc1-3" title="libitlog" />
libitlog
========
A portable logger for monitoring iterative numerical algorithms.

<A name="toc2-8" title="Purpose" />
Purpose
-------
The library libitlog helps you to visualize the iterations of your numerical
algorithm in a tabular manner by supporting and encouraging you to monitor
important quantities of your computation. It is written in C with a
focus on portability.

<A name="toc2-16" title="License" />
License
-------
This Source Code Form is subject to the terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not distributed with this file, you can obtain one
at http://mozilla.org/MPL/2.0/.

<A name="toc2-23" title="Contribution policy" />
Contribution policy
-------------------
Please fork your own repository and supply updates via pull requests.

<A name="toc2-28" title="Style guide" />
Style guide
-----------
Read the book [Scalable C](https://www.gitbook.com/book/hintjens/scalable-c/details).

<A name="toc2-33" title="Example" />
Example
-------
The following code is the self test of the `il_log` class.

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

It produces the following output:

```out
    Running itlog selftests...
     * il_log: 
    Run without print interval.
    
          last   average   minimum   maximum    [0, 1]
             0       0.0         0         0   0.0e+00
             1       1.0         1         1   3.4e-02
             2       2.0         2         2   6.9e-02
             3       3.0         3         3   1.0e-01
             4       4.0         4         4   1.4e-01
             5       5.0         5         5   1.7e-01
             6       6.0         6         6   2.1e-01
             7       7.0         7         7   2.4e-01
             8       8.0         8         8   2.8e-01
             9       9.0         9         9   3.1e-01
    
          last   average   minimum   maximum    [0, 1]
            10      10.0        10        10   3.4e-01
            11      11.0        11        11   3.8e-01
            12      12.0        12        12   4.1e-01
            13      13.0        13        13   4.5e-01
            14      14.0        14        14   4.8e-01
            15      15.0        15        15 1-4.8e-01
            16      16.0        16        16 1-4.5e-01
            17      17.0        17        17 1-4.1e-01
            18      18.0        18        18 1-3.8e-01
            19      19.0        19        19 1-3.4e-01
    
          last   average   minimum   maximum    [0, 1]
            20      20.0        20        20 1-3.1e-01
            21      21.0        21        21 1-2.8e-01
            22      22.0        22        22 1-2.4e-01
            23      23.0        23        23 1-2.1e-01
            24      24.0        24        24 1-1.7e-01
            25      25.0        25        25 1-1.4e-01
            26      26.0        26        26 1-1.0e-01
            27      27.0        27        27 1-6.9e-02
            28      28.0        28        28 1-3.4e-02
            29      29.0        29        29 1-0.0e+00
    
    Run with print interval.
    
       average   minimum   maximum    [0, 1]
           0.0         0         0   0.0e+00
           1.5         1         2   6.9e-02
           3.5         3         4   1.4e-01
           5.5         5         6   2.1e-01
           7.5         7         8   2.8e-01
           9.5         9        10   3.4e-01
          11.5        11        12   4.1e-01
          13.5        13        14   4.8e-01
          15.5        15        16 1-4.5e-01
          17.5        17        18 1-3.8e-01
    
       average   minimum   maximum    [0, 1]
          19.5        19        20 1-3.1e-01
          21.5        21        22 1-2.4e-01
          23.5        23        24 1-1.7e-01
          25.5        25        26 1-1.0e-01
          27.5        27        28 1-3.4e-02
    OK
     * il_column: OK
    Tests passed OK
```

_This documentation was generated from itlog/README.txt using [Gitdown](https://github.com/zeromq/gitdown)_
