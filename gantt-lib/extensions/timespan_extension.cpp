#include "timespan_extension.h"


TimeSpan operator*(const TimeSpan &ts, const qreal d)
{
    return TimeSpan((long long)(ts.totalMicroseconds() * d + 0.5));
}

TimeSpan operator*(const qreal d, const TimeSpan &ts)
{
    return ts * d;
}

TimeSpan operator/(const TimeSpan &ts, const qreal d)
{
    return ts * (1. / d);
}

bool timeSpanIsValid(const TimeSpan &ts)
{
    return ts.totalMicroseconds();
}
