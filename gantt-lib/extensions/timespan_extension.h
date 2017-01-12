#ifndef TIMESPAN_EXTENSION_H
#define TIMESPAN_EXTENSION_H

#include "gantt-lib_global.h"
#include "utcdatetime.h"

bool timeSpanIsValid(const TimeSpan &ts);

TimeSpan GANTTLIBSHARED_EXPORT operator*(const TimeSpan &ts, double d);
TimeSpan GANTTLIBSHARED_EXPORT operator*(double d, const TimeSpan &ts);
TimeSpan GANTTLIBSHARED_EXPORT operator/(const TimeSpan &ts, double d);


#endif // TIMESPAN_EXTENSION_H
