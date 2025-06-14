#include "SocketObserver.h"

#include <iostream>

SocketObserver::SocketObserver()
{
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (client_socket == -1) {
        throw std::runtime_error("Unable to create socket");
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;
}

SocketObserver::~SocketObserver()
{
    close(client_socket);
}

void SocketObserver::update(const std::shared_ptr<std::vector<float>>& waveform_buffer)
{
    sendto(client_socket,
        reinterpret_cast<const char*>(waveform_buffer->data()),
        waveform_buffer->size() * sizeof(float),
        0,
        (struct sockaddr*)&server_address,
        sizeof(server_address));
}

std::optional<AudioTask> SocketObserver::getSocketTask() const
{
    return std::nullopt;
}