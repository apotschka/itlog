.set GIT=https://github.com/apotschka/itlog

libitlog
========

A portable logger for monitoring iterative numerical algorithms.

Purpose
-------

The library libitlog helps you to visualize the iterations of your numerical
algorithm in a tabular manner by supporting and encouraging you to monitor
important quantities of your computation. It is written in C with a
focus on portability.

License
-------

This Source Code Form is subject to the terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not distributed with this file, you can obtain one
at <http://mozilla.org/MPL/2.0/>.

Contribution policy
-------------------

Please fork your own repository and supply updates via pull requests.

Style guide
-----------

Read the book [Scalable C](https://www.gitbook.com/book/hintjens/scalable-c/details).

API reference
-------------

The main functionality of libitlog is in its `il_log` class, which becomes
accessible by including `#include "itlog.h"`. The `il_log` class has the
following interface:

.pull include/il_log.h@interface,code

Example
-------

The following code is the self test of the `il_log` class.

.pull src/il_log.c@selftest,code

If `verbose` is `true`, it produces the following output:

.pull il_selftest.txt,code

Python bindings
---------------

TODO

