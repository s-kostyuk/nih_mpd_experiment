#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cinttypes>

class Socket
{


public:
    enum class Domain {
        INET = AF_INET, INET6 = AF_INET6
    };

    enum class Type {
        TCP = SOCK_STREAM, UDP = SOCK_DGRAM
    };

    enum class Protocol {
        IP = 0
    };

    Socket(Domain _domain = Domain::INET, Type _type = Type::TCP, Protocol _protocol = Protocol::IP);
    Socket(const Socket &) = delete;

    Socket & operator = (const Socket &) = delete;

    ~Socket();

    void connect(const std::string & _host, uint16_t port);
    void disconnect();

    size_t read(void * _buffer, size_t _count) { return recv(_buffer, _count, 0); }
    size_t write(void * _buffer, size_t _count) { return send(_buffer, _count, 0); }

    template <typename T>
    size_t read(T & _buffer) { return recv(_buffer, 0); }
    template <typename T>
    size_t write(T & _buffer) { return send(_buffer, 0); }

    bool isConnected() const { return m_isConnected; }
    const std::string & getHost() const { return m_host; }
    int getPort() const { return m_port; }

private:
    int m_fileDescriptor;
    bool m_isConnected = false;
    std::string m_host;
    int m_port;

    struct sockaddr_in m_sockAddr;

    size_t recv(void * _buffer, size_t _count, int _flags);
    size_t send(const void * _buffer, size_t _count, int _flags);

    template <typename T>
    size_t recv(T & _buffer, int _flags) {
        return recv(_buffer.data(), _buffer.size(), _flags);
    }

    template <typename T>
    size_t send(const T & _buffer, int _flags) {
        return send(_buffer.data(), _buffer.size(), _flags);
    }
};

#endif // SOCKET_H
