#!/usr/bin/env python

from itlog import IlLog
from time import sleep

log = IlLog()

data_max = 30.0;
data = 0.0;
log.set_output_interval (5)
while data < data_max:
    log.entry (0, "%8s", "last", "%8.0f", data, IlLog.USE_LAST)
    log.entry (0, "%8s", "average", "%8.1f", data, IlLog.USE_AVERAGE)
    log.entry (0, "%8s", "minimum", "%8.0f", data, IlLog.USE_MIN)
    log.entry (0, "%8s", "maximum", "%8.0f", data, IlLog.USE_MAX)
    log.entry (0, "%8s", "sum", "%8.0f", data, IlLog.USE_SUM)
    log.entry (0, "%11s", "[0,1]", "%8.2e", data / (data_max - 1.0), \
            IlLog.USE_LAST | IlLog.UNIT_INTERVAL)
    sleep (0.003)
    log.output_line ()
    data += 1.0

log.set_output_interval (0)
log.output_line ()

