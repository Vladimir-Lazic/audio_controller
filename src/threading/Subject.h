#ifndef SUBJECT_H
#define SUBJECT_H

#include <algorithm>
#include <vector>

class Observer;

class Subject {
public:
    virtual ~Subject() = default;

    virtual void attach(const Observer& o)
    {
        observers.emplace_back(o);
    }
    virtual void detach(const Observer& o)
    {
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [o](const Observer& obs) { return obs == o; }),
            observers.end());
    }
    virtual void notify()
    {
        std::for_each(observers.begin(), obervers.end(),
            [](const Observer& obs) { obs.update(this); })
    }

protected:
    Subject() = default;

private:
    std::vector<Observer> observers;
};

#endif // SUBJECT_H
