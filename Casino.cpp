#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cmath>
#include <chrono>
#include <thread>
#include <algorithm>
#include <SFML/Audio.hpp>
#include "audio.h"
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
    User(string n, int c, float a, int l=30)
    {
        name = n;
        CID = c;
        luck = l;
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
            playDiceRoll();
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
            cout << "Computer Guess :" << roll<<endl;
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
                playJackpot();
                cout<<"Amount Won = " << (bet + bet * 0.5) << "!!!\n";
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
            A = slot(rng);
            a = symbols[A];
            B = slot(rng);
            if(chance < u->getLuck()){
                if(abs(B-A)>1) B=A;
            }
            b = symbols[B];
            C = slot(rng);
            if(chance < u->getLuck()){
                if(abs(C-A)>1 && abs(B-C)>1) C=A;
            }
            c = symbols[C];
            playSlotMachine(a,b,c,symbols);
            cout<<"\r"<<a<<" | "<<b<<" | "<<c<<endl;
            if(a==b && b==c){
                playJackpot();
                cout<<"Won : "<<bet+bet*0.4<<endl;
                u->updateAmount(bet*0.4);
                u->updateStreak(1);
                u->updateLuck(-30);
            }
            else if(a==b || b==c || a==c){
                cout<<"Very Close!!!\nWon : "<<bet+bet*0.1<<endl;
                u->updateAmount(bet*0.1);
                u->updateStreak(0);
                u->updateLuck(5);
            }
            else{
                cout<<"The Next Spin holds a JACKPOT!\nLoss : "<<bet<<endl;
                u->updateAmount(-bet);
                u->updateStreak(0);
                u->updateLuck(15);
            }
            cout<<"Wanna Play Again ? The Luck Engine is RUNNING!!! (Enter 1 for yes, 0 for no) :";
            cin>>F;
            if(!F){
                break;
            }
        }
    }
};
class russian_roulete : public Game
{
    int player_chamber;
    vector<int> opponentChambers;
    int playerHP;
    int opponentHP;
    float basereward;
    float luck_multiplier;
    uniform_int_distribution<int> chamber;

public:
    russian_roulete(User &U) : chamber(1, 6)
    {
        u = &U ;
        playerHP = 3;
        opponentHP = 3;
        luck_multiplier = 1.0 ;
    }
    void gameInfo() override
    {
        cout << "\nBOTH PLAYER AND COMPUTER HAVE 3 HEALTH POINTS PLAYER HAVE TO SHOOT THE COMPUTER 3 TIMES TO WIN THE GAME \nCOMPUTER WILL BE PRESENT IN 2 OUT 6 ROOMS !!\n ";
    }

    void setBaseReward()
    {
        this->placeBet() ;
        basereward = bet ;
    }

    int getPlayerShot()
    {
        int choice;
        while (true)
        {
            cout << "\nENTER WHICH ROOM (1-6) TO SHOOT IN\n\n";
            cin >> choice;
            if (choice >= 1 && choice <= 6)
                break;
            cout << "Invalid choice! Try again.\n";
        }

        return choice ;
    }

    void repositionOpponent()
    {
        opponentChambers.assign(6,0) ;
        vector<int> temp = {1,2,3,4,5,6} ;
        playReload();
        shuffle(temp.begin() , temp.end() , rng ) ;
        int x  =  temp[0] , y = temp[1] ;
        opponentChambers[x-1] = 1 ;
        opponentChambers[y-1] = 1 ;
    }

    bool evaluateShot() 
    {
        if( opponentChambers[ player_chamber-1 ]  == 1){
            playShoot();
            return true ;
        }
        else 
            return false ;
    }

    void calculateLuckMultiplier()
    {
        if( u->getLuck() > 65 )
        {
            luck_multiplier = 1.5 ;
        } 

        else if(  u->getLuck() >= 35 && u->getLuck() <= 65 )
            luck_multiplier = 1.0 ;
        
        else {
            luck_multiplier = 0.9 ;
        }
    }

    void resolveGame(bool playerWon)
    {
        if(playerWon)
        {
            u->updateAmount(basereward*luck_multiplier) ;
            u->updateStreak(1);
            u->updateLuck(-30);
        }

        else
        {
            u->updateAmount(-basereward) ;
            u->updateStreak(0);
            u->updateLuck(+20);
        }
    }

    void resetGame()
    {
        playerHP = 3 ;
        opponentHP = 3 ;
        opponentChambers.assign(6,0) ;
    }

    void playRoulette()
    {
        int replay  ;
        while(true)
    {
            cout<<"\n\n" ;
            this->gameInfo() ;
            cout<< "\n\n" ;
            this->setBaseReward() ;
            cout << "\n\nGAME STARTING\n\n" ;
            while(true)
            {
            cout << "PLAYER HP  :" << playerHP << endl ;
            cout << "OPPONENT HP  :" << opponentHP << endl ;
            this->repositionOpponent() ;
            player_chamber = this->getPlayerShot() ;
            if( this->evaluateShot() ) 
            {
                cout << "\nOpponent loses HP\n" ;
                opponentHP-- ; 
            }
            else
            {
                cout << "\nPLayer loses HP\n" ;
                playerHP-- ;
            }
            
            if(playerHP == 0)
            {
                this->resolveGame(0) ;
                cout << "\n!! PLAYER LOSES !! \n" ;
                cout << "\n AMOUNT LOST =  " << basereward  << endl ;
                break ;
            }
            
            else if( opponentHP == 0 )
            {
                this->calculateLuckMultiplier() ;
                this->resolveGame(1) ;
                playJackpot();
                cout << "\n AMOUNT WON =  " << basereward*luck_multiplier  << endl ;
                break ;
            }
            }
        
        this->resetGame() ;
        cout << "\n\nWANT TO PLAY AGAIN PRESS 1 FOR YES 0 FOR NO\n\n" ;
        cin >> replay ;
        if(!replay) break ;
        
    }


    }

};
class blackJack : public Game{
    vector <int> cards = {1,2,3,4,5,6,7,8,9,10,11,12,13};
    uniform_int_distribution <int> chance_list;
    public:
    void gameInfo(){
        cout<<"Black Jacks!!!\nA card game where the computer and player has 2 cards and the card closest to 21 wins\n";
    }
    blackJack(User &U) : chance_list(0,99){
        u = &U;
    }
    string cardValue(int c){
        if(c<11 && c>1) return to_string(c);
        if(c==1) return ("Ace");
        if(c==11) return ("Jack");
        if(c==12) return ("Queen");
        if(c==13) return ("King");
        return ("Out of Bounds");
    }
    void checkAceSum(int &count, int &sum){
        while(sum>21 && count>0){
            sum -= 10;
            count--;
        }
    }
    bool checkBlackJack(int card1, int card2){
        if((card1 == 1 && (card2>10 && card2<14)) || (card2 == 1 && (card1>10 && card1 <14))) return 1;
        return 0;
    }
    void playBlackJack(){
        while(true){
            int player = 0, dealer = 0, choice, F, aceCountDealer = 0, aceCountPlayer = 0, p = 4, dBJ = 0, pBJ = 0;
            placeBet();
            playShuffle();
            shuffle(cards.begin(), cards.end(), rng);
            if(cards[0] ==1){
                player += 10;
                aceCountPlayer++;
            }
            if(cards[2] ==1){
                player += 10;
                aceCountPlayer++;
            }
            if(cards[1] ==1){
                dealer +=10;
                aceCountDealer++;
            }
            if(cards[3] ==1){
                dealer += 10;
                aceCountDealer++;
            }
            player += cards[0] + cards[2];
            dealer += cards[1] + cards[3];
            pBJ = checkBlackJack(cards[0], cards[2]);
            dBJ = checkBlackJack(cards[1], cards[3]);
            cout<<"Your cards :"<<cardValue(cards[0])<<" "<<cardValue(cards[2])<<endl;
            cout<<"Dealer cards :"<<cardValue(cards[1])<<" Hidden\n";
            if(pBJ && !dBJ){
                cout<<"Black Jack!!!!";
                playJackpot();
                cout<<"Amount Won : "<<bet+bet*0.5<<endl;
                u->updateAmount(bet*0.5);
                u->updateStreak(1);
                u->updateLuck(-35);
                pBJ = 1;
            }
            else if(!pBJ && dBJ){
                cout<<"Dealer  hit Black Jack!!!\nAmount Lost : "<<bet<<endl;
                u->updateAmount(-bet);
                u->updateStreak(0);
                u->updateLuck(20);
            }
            else if(pBJ && dBJ){
                cout<<"Both Hit The Black Jack!!!\nDRAW!!!\n";
            }
            else{
                while(true){
                    int chance = chance_list(rng);
                    while(true){
                        cout<<"Enter you choice (1 to hit and 0 to stand) :";
                        cin>>choice;
                        if(choice == 1 || choice == 0 ) break;
                        else cout<<"Invalid choice!!!\n";
                    }
                    if(!choice) break;
                    int c = cards[p];
                    if(chance<u->getLuck() && rng()%2){
                        if(player+c >21) c = rng()%7 + 2;
                    }
                    cout<<"Your card : "<<c<<endl;
                    player += c;
                    if(c == 1){
                        player += 10;
                        aceCountPlayer++;
                    }
                    p++;
                    checkAceSum(aceCountPlayer, player);
                    cout<<"Your Cards :";
                    for(int i = 0; i<p; i+=2) cout<<cardValue(cards[i])<<" ";
                    cout<<endl;
                    if(player>21) break;
                }
                if(dealer<17){
                    while(dealer<17){
                        int chance = chance_list(rng);
                        int C = cards[p];
                        if(chance>u->getLuck() && rng()%2){
                            if(dealer+C>21) C = rng()&4 + 2;
                        }
                        dealer += C;
                        if(C ==1){
                            dealer += 10;
                            aceCountDealer++;
                        }
                        p++;
                        checkAceSum(aceCountDealer , dealer);
                    }
                    cout<<"Dealer Hits\n";
                } 
                else cout<<"Dealer Stand\n";
                cout<<"Time to reveal cards\n";
                cout<<"Dealer Sum : "<<dealer<<"\nPlayer Sum : "<<player<<endl;
                if(player>21 || (player<dealer && dealer<21)){
                    cout<<"Player Lost!\nAmount lost : "<<bet;
                    u->updateAmount(-bet);
                    u->updateStreak(0);
                    u->updateLuck(20);
                }
                else if(player>dealer || dealer>21){
                    playJackpot();
                    cout<<"Amount Won : "<<bet+bet*0.4<<endl;
                    u->updateAmount(bet*0.4);
                    u->updateStreak(1);
                    u->updateLuck(-30);
                }
                else if(player == dealer) cout<<"DRAW!!!!\n";
            }
            cout<<"Want to play again ?(1 for yes and 0 for no) : ";
            cin>>F;
            if(F==0) break;
        }
    }
};
int main(){
    User obj1("Ravi",101,20000);
    slotMachine obj(obj1);
    obj.playSlot();
}