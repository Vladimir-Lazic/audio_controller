#ifndef OBSERVER_H
#define OBSERVER_H

#include <memory>
#include <vector>

class Subject;

class Observer {
public:
    virtual ~Observer() = default;

    virtual void update(const std::shared_ptr<std::vector<float>>& waveform_buffer) = 0;

protected:
    Observer() = default;
};

#endif // OBSERVER_H
