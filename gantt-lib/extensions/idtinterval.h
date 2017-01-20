#ifndef IDTINTERVAL_H
#define IDTINTERVAL_H

#include "utcdatetime.h"

class IDtInterval
{
public:
    virtual ~IDtInterval();

    virtual UtcDateTime left() const = 0;
    virtual UtcDateTime right() const = 0;
    virtual UtcDateTime min() const = 0;
    virtual UtcDateTime max() const = 0;

    virtual void setLeft(const UtcDateTime &dt) = 0;
    virtual void setRight(const UtcDateTime &dt) = 0;
    virtual void setMin(const UtcDateTime &dt) = 0;
    virtual void setTimeSpan(const TimeSpan &ts) = 0;
};

#endif // IDTINTERVAL_H
