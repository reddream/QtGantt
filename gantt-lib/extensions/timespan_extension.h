#ifndef TIMESPAN_EXTENSION_H
#define TIMESPAN_EXTENSION_H

#include "gantt-lib_global.h"
#include "utcdatetime.h"

bool timeSpanIsValid(const TimeSpan &ts);

TimeSpan GANTTLIBSHARED_EXPORT operator*(const TimeSpan &ts, const qreal d);
TimeSpan GANTTLIBSHARED_EXPORT operator*(const qreal d, const TimeSpan &ts);
TimeSpan GANTTLIBSHARED_EXPORT operator/(const TimeSpan &ts, const qreal d);


#endif // TIMESPAN_EXTENSION_H
