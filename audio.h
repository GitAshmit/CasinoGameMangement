#ifndef AUDIO_H
#define AUDIO_H
#include <string>
#include <vector>

    void playJackpot();
    void playShoot();
    void playReload();
    void playDiceRoll();
    void playSlotMachine(std::string a, std::string b, std::string c, std::vector <std::string> &symbols);
    void playShuffle();
    void playHorse();
#endif