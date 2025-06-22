#ifndef PYTHON_OBSERVER_H
#define PYTHON_OBSERVER_H

#include "Observer.h"

#include <netinet/in.h>
#include <sys/socket.h>

class SocketObserver : public Observer {
public:
    void update(const WaveformBuffer&) override;

    std::optional<std::string> listen() override;

    SocketObserver(const std::string& ip_addr,
        const int local_port,
        const int remote_port);
    ~SocketObserver();

private:
    int socket_fd;
    sockaddr_in remote_addr;
};

#endif // PYTHON_OBSERVER_H