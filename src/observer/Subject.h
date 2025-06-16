#ifndef SUBJECT_H
#define SUBJECT_H

#include <algorithm>
#include <vector>

class Observer;

class Subject {
public:
    virtual ~Subject() = default;

    virtual void attach(Observer* o)
    {
        observers.emplace_back(o);
    }

    virtual void detach(Observer* o)
    {
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [&o](Observer* obs) { return (obs == o) || (obs == nullptr); }),
            observers.end());
    }

    virtual void notify(const std::shared_ptr<std::vector<float>>& waveform_buffer)
    {
        std::for_each(observers.begin(), observers.end(),
            [waveform_buffer](Observer* obs) {
                if (obs == nullptr) {
                    return;
                }
                obs->update(waveform_buffer);
            });
    }

protected:
    Subject() = default;

private:
    std::vector<Observer*> observers;
};

#endif // SUBJECT_H