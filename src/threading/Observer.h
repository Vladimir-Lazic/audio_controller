#ifndef OBSERVER_H
#define OBSERVER_H

#include <cstdlib>

class Subject;

class Observer {
public:
    virtual ~Observer() = default;

    virtual void update(const Subject& subject) = 0;

    bool operator==(const Observer& rhs)
    {
        return (unique_id == rhs.unique_id);
    }

protected:
    Observer()
        : unique_id(static_cast<int32_t>(rand()))
    {
    }

private:
    int32_t unique_id { -1 };
};

#endif // OBSERVER_H
