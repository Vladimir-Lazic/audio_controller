#ifndef PYTHON_OBSERVER_H
#define PYTHON_OBSERVER_H

#include "InputHandler.h"
#include "Observer.h"

#include <netinet/in.h>
#include <optional>
#include <sys/socket.h>

class SocketObserver : public Observer {
public:
    SocketObserver(const std::string& ip_addr,
        const int local_port,
        const int remote_port);
    ~SocketObserver();

    void update(const WaveformBuffer&) override;

    std::optional<AudioTask> listen() override;

private:
    int socket_fd;
    sockaddr_in remote_addr;

    InputHandler input_handler;
};

#endif // PYTHON_OBSERVER_H