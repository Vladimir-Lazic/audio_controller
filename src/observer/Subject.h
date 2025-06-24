#ifndef SUBJECT_H
#define SUBJECT_H

#include <algorithm>
#include <memory>
#include <vector>

class Observer;

class Subject {
public:
    virtual ~Subject() = default;

    virtual void attach(const std::shared_ptr<Observer>& observer)
    {
        observers.emplace_back(observer);
    }

    virtual void detach(const std::shared_ptr<Observer>& observer)
    {
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [remove_candidate = observer](std::weak_ptr<Observer>& observer) {
                    if (auto obs = observer.lock()) {
                        return (remove_candidate == obs) || (obs == nullptr);
                    }
                    return false;
                }),
            observers.end());
    }

    virtual void notify(const float& sample)
    {
        std::for_each(observers.begin(), observers.end(),
            [sample = sample](std::weak_ptr<Observer>& observer) {
                if (auto obs = observer.lock()) {
                    obs->update(sample);
                }
            });
    }

protected:
    Subject() = default;

private:
    std::vector<std::weak_ptr<Observer>> observers;
};

#endif // SUBJECT_H