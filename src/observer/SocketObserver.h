#ifndef PYTHON_OBSERVER_H
#define PYTHON_OBSERVER_H

#include "AudioTaskBuilder.h"
#include "Observer.h"

#include <netinet/in.h>
#include <optional>
#include <sys/socket.h>

class SocketObserver : public Observer {
public:
    SocketObserver();
    ~SocketObserver();

    void update(const std::shared_ptr<std::vector<float>>&) override;

    std::optional<AudioTask> getSocketTask() const;

private:
    int client_socket;
    sockaddr_in server_address;
};

#endif // PYTHON_OBSERVER_H