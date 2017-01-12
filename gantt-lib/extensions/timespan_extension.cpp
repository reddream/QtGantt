#include "timespan_extension.h"


TimeSpan operator*(const TimeSpan &ts, double d)
{
    return TimeSpan((long long)(ts.totalMicroseconds() * d + 0.5));
}

TimeSpan operator*(double d, const TimeSpan &ts)
{
    return ts * d;
}

TimeSpan operator/(const TimeSpan &ts, double d)
{
    return ts * (1. / d);
}

bool timeSpanIsValid(const TimeSpan &ts)
{
    return ts.totalMicroseconds();
}
