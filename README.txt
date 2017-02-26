.set GIT=https://github.com/apotschka/itlog

libitlog
========

| Linux & MacOSX | Windows  |
|:--------------:|:--------:|
|[![Build Status](https://travis-ci.org/apotschka/itlog.svg?branch=master)](https://travis-ci.org/apotschka/itlog)|[![Build status](https://ci.appveyor.com/api/projects/status/vjdir00phjdg3ly0?svg=true)](https://ci.appveyor.com/project/apotschka/itlog)|

A portable logger for monitoring iterative numerical algorithms.

Contents
--------

.toc 3

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

Please fork your own repository and supply updates via pull requests. This
project follows the [Collective Code Construction Contract
C4.1](https://rfc.zeromq.org/spec:42/C4).

Style guide
-----------

This project follows the [C Language Style for Scalability
(CLASS)](https://rfc.zeromq.org/spec:21/CLASS/) and the
[ZeroMQ Coding Style Guidelines](http://zeromq.org/docs:style). The unfinished
book [Scalable C](https://www.gitbook.com/book/hintjens/scalable-c/details) by
Pieter Hintjens is a good start for the overall software engineering approach.

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

If `verbose` is `true`, it produces the following output on `stderr`:

.pull il_selftest.txt,code

Python bindings
---------------

This library comes with bindings to Python, automatically generated via
[zproject](https://github.com/zeromq/zproject). Here is a short example script,
similar to the example above:

.pull bindings/python/il_selftest.py,code

History
-------

My original motivation to start libitlog was to gather hands-on experience with
Scalable-C style software engineering. I needed a small project to work on. It
was the birth of libitlog.

