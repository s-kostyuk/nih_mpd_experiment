#ifndef MPD_PLAYER_H
#define MPD_PLAYER_H

#include <string>
#include <sstream>
#include <array>
#include "socket.h"
#include <map>
#include <memory>

class MPDPlayer
{
public:
    MPDPlayer(const std::string & _host = "localhost", int _port = 6600);

    void connect();
    void disconnect();
    std::string sendCommand(const std::string & _cmd);

    void play() { sendCommand("play\n"); }
    void stop() { sendCommand("stop\n"); }
    void pause() { sendCommand("pause\n"); }
    void next() { sendCommand("next\n"); }
    void prev() { sendCommand("prev\n"); }

    std::string status() { return sendCommand("status\n"); }

    bool isConnected() { return m_socket.isConnected(); }
    const std::string & getMPDVersion();

private:
    const std::string m_host;
    const int m_port;
    std::array<char, 1024> m_buffer;

    Socket m_socket;
    std::unique_ptr<std::string> m_pMPDVersion { nullptr };
};

#endif // MPD_PLAYER_H
