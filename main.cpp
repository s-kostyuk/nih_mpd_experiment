#include <iostream>
#include <array>
#include "mpd_player.h"

int main() {

    MPDPlayer player;
    player.connect();

    std::cout << player.getMPDVersion() << '\n';

    player.play();
    player.stop();
    player.play();

    std::cout << player.status();

    return 0;
}
