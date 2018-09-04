#include "mpd_player.h"

bool contains(const std::string & _str, const std::string & _fragment, size_t startPos) {
    return  (_str.size() >= (_fragment.size() + startPos))
            && (_str.compare(startPos, _fragment.size() + startPos, _fragment) == 0);
}

bool endsWith(const std::string &_str, const std::string &_suffix) {
    return contains(_str, _suffix, _str.size() - _suffix.size());
}

bool startsWith(const std::string & _str, const std::string & _prefix) {
    return contains(_str, _prefix, 0);
}


MPDPlayer::MPDPlayer(const std::string & _host, int _port)
    : m_host(_host), m_port(_port), m_socket(Socket::Domain::INET, Socket::Type::TCP, Socket::Protocol::IP)
{

}



const std::string & MPDPlayer::getMPDVersion() {
    if (! m_pMPDVersion) {
        throw std::logic_error("Connection was not established");
    }

    return * m_pMPDVersion;
}

void MPDPlayer::connect() {
    m_socket.connect(m_host, m_port);
    size_t countReceived = m_socket.read(m_buffer);
    std::string result { m_buffer.begin(), m_buffer.begin() + countReceived };

    const std::string prefix { "OK MPD " };

    if (! startsWith(result, prefix)) {
        throw std::logic_error("Server returned an error - not MPD server?");
    }

    m_pMPDVersion = std::make_unique<std::string>(result.begin() + prefix.size(), result.end() - 1);
}

void MPDPlayer::disconnect() {
    m_socket.disconnect();
}



std::string MPDPlayer::sendCommand(const std::string &_cmd) {
    m_socket.write(_cmd);
    size_t countReceived = m_socket.read(m_buffer);

    std::string str(m_buffer.begin(), m_buffer.begin() + countReceived);

    const std::string okSuffix { "OK\n" };

    if (! endsWith(str, okSuffix)) {
        throw std::logic_error("Command was not accepted");
    }

    return str;
}
