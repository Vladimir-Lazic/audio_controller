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

    timeval timeout {}; // brief timeout so thread can check if alive status
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;
    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

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

void SocketObserver::update(const WaveformBuffer& waveform_buffer)
{
    sendto(socket_fd,
        reinterpret_cast<const char*>(waveform_buffer.data()),
        waveform_buffer.size() * sizeof(float),
        0,
        (struct sockaddr*)&remote_addr,
        sizeof(remote_addr));
}

std::optional<std::string> SocketObserver::listen()
{
    sockaddr_in sender_addr {};
    socklen_t sender_len = sizeof(sender_addr);
    char data_buffer[1024];
    memset(data_buffer, 0, sizeof(data_buffer));

    ssize_t data_len = recvfrom(socket_fd,
        data_buffer,
        sizeof(data_buffer) - 1,
        0,
        (struct sockaddr*)&sender_addr,
        &sender_len);

    if (data_len <= 0) {
        return std::nullopt;
    }

    data_buffer[data_len] = '\0';

    return std::string(data_buffer, data_len);
}