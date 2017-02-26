#!/usr/bin/env python

from itlog import IlLog
from time import sleep
from tempfile import TemporaryFile
from sys import stdout

# construct logger
log = IlLog()

# reroute output from stdout to a temporary file
tmpfile = TemporaryFile()
log.add_file(tmpfile)
log.remove_file(stdout)

# set logger print level
log.set_print_level(4)

# wait at least 5 ms from one logging line to the next
log.set_output_interval(5)

# print header line every 7th output line
log.set_header_frequency(7)

# log some data
data_max = 30.0;
data = 0.0;
while data < data_max:
    sleep(0.003) # pretend to compute something
    log.entry(0, "%8s", "last", "%8.0f", data, IlLog.USE_LAST)
    log.entry(0, "%8s", "average", "%8.1f", data, IlLog.USE_AVERAGE)
    log.entry(0, "%8s", "minimum", "%8.0f", data, IlLog.USE_MIN)
    log.entry(0, "%8s", "maximum", "%8.0f", data, IlLog.USE_MAX)
    log.entry(0, "%8s", "sum", "%8.0f", data, IlLog.USE_SUM)
    log.entry(0, "%11s", "[0,1]", "%8.2e", data / (data_max - 1.0), \
            IlLog.USE_LAST | IlLog.UNIT_INTERVAL)
    log.output_line()
    data += 1.0

# log last line
log.set_output_interval(0)
log.output_line()

# print contents of temporary file
tmpfile.seek(0)
for eachLine in tmpfile.readlines():
    print eachLine,

