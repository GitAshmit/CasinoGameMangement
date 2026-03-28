#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cmath>
#include <chrono>
#include <thread>
using namespace std;
mt19937 rng(random_device{}());
class User
{
protected:
    string name;
    int CID;
    int luck;
    float amountWon;
    vector<bool> streak;

public:
    User(string n, int c, int l, float a)
    {
        name = n;
        CID = c;
        luck = 30;
        amountWon = a;
    }
    void display() const
    {
        cout << "Name :" << name << "\nCID :" << CID << "Amount :" << amountWon << endl;
    }
    int getLuck() const
    {
        return luck;
    }
    float getAmount() const
    {
        return amountWon;
    }
    void updateAmount(float f)
    {
        amountWon += f;
    }
    void updateLuck(int l)
    {
        luck += l;
        if (luck > 100)
            luck = 70;
        if (luck < 0)
            luck = 30;
    }
    void updateStreak(bool a)
    {
        streak.push_back(a);
        int N = streak.size();
        if (N >= 3)
        {
            if (streak[N - 3] == 0 && streak[N - 1] == 0 && streak[N - 2] == 0)
                updateLuck(25);
        }
        if (N > 6)
            streak.erase(streak.begin());
    }
};
class Game
{
protected:
    string gameName;
    int points, bet;
    User *u = nullptr;

public:
    virtual void gameInfo() = 0;
    void showGameName() const
    {
        cout << "Game Name :" << gameName << endl;
    }
    void showPoints() const
    {
        cout << "Points :" << points << endl;
    }
    void placeBet()
    {
        while (true)
        {
            int f = 1;
            cout << "Enter amount to bet :";
            cin >> bet;
            if (bet > u->getAmount())
            {
                cout << "Insufficient Balance!!!\n";
                f = 0;
            }
            if (f)
                break;
        }
    }
};
class diceGuess : public Game
{
    uniform_int_distribution<int> dice;
    uniform_int_distribution<int> chance_list;

public:
    void gameInfo()
    {
        cout << "Dice Guessing!!!\nIn this game the computer would roll 2 dice and a user is asked for a choice between 2 to 12 if the Choice matches the user wins 50% of the bet\n";
    }
    diceGuess(User &U) : dice(1, 6), chance_list(0, 99)
    {
        u = &U;
    }
    void playDiceGame()
    {
        while (true)
        {
            placeBet();
            int ch, F;
            while (true)
            {
                bool f = 1;
                cout << "Enter Your Guess :";
                cin >> ch;
                if (ch < 2 || ch > 12)
                {
                    cout << "Invalid Choice!!!\n";
                    f = 0;
                }
                if (f)
                    break;
            }
            int roll = dice(rng) + dice(rng);
            int chance = chance_list(rng);
            if (chance < (u->getLuck()))
            {
                if (roll > ch)
                    roll--;
                else if (roll < ch)
                    roll++;
                roll = max(2, min(12, roll));
            }
            cout << "Computer Guess :" << roll;
            if (abs(ch - roll) == 2)
            {
                cout << "Close Guess!!!\nAmount Won = " << (bet + bet * 0.1) << "!!!\n";
                u->updateAmount(bet * 0.1);
                u->updateLuck(-20);
                u->updateStreak(0);
            }
            else if (abs(ch - roll) == 1)
            {
                cout << "Very Close!!!!\nAmount Won = " << (bet + bet * 0.15) << "!!!\n";
                u->updateAmount(bet * 0.15);
                u->updateLuck(-35);
                u->updateStreak(0);
            }
            else if (ch == roll)
            {
                cout << "\n---------------\nJACKPOT!!!!!\n---------------\nAmount Won = " << (bet + bet * 0.5) << "!!!\n";
                u->updateAmount(bet * 0.5);
                u->updateLuck(-80);
                u->updateStreak(1);
            }
            else
            {
                cout << "Well Tried!!!\nAmount Loss = " << bet << endl;
                u->updateAmount(-bet);
                u->updateLuck(20);
                u->updateStreak(0);
            }
            cout << "Wanna Try Again ? I can sense the Luck Pumping!!! (Enter 1 for yes and 0 for no) :";
            cin >> F;
            if (F == 0)
                break;
        }
    }
};
class slotMachine : public Game{
    uniform_int_distribution <int> slot;
    uniform_int_distribution <int> chance_list;
    vector <string> symbols = {"🥀","🫦","💔","7","🍕","🗣️"};
    public:
    slotMachine(User &U) : slot(0,5), chance_list(0,99){
        u = &U;
    }
    void gameInfo(){
        cout<<"Slot Machine!!!\nPlace the bet in the slot and check your luck\n";
    }
    void playSlot(){
        while(true){
            placeBet();
            string a , b , c;
            int A , B , C, F , chance = chance_list(rng);
            for(int i =0; i<5; i++){
                cout<<"\r"<<symbols[slot(rng)]<<" | "<<symbols[slot(rng)]<<" | "<<symbols[slot(rng)]<<flush;
                this_thread::sleep_for(chrono::milliseconds(100));
            }
            A = slot(rng);
            a = symbols[A];
            for(int i =0; i<5; i++){
                cout<<"\r"<<a<<" | "<<symbols[slot(rng)]<<" | "<<symbols[slot(rng)]<<flush;
                this_thread::sleep_for(chrono::milliseconds(100));
            }
            B = slot(rng);
            if(chance < u->getLuck()){
                if(abs(B-A)>1) B=A;
            }
            b = symbols[B];
            for(int i =0; i<5; i++){
                cout<<"\r"<<a<<" | "<<b<<" | "<<symbols[slot(rng)]<<flush;
                this_thread::sleep_for(chrono::milliseconds(100));
            }
            C = slot(rng);
            if(chance < u->getLuck()){
                if(abs(C-A)>1 && abs(B-C)>1) C=A;
            }
            c = symbols[C];
            cout<<"\r"<<a<<" | "<<b<<" | "<<c<<endl;
            if(a==b && b==c){
                cout<<"\n---------------JACKPOT!!!!!!\n---------------\nWon : "<<bet+bet*0.4<<endl;
                u->updateAmount(bet*0.4);
                u->updateStreak(1);
            }
            else if(a==b || b==c || a==c){
                cout<<"Very Close!!!\nWon : "<<bet+bet*0.1<<endl;
                u->updateAmount(bet*0.1);
                u->updateStreak(0);
            }
            else{
                cout<<"The Next Spin holds a JACKPOT!\nLoss : "<<bet<<endl;
                u->updateAmount(-bet);
                u->updateStreak(0);
            }
            cout<<"Wanna Play Again ? The Luck Engine is RUNNING!!! (Enter 1 for yes, 0 for no) :";
            cin>>F;
            if(!F){
                break;
            }
        }
    }
};