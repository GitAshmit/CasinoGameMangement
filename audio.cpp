#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <SFML/Audio.hpp>
#include <vector>
#include "audio.h"
using namespace std;
mt19937 Rng(random_device{}());

void playJackpot(){
    uniform_int_distribution <int> jack(1,3);
    static sf::SoundBuffer buffer1;
    static sf::SoundBuffer buffer2;
    static sf::SoundBuffer buffer3;
    bool load1 = false, load2 = false, load3 = false;
    int r = jack(Rng);
    cout<<"\n-----------\nJACKPOT!!!!!\n-----------\n";
    if(r == 1){
        if(!load1){
            if(!buffer1.loadFromFile("Sounds/I_Just_hit_jackpot.wav")){
                cout<<"Failed to load jackpot\n";
                return;
            }
            load1 = true;
        }
        sf::Sound sound(buffer1);
        sound.play();
        while(sound.getStatus() == sf::Sound::Status::Playing) sf::sleep(sf::milliseconds(100));
    }
    else if(r == 2){
        if(!load2){
            if(!buffer2.loadFromFile("Sounds/Jackpot.wav")){
                cout<<"Failed to load jackpot\n";
                return;
            }
            load2 = true;
        }
        sf::Sound sound(buffer2);
        sound.play();
        while(sound.getStatus() == sf::Sound::Status::Playing) sf::sleep(sf::milliseconds(100));
    }
    else{
        if(!load3){
            if(!buffer3.loadFromFile("Sounds/jackpot_2.wav")){
                cout<<"Failed to load jackpot\n";
                return;
            }
            load3 = true;
        }
        sf::Sound sound(buffer3);
        sound.play();
        while(sound.getStatus() == sf::Sound::Status::Playing) sf::sleep(sf::milliseconds(100));
    }
    
}
void playShoot(){
    static sf::SoundBuffer buffer;
    bool load = false;
    if(!load){
        if(!buffer.loadFromFile("Sounds/Shoot.wav")){
            cout<<"Failed to load shoot audio\n";
            exit(0);
        }
        load = true;
    }
    sf::Sound sound(buffer);
    sound.play();
    while(sound.getStatus() == sf::Sound::Status::Playing){
        sf::sleep(sf::milliseconds(100));
    }
} 
void playReload(){
    static sf::SoundBuffer buffer;
    bool load = false;
    if(!load){
        if(!buffer.loadFromFile("Sounds/Reload.wav")){
            cout<<"Failed to load reload sound\n";
            exit(0);
        }
        load = true;
    }   
    sf::Sound sound(buffer);
    sound.play();
    while(sound.getStatus() == sf::Sound::Status::Playing) sf::sleep(sf::milliseconds(100));
}
void playDiceRoll(){
    static sf::SoundBuffer buffer;
    bool load = false;
    if(!load){
        if(!buffer.loadFromFile("Sounds/Dice_Roll.wav")){
            cout<<"Failed to load roll sound\n";
            exit(0);
        }
        load = true;
    }
    sf::Sound sound(buffer);
    sound.play();
    while(sound.getStatus() == sf::Sound::Status::Playing) sf::sleep(sf::milliseconds(100));
}
void playSlotMachine(string a, string b, string c,vector <string> &symbols){
    uniform_int_distribution <int> slot(0,symbols.size()-1);
    static sf::SoundBuffer buffer;
    bool load = false;
    if(!load){
        if(!buffer.loadFromFile("Sounds/slot_machine.wav")){
            cout<<"Failed to load slot machine sound\n";
            exit(0);
        }
        load = true;
    }
    sf::Sound sound(buffer);
    sound.play();
    for(int i =0; i<5; i++){
        cout<<"\r"<<symbols[slot(Rng)]<<" | "<<symbols[slot(Rng)]<<" | "<<symbols[slot(Rng)]<<flush;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    for(int i =0; i<5; i++){
        cout<<"\r"<<a<<" | "<<symbols[slot(Rng)]<<" | "<<symbols[slot(Rng)]<<flush;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    for(int i =0; i<5; i++){
        cout<<"\r"<<a<<" | "<<b<<" | "<<symbols[slot(Rng)]<<flush;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}
void playShuffle(){
    static sf::SoundBuffer buffer;
    bool load = false;
    if(!load){
        if(!buffer.loadFromFile("Sounds/shuffle.wav")){
            cout<<"Failed to load shuffle\n";
            exit(0);
        }
        load = true;
    }
    sf::Sound sound(buffer);
    sound.play();
    while(sound.getStatus() == sf::Sound::Status::Playing) sf::sleep(sf::milliseconds(100));
}
void playHorse(){
    static sf::SoundBuffer buffer;
    bool load = false;
    if(!load){
        if(!buffer.loadFromFile("Sounds/horse.wav")){
            cout<<"Failed to load horse neigh sound\n";
            return;
        }
        load = true;
    }
    sf::Sound sound(buffer);
    sound.play();
    while(sound.getStatus() == sf::Sound::Status::Playing) sf::sleep(sf::milliseconds(100));
}


