#include "audio.h"
#include <iostream>
#include <SFML/Audio.hpp>
#include <random>
using namespace std;
mt19937 Rng(random_device{}());

void playJackpot(){
    sf::SoundBuffer buffer;
    if(!buffer.loadFromFile("Sounds/I_Just_hit_jackpot.wav")){
        cout<<"Failed to load audio\n";
        exit(0);
    }
    sf::Sound sound(buffer);
    sound.play();
    while(sound.getStatus() == sf::Sound::Status::Playing){
        sf::sleep(sf::milliseconds(100));
    }
    
}
void playShoot(){
    sf::SoundBuffer buffer;
    if(!buffer.loadFromFile("Sounds/Shoot.wav")){
        cout<<"Failed to load shoot audio\n";
        exit(0);
    }
    sf::Sound sound(buffer);
    sound.play();
    while(sound.getStatus() == sf::Sound::Status::Playing){
        sf::sleep(sf::milliseconds(100));
    }
}
static void playReload(){
    sf::SoundBuffer buffer;
    if(!buffer.loadFromFile("Sounds/Reload.wav")){
        cout<<"Failed to load reload sound\n";
        exit(0);
    }
    sf::Sound sound(buffer);
    sound.play();
    while(sound.getStatus() == sf::Sound::Status::Playing) sf::sleep(sf::milliseconds(100));
}
static void playDiceRoll(){
    sf::SoundBuffer buffer;
    if(!buffer.loadFromFile("Sounds/Dice_Roll.wav")){
        cout<<"Failed to load roll sound\n";
        exit(0);
    }
    sf::Sound sound(buffer);
    sound.play();
    while(sound.getStatus() == sf::Sound::Status::Playing) sf::sleep(sf::milliseconds(100));
}
static void playSlotMachine(){
    sf::SoundBuffer buffer;
    if(!buffer.loadFromFile("SOunds/slot_machine.wav")){
        cout<<"Failed to load slot machine sound\n";
        exit(0);
    }
    sf::Sound sound(buffer);
    sound.play();
    while(sound.getStatus() == sf::Sound::Status::Playing) sf::sleep(sf::milliseconds(100));
}
static void playShuffle(){
    sf::SoundBuffer buffer;
    if(!buffer.loadFromFile("Sounds/shuffle.wav")){
        cout<<"Failed to load shuffle\n";
        exit(0);
    }
    sf::Sound sound(buffer);
    sound.play();
    while(sound.getStatus() == sf::Sound::Status::Playing) sf::sleep(sf::milliseconds(100));
}


