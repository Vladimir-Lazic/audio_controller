#ifndef PYTHON_OBSERVER_H
#define PYTHON_OBSERVER_H

#include "Observer.h"
#include "ThreadPool.h"

#include <netinet/in.h>
#include <sys/socket.h>

class PythonObserver : public Observer {
public:
    PythonObserver();
    ~PythonObserver();

    void update(const std::shared_ptr<std::vector<float>>&) override;

private:
    int client_socket;
    sockaddr_in server_address;

    enum Threads : int32_t { Size = 1 };
    ThreadPool thread_pool { Threads::Size };
};

#endif // PYTHON_OBSERVER_H