import itlog

log = itlog.IlLog()

data_max = 30.0;
data = 0.0;
while data < data_max:
    log.entry(0, "%8s", "last", "%8.0f", data, itlog.IlLog.USE_LAST)
    log.entry(0, "%8s", "average", "%8.1f", data, itlog.IlLog.USE_LAST)
    log.entry(0, "%8s", "minimum", "%8.0f", data, itlog.IlLog.USE_LAST)
    log.entry(0, "%8s", "maximum", "%8.0f", data, itlog.IlLog.USE_LAST)
    log.entry(0, "%11s", "[0,1]", "%8.2e", data / (data_max - 1), \
            itlog.IlLog.USE_LAST | itlog.IlLog.UNIT_INTERVAL)
    log.output_line()
    data += 1.0

