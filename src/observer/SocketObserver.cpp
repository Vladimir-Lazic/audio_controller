#include "SocketObserver.h"

#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#include <iostream>

SocketObserver::SocketObserver(const std::string& ip_addr,
    const int local_port,
    const int remote_port)
{
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socket_fd == -1) {
        throw std::runtime_error("Unable to create socket");
    }

    sockaddr_in local_addr {};
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(local_port);
    local_addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());

    auto bind_result = bind(socket_fd, (struct sockaddr*)&local_addr, sizeof(local_addr));

    if (bind_result < 0) {
        throw std::runtime_error("Unable to bind socket");
    }

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(remote_port);
    remote_addr.sin_addr.s_addr = INADDR_ANY;
}

SocketObserver::~SocketObserver()
{
    close(socket_fd);
    std::cout << "Socket closed" << std::endl;
}

void SocketObserver::update(const float& sample)
{
    sendto(socket_fd,
        reinterpret_cast<const char*>(&sample),
        sizeof(float),
        0,
        (struct sockaddr*)&remote_addr,
        sizeof(remote_addr));
}

std::optional<AudioTask> SocketObserver::listen()
{
    sockaddr_in sender_addr {};
    socklen_t sender_len = sizeof(sender_addr);
    char data_buffer[1024];
    memset(data_buffer, 0, sizeof(data_buffer));

    size_t data_len = recvfrom(socket_fd,
        data_buffer,
        sizeof(data_buffer) - 1,
        0,
        (struct sockaddr*)&sender_addr,
        &sender_len);

    if (data_len <= 0) {
        return std::nullopt;
    }

    data_buffer[data_len] = '\0';

    return input_handler.process(data_buffer);
}
