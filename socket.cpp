#include "socket.h"

#include <sys/socket.h>
#include <stdexcept>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <cassert>
#include <iostream>
#include <errno.h>

Socket::Socket(Domain _domain, Type _type, Protocol _protocol)
{
    m_fileDescriptor = socket(
                static_cast<int>(_domain),
                static_cast<int>(_type),
                static_cast<int>(_protocol)
                );

    if (m_fileDescriptor < 0) {
        throw std::runtime_error("Failed to create socket");
    }
}

Socket::~Socket()
{
    if (isConnected()) {
        try {
            disconnect();
        }
        catch (std::logic_error & e) {

        }
    }

    int closeResult = close(m_fileDescriptor);

    assert(closeResult == 0);
}

const char * errorToReasonString(int err) {
    switch (err) {
    case EACCES:
    case EPERM:
        return "Access denied";
    case EADDRINUSE:
        return "Local address is already in use.";
    case EAFNOSUPPORT:
        return "Unsupported address family";
    case EAGAIN:
        return "Insufficient entries in the routing cache.";
    case EALREADY:
        return "Socket is not ready";
    case EBADF:
        return "Invalid file descriptor";
    case ECONNREFUSED:
        return "Connection refused";
    case ECONNRESET:
        return "Connection reset by peer";
    case ENOTCONN:
        return "Connection was not established";
    default:
        return "Other error";
        break;
    }
}

void Socket::disconnect() {
    if (! isConnected()) {
        throw std::logic_error("Error: Socket is not connected");
    }

    int shutdownResult = shutdown(m_fileDescriptor, SHUT_RDWR);
    if (shutdownResult != 0) {
        throw std::logic_error("Failed to shutdown socket");
    }

    m_isConnected = false;
}

void Socket::connect(const std::string & _host, uint16_t port) {
    if (isConnected()) {
        throw std::runtime_error("Failed: Already connected");
    }

    struct hostent * pHostInfo = gethostbyname(_host.c_str());

    if (pHostInfo == NULL) {
        throw std::runtime_error("Failed to resolve host");
    }

    assert (pHostInfo->h_addr_list[0] != NULL);

    m_sockAddr.sin_family = pHostInfo->h_addrtype;
    m_port = port;
    m_sockAddr.sin_port = htons(port);

    struct in_addr * pAddr = (in_addr *)(pHostInfo->h_addr_list[0]);
    m_sockAddr.sin_addr = *pAddr;

    int connectionResult = ::connect(m_fileDescriptor, (sockaddr *)(&m_sockAddr), sizeof(m_sockAddr));

    if (connectionResult != 0) {
        throw std::runtime_error(errorToReasonString(errno));
    }

    m_isConnected = true;
}

size_t Socket::recv(void *_buffer, size_t _count, int _flags) {
    ssize_t result = ::recv(m_fileDescriptor, _buffer, _count, _flags);
    if (result < 0) {
        throw std::runtime_error(errorToReasonString(errno));
    }
    return result;
}

size_t Socket::send(const void *_buffer, size_t _count, int _flags) {
     ssize_t result = ::send(m_fileDescriptor, _buffer, _count, _flags);
     if (result < 0) {
         throw std::runtime_error(errorToReasonString(errno));
     }
     return result;
}
