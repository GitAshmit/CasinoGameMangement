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
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;
mt19937 rng(random_device{}());
class User
{
protected:
    string name;
    int CID;
    int luck;
    float amountWon;
    int total_points;
    vector<bool> streak;

public:
    User(string n, int c, float a, int l = 30)
    {
        name = n;
        CID = c;
        luck = l;
        amountWon = a;
        total_points = 0;
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

    void updatetotalpoints(int p)
    {
        total_points = max(0, total_points + p);
    }

    void settotalpoints(int p) { total_points = p; }

    int gettotalpoints() const { return total_points; }

    int getCID() const { return CID; }

    string getName() const { return name; }

    void saveToFile() const
    {
        ifstream fin("users.txt");
        vector<string> lines;
        string line;
        bool found = false;

        while (getline(fin, line))
        {
            if (line.empty())
                continue;

            istringstream ss(line);
            string idStr, filename, amountStr, luckStr, pointsStr;

            getline(ss, idStr, '|');
            getline(ss, filename, '|');
            getline(ss, amountStr, '|');
            getline(ss, luckStr, '|');
            getline(ss, pointsStr, '|');

            if (idStr.empty() || filename.empty() || amountStr.empty() ||
                luckStr.empty() || pointsStr.empty())
            {
                continue;
            }

            int id;
            float amount;
            int luck, points;

            try
            {
                id = stoi(idStr);
                amount = stof(amountStr);
                luck = stoi(luckStr);
                points = stoi(pointsStr);
            }
            catch (...)
            {
                continue;
            }

            if (id == CID)
            {

                lines.push_back(to_string(CID) + "|" + this->name + "|" +
                                to_string(amountWon) + "|" +
                                to_string(luck) + "|" +
                                to_string(total_points));
                found = true;
            }
            else
            {

                lines.push_back(line);
            }
        }
        fin.close();

        if (!found)
        {
            lines.push_back(to_string(CID) + "|" + this->name + "|" +
                            to_string(amountWon) + "|" +
                            to_string(luck) + "|" +
                            to_string(total_points));
        }

        ofstream fout("users.txt");
        for (string &l : lines)
            fout << l << "\n";
        fout.close();
        cout << "Progress Saved!\n";
    }

    static bool loadFromFile(int cid, User &outUser)
    {
        ifstream file("users.txt");

        if (!file)
            return false;

        string line;

        while (getline(file, line))
        {
            if (line.empty())
                continue;

            istringstream ss(line);

            string idStr, name, amountStr, luckStr, pointsStr;

            getline(ss, idStr, '|');
            getline(ss, name, '|');
            getline(ss, amountStr, '|');
            getline(ss, luckStr, '|');
            getline(ss, pointsStr, '|');

            if (idStr.empty() || name.empty() || amountStr.empty() ||
                luckStr.empty() || pointsStr.empty())
                continue;

            int id;
            float amount;
            int luck, points;

            try
            {
                id = stoi(idStr);
                amount = stof(amountStr);
                luck = stoi(luckStr);
                points = stoi(pointsStr);
            }
            catch (...)
            {
                continue;
            }

            if (id == cid)
            {
                outUser = User(name, cid, amount, luck);
                outUser.settotalpoints(points);
                return true;
            }
        }

        return false;
    }

    void logGame(string gameName, int betAmount, float result, int pts) const
    {
        ofstream log("history.txt", ios::app);
        log << "CID:" << CID
            << " | Game: " << gameName
            << " | Bet: " << betAmount
            << " | Result: " << (result >= 0 ? "WON " : "LOST ")
            << abs(result)
            << " | Points: " << pts
            << "\n";
        log.close();
    }

    static void showHistory(int cid)
    {
        ifstream file("history.txt");
        string line;
        bool found = false;
        cout << "\n--- History for CID " << cid << " ---\n";
        while (getline(file, line))
        {
            if (line.find("CID:" + to_string(cid)) != string::npos)
            {
                cout << line << "\n";
                found = true;
            }
        }
        if (!found)
            cout << "No history found.\n";
    }
    void addBalance()
    {
        double x;
        int ch;
        while (true)
        {
            cout << "Enter amount: ";
            if (cin >> x && x > 0)
                break;
            cout << "Invalid! Enter a positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Enter method of payment :\n1.Card\n2.Cash\n3.Paypall\n";
        while (true)
        {
            cin >> ch;
            if (ch > 0 && ch < 4)
                break;
            else
                cout << "Invalid choice please re-input :";
        }
        switch (ch)
        {
        case 1:
            int cardn, cardpin;
            cout << "Enter card number and card pin :";
            cin >> cardn >> cardpin;
            cout << "Processing....\n";
            this_thread::sleep_for(chrono::milliseconds(50));
            cout << "Payment succesfull !!!\n";
            break;

        case 2:
            int age;
            cout << "Enter your legal age :";
            cin >> age;
            cout << "Payment succesfull !!!\n";
            break;

        case 3:
            int acc;
            cout << "Enter account number and further continue on you phone :";
            cin >> acc;
            cout << "Processing....\n";
            this_thread::sleep_for(chrono::milliseconds(50));
            cout << "Payment succesfull !!!\n";
        }
        amountWon += x;
    }
    static int generateCID()
    {
        static mt19937 gen(random_device{}());
        uniform_int_distribution<int> dist(1000, 9999); // 4-digit CID
        return dist(gen);
    }

    static bool cidExists(int cid)
    {
        ifstream file("users.txt");
        string line;

        while (getline(file, line))
        {
            istringstream ss(line);
            string idStr;
            getline(ss, idStr, '|');

            try
            {
                if (stoi(idStr) == cid)
                    return true;
            }
            catch (...)
            {
            }
        }
        return false;
    }

    static int generateUniqueCID()
    {
        int cid;
        do
        {
            cid = generateCID();
        } while (cidExists(cid));

        return cid;
    }
};
class Game
{
protected:
    string gameName;
    int points = 0, bet;
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
            while (!(cin >> bet))
            {
                cout << "Invalid bet! Try again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (bet > u->getAmount())
            {
                cout << "Insufficient Balance!!!\n";
                f = 0;
            }
            if (bet < 10 || bet % 10 != 0)
            {
                cout << "Bet should be greater than 10 and should be a multiple of 10\n";
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
        gameName = "Dice Guess";
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
                while (!(cin >> ch))
                {
                    cout << "Invalid input! Try again: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
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
            cout << "Computer Roll :" << roll << endl;
            if (abs(ch - roll) == 2)
            {
                cout << "Close Guess!!!\nAmount Won = " << (bet + bet * 0.1) << "!!!\n";
                u->updateAmount(bet * 0.1);
                u->updateLuck(-20);
                u->updateStreak(0);
                u->updatetotalpoints(bet / 2);
                u->logGame("Dice Guess", bet, bet * 0.1, bet / 2);
            }
            else if (abs(ch - roll) == 1)
            {
                cout << "Very Close!!!!\nAmount Won = " << (bet + bet * 0.15) << "!!!\n";
                u->updateAmount(bet * 0.15);
                u->updateLuck(-35);
                u->updateStreak(0);
                u->updatetotalpoints(bet * 1);
                u->logGame("Dice Guess", bet, bet * 0.15, bet * 1);
            }
            else if (ch == roll)
            {
                playJackpot();
                cout << "Amount Won = " << (bet + bet * 0.5) << "!!!\n";
                u->updateAmount(bet * 0.5);
                u->updateLuck(-80);
                u->updateStreak(1);
                u->updatetotalpoints(bet * 2);
                u->logGame("Dice Guess", bet, bet * 0.5, bet * 2);
            }
            else
            {
                cout << "Well Tried!!!\nAmount Loss = " << bet << endl;
                u->updateAmount(-bet);
                u->updateLuck(20);
                u->updateStreak(0);
                u->updatetotalpoints(0);
                u->logGame("Dice Guess", bet, -bet, 0);
            }
            cout << "Wanna Try Again ? I can sense the Luck Pumping!!! (Enter 1 for yes and 0 for no) :";
            while (!(cin >> F))
            {
                cout << "Invalid input! Try again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (F == 0)
                break;
        }
    }
};
class slotMachine : public Game
{
    uniform_int_distribution<int> slot;
    uniform_int_distribution<int> chance_list;
    vector<string> symbols = {"1", "2", "3", "4", "5", "6"};

public:
    slotMachine(User &U) : slot(0, 5), chance_list(0, 99)
    {
        u = &U;
        gameName = "Slot Machine";
    }
    void gameInfo()
    {
        cout << "Slot Machine!!!\nPlace the bet in the slot and check your luck\n";
    }
    void playSlot()
    {
        while (true)
        {
            placeBet();
            string a, b, c;
            int A, B, C, F, chance = chance_list(rng);
            A = slot(rng);
            a = symbols[A];
            B = slot(rng);
            if (chance < u->getLuck())
            {
                if (abs(B - A) > 1)
                    B = A;
            }
            b = symbols[B];
            C = slot(rng);
            if (chance < u->getLuck())
            {
                if (abs(C - A) > 1 && abs(B - C) > 1)
                    C = A;
            }
            c = symbols[C];
            playSlotMachine(a, b, c, symbols);
            cout << "\r" << a << " | " << b << " | " << c << endl;
            if (a == b && b == c)
            {
                playJackpot();
                cout << "Won : " << bet + bet * 0.4 << endl;
                u->updateAmount(bet * 0.4);
                u->updateStreak(1);
                u->updateLuck(-30);
                u->updatetotalpoints(bet * 2);
                u->logGame("Slot Machine", bet, bet * 0.4, bet * 2);
            }
            else if (a == b || b == c || a == c)
            {
                cout << "Very Close!!!\nWon : " << bet + bet * 0.1 << endl;
                u->updateAmount(bet * 0.1);
                u->updateStreak(0);
                u->updateLuck(5);
                u->updatetotalpoints(bet);
                u->logGame("Slot Machine", bet, bet * 0.1, bet);
            }
            else
            {
                cout << "The Next Spin holds a JACKPOT!\nLoss : " << bet << endl;
                u->updateAmount(-bet);
                u->updateStreak(0);
                u->updateLuck(15);
                u->updatetotalpoints(0);
                u->logGame("Slot Machine", bet, -bet, 0);
            }
            cout << "Wanna Play Again ? The Luck Engine is RUNNING!!! (Enter 1 for yes, 0 for no) :";
            while (!(cin >> F))
            {
                cout << "Invalid input! Try again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (!F)
            {
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
        u = &U;
        playerHP = 3;
        opponentHP = 3;
        luck_multiplier = 1.0;
        gameName = "Russian Roulete";
    }
    void gameInfo() override
    {
        cout << "\nBOTH PLAYER AND COMPUTER HAVE 3 HEALTH POINTS PLAYER HAVE TO SHOOT THE COMPUTER 3 TIMES TO WIN THE GAME \nCOMPUTER WILL BE PRESENT IN 2 OUT 6 ROOMS !!\n ";
    }

    void setBaseReward()
    {
        this->placeBet();
        basereward = bet;
    }

    int getPlayerShot()
    {
        int choice;
        while (true)
        {
            cout << "\nENTER WHICH ROOM (1-6) TO SHOOT IN\n\n";
            while (!(cin >> choice))
            {
                cout << "Invalid input! Try again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (choice >= 1 && choice <= 6)
                break;
            cout << "Invalid choice! Try again.\n";
        }

        return choice;
    }

    void repositionOpponent()
    {
        opponentChambers.assign(6, 0);
        vector<int> temp = {1, 2, 3, 4, 5, 6};
        playReload();
        shuffle(temp.begin(), temp.end(), rng);
        int x = temp[0], y = temp[1];
        opponentChambers[x - 1] = 1;
        opponentChambers[y - 1] = 1;
    }

    bool evaluateShot()
    {
        playShoot();
        if (opponentChambers[player_chamber - 1] == 1)
        {
            return true;
        }
        else
            return false;
    }

    void calculateLuckMultiplier()
    {
        if (u->getLuck() > 65)
        {
            luck_multiplier = 1.5;
        }

        else if (u->getLuck() >= 35 && u->getLuck() <= 65)
            luck_multiplier = 1.0;

        else
        {
            luck_multiplier = 0.9;
        }
    }

    void resolveGame(bool playerWon)
    {
        if (playerWon)
        {
            u->updateAmount(basereward * luck_multiplier);
            u->updateStreak(1);
            u->updateLuck(-30);
            u->updatetotalpoints(bet * 2);
            u->logGame("Russian Roulette", bet, basereward * luck_multiplier, bet * 2);
        }

        else
        {
            u->updateAmount(-basereward);
            u->updateStreak(0);
            u->updateLuck(+20);
            u->updatetotalpoints(0);
            u->logGame("Russian Roulette", bet, -bet, 0);
        }
    }

    void resetGame()
    {
        playerHP = 3;
        opponentHP = 3;
        opponentChambers.assign(6, 0);
    }

    void playRoulette()
    {
        int replay;
        while (true)
        {
            cout << "\n\n";
            this->gameInfo();
            cout << "\n\n";
            this->setBaseReward();
            cout << "\n\nGAME STARTING\n\n";
            while (true)
            {
                cout << "PLAYER HP  :" << playerHP << endl;
                cout << "OPPONENT HP  :" << opponentHP << endl;
                this->repositionOpponent();
                player_chamber = this->getPlayerShot();
                if (this->evaluateShot())
                {
                    cout << "\nOpponent loses HP\n";
                    opponentHP--;
                }
                else
                {
                    cout << "\nPLayer loses HP\n";
                    playerHP--;
                }

                if (playerHP == 0)
                {
                    this->resolveGame(0);
                    cout << "\n!! PLAYER LOSES !! \n";
                    cout << "\n AMOUNT LOST =  " << basereward << endl;
                    break;
                }

                else if (opponentHP == 0)
                {
                    this->calculateLuckMultiplier();
                    this->resolveGame(1);
                    playJackpot();
                    cout << "\n AMOUNT WON =  " << basereward * luck_multiplier << endl;
                    break;
                }
            }

            this->resetGame();
            cout << "\n\nWANT TO PLAY AGAIN PRESS 1 FOR YES 0 FOR NO\n\n";
            while (!(cin >> replay))
            {
                cout << "Invalid input! Try again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (!replay)
                break;
        }
    }
};
class blackJack : public Game
{
    vector<int> cards = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    uniform_int_distribution<int> chance_list;

public:
    void gameInfo()
    {
        cout << "Black Jacks!!!\nA card game where the computer and player has 2 cards and the card closest to 21 wins but if you cross it you lose all your money!!!\n";
    }
    blackJack(User &U) : chance_list(0, 99)
    {
        u = &U;
        gameName = "Black Jacks";
    }
    string cardValue(int c)
    {
        if (c < 11 && c > 1)
            return to_string(c);
        if (c == 1)
            return ("Ace");
        if (c == 11)
            return ("Jack");
        if (c == 12)
            return ("Queen");
        if (c == 13)
            return ("King");
        return ("Out of Bounds");
    }
    void checkAceSum(int &count, int &sum)
    {
        while (sum > 21 && count > 0)
        {
            sum -= 10;
            count--;
        }
    }
    bool checkBlackJack(int card1, int card2)
    {
        if ((card1 == 1 && (card2 > 10 && card2 < 14)) || (card2 == 1 && (card1 > 10 && card1 < 14)))
            return 1;
        return 0;
    }
    void playBlackJack()
    {
        while (true)
        {
            int player = 0, dealer = 0, choice, F, aceCountDealer = 0, aceCountPlayer = 0, p = 4, dBJ = 0, pBJ = 0;
            placeBet();
            shuffle(cards.begin(), cards.end(), rng);
            playShuffle();
            if (cards[0] == 1)
            {
                player += 10;
                aceCountPlayer++;
            }
            if (cards[2] == 1)
            {
                player += 10;
                aceCountPlayer++;
            }
            if (cards[1] == 1)
            {
                dealer += 10;
                aceCountDealer++;
            }
            if (cards[3] == 1)
            {
                dealer += 10;
                aceCountDealer++;
            }
            player += cards[0] + cards[2];
            dealer += cards[1] + cards[3];
            pBJ = checkBlackJack(cards[0], cards[2]);
            dBJ = checkBlackJack(cards[1], cards[3]);
            cout << "Your cards :" << cardValue(cards[0]) << " " << cardValue(cards[2]) << endl;
            cout << "Dealer cards :" << cardValue(cards[1]) << " Hidden\n";
            if (pBJ && !dBJ)
            {
                cout << "Black Jack!!!!";
                playJackpot();
                cout << "Amount Won : " << bet + bet * 0.5 << endl;
                u->updateAmount(bet * 0.5);
                u->updateStreak(1);
                u->updateLuck(-35);
                pBJ = 1;
                u->updatetotalpoints(bet * 2);
                u->logGame("Black Jack", bet, bet * 0.5, bet * 2);
            }
            else if (!pBJ && dBJ)
            {
                cout << "Dealer  hit Black Jack!!!\nAmount Lost : " << bet << endl;
                u->updateAmount(-bet);
                u->updateStreak(0);
                u->updateLuck(20);
                u->updatetotalpoints(0);
                u->logGame("Black Jack", bet, -bet, 0);
            }
            else if (pBJ && dBJ)
            {
                cout << "Both Hit The Black Jack!!!\nDRAW!!!\n";
            }
            else
            {
                while (true)
                {
                    int chance = chance_list(rng);
                    while (true)
                    {
                        cout << "Enter you choice (1 to hit and 0 to stand) :";
                        while (!(cin >> choice))
                        {
                            cout << "Invalid input! Try again: ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        if (choice == 1 || choice == 0)
                            break;
                        else
                            cout << "Invalid choice!!!\n";
                    }
                    if (!choice)
                        break;
                    int c = cards[p];
                    if (chance < u->getLuck() && rng() % 2)
                    {
                        if (player + c > 21)
                            c = rng() % 7 + 2;
                    }
                    cout << "Drawed Card : " << cardValue(c) << endl;
                    player += c;
                    if (c == 1)
                    {
                        player += 10;
                        aceCountPlayer++;
                    }
                    p++;
                    checkAceSum(aceCountPlayer, player);
                    cout << "Your Cards :";
                    for (int i = 0; i < p; i += 2)
                        cout << cardValue(cards[i]) << " ";
                    cout << endl;
                    if (player > 21)
                        break;
                }
                if (dealer < 17)
                {
                    while (dealer < 17)
                    {
                        int chance = chance_list(rng);
                        int C = cards[p];
                        if (chance > u->getLuck() && rng() % 2)
                        {
                            if (dealer + C > 21)
                                C = rng() % 4 + 2;
                        }
                        dealer += C;
                        if (C == 1)
                        {
                            dealer += 10;
                            aceCountDealer++;
                        }
                        p++;
                        checkAceSum(aceCountDealer, dealer);
                    }
                    cout << "Dealer Hits\n";
                }
                else
                    cout << "Dealer Stand\n";
                cout << "Time to reveal cards\n";
                cout << "Dealer Sum : " << dealer << "\nPlayer Sum : " << player << endl;
                if (player > 21 || (player < dealer && dealer < 21))
                {
                    cout << "Player Lost!\nAmount lost : " << bet << endl;
                    u->updateAmount(-bet);
                    u->updateStreak(0);
                    u->updateLuck(20);
                    u->updatetotalpoints(0);
                    u->logGame("Black Jack", bet, -bet, 0);
                }
                else if (player > dealer || dealer > 21)
                {
                    playJackpot();
                    cout << "Amount Won : " << bet + bet * 0.5 << endl;
                    u->updateAmount(bet * 0.5);
                    u->updateStreak(1);
                    u->updateLuck(-30);
                    u->updatetotalpoints(bet * 2);
                    u->logGame("Black Jack", bet, bet * 0.5, bet * 2);
                }
                else if (player == dealer)
                    cout << "DRAW!!!!\n";
            }
            cout << "Want to play again ?(1 for yes and 0 for no) : ";
            while (!(cin >> F))
            {
                cout << "Invalid input! Try again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (F == 0)
                break;
        }
    }
};
class horseRace : public Game
{
    uniform_real_distribution<float> speed_mod;
    vector<string> horses = {"Alpha", "Bravo", "Charlie", "Delta", "Echo"};

public:
    horseRace(User &U) : speed_mod(0.5, 2.0)
    {
        u = &U;
        gameName = "Thundering Hooves";
    }

    void gameInfo() override
    {
        cout << "\n--- HORSE RACE BETTING ---" << endl;
        cout << "Pick a horse:-\n1. Alpha\n2. Bravo\n3. Charlie\n4. Delta\n5. Echo.\nEach horse has a random performance." << endl;
        cout << "If your horse wins, you get 2.5x your bet!" << endl;
        cout << "High luck increases the chance of your horse getting a speed boost." << endl;
    }

    void playRace()
    {
        while (true)
        {
            gameInfo();
            placeBet();

            int choice;
            while (true)
            {
                cout << "Choose your horse\n1. Alpha\n2. Bravo\n3. Charlie\n4. Delta\n5. Echo\nEnter your choice : ";
                while (!(cin >> choice))
                {
                    cout << "Invalid input! Try again: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                if (choice >= 1 && choice <= 5)
                    break;
                cout << "Invalid stable! Choose 1 to 5.\n";
            }
            playShoot();
            this_thread::sleep_for(chrono::seconds(1));
            cout << "\nTHE RACE IS ON!\n";
            playHorse();
            vector<float> progress(5, 0.0);
            bool raceFinished = false;
            int winner = -1;
            while (!raceFinished)
            {
                for (int i = 0; i < 5; i++)
                {
                    if (raceFinished)
                        break;
                    float boost = speed_mod(rng);
                    if (i == (choice - 1) && (rng() % 100 < u->getLuck()))
                    {
                        boost += 0.5;
                    }
                    progress[i] += boost;
                    cout << horses[i] << " |";
                    int p = (int)progress[i];
                    for (int j = 0; j < 20; j++)
                    {
                        if (j == p)
                            cout << ">";
                        else
                            cout << "-";
                    }
                    cout << "| FINISH" << endl;
                    if (progress[i] >= 20.0 && !raceFinished)
                    {
                        raceFinished = true;
                        winner = i;
                    }
                }
                if (!raceFinished)
                {
                    this_thread::sleep_for(chrono::milliseconds(200));
                    cout << "\033[5A";
                }
            }
            cout << "\nWINNER: " << horses[winner] << "!\n";
            if (winner == (choice - 1))
            {
                float reward = bet * 1.5;
                playJackpot();
                cout << "You won " << (bet + reward) << "!" << endl;
                u->updateAmount(reward);
                u->updateLuck(-40);
                u->updateStreak(1);
                u->updatetotalpoints(bet * 2);
                u->logGame("Horse Game", bet, bet * 1.5, bet * 2);
            }
            else
            {
                cout << "Better luck at the next track. Lost: " << bet << endl;
                u->updateAmount(-bet);
                u->updateLuck(15);
                u->updateStreak(0);
                u->updatetotalpoints(0);
                u->logGame("Horse Game", bet, -bet, 0);
            }
            int F;
            cout << "Wanna visit the stables again? (1 for Yes, 0 for No): ";
            while (!(cin >> F))
            {
                cout << "Invalid input! Try again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (F == 0)
                break;
        }
    }
};

int main()
{
    int choice;
    cout << "==============================\n";
    cout << "   WELCOME TO THE CASINO!!\n";
    cout << "==============================\n";
    cout << "1. New User\n2. Load Existing User\nChoice: ";
    while (!(cin >> choice))
    {
        cout << "Invalid input! Try again: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    User *player = nullptr;

    if (choice == 2)
    {
        int cid;
        cout << "Enter your CID: ";
        cin >> cid;

        User temp("temp", cid, 0);

        if (User::loadFromFile(cid, temp))
        {
            cout << "Welcome back, " << temp.getName() << "!\n";
            player = new User(temp);
        }
        else
        {
            cout << "CID not found! Register? (1 Yes / 0 No): ";
            int reg;
            while (!(cin >> reg))
            {
                cout << "Invalid input! Try again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            if (reg)
            {
                string name;
                float balance;

                cout << "Name: ";
                getline(cin >> ws, name);

                cout << "Balance: ";
                while (!(cin >> balance))
                {
                    cout << "Invalid balance! Try again: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                int newCID = User::generateUniqueCID();
                cout << "Your new CID is: " << newCID << endl;
                player = new User(name, newCID, balance);
                player->saveToFile();
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
        string name;
        float balance;

        cout << "Name: ";
        getline(cin >> ws, name);
        cout << "CID: ";
        int cid = User::generateUniqueCID();
        cout << "Your generated CID is: " << cid << endl;
        cout << "Balance: ";
        while (!(cin >> balance))
        {
            cout << "Invalid balance! Try again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        player = new User(name, cid, balance);
        player->saveToFile();
    }

    int h;
    cout << "\nView your game history? (1 Yes / 0 No): ";
    while (!(cin >> h))
    {
        cout << "Invalid input! Try again: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (h)
        User::showHistory(player->getCID());

    // Game loop
    while (true)
    {
        cout << "\n==============================\n";
        cout << "        GAME MENU\n";
        cout << "==============================\n";
        cout << "Balance : " << player->getAmount() << "\n";
        cout << "Points  : " << player->gettotalpoints() << "\n";
        cout << "------------------------------\n";
        cout << "1. Dice Guess\n";
        cout << "2. Slot Machine\n";
        cout << "3. Russian Roulette\n";
        cout << "4. Black Jack\n";
        cout << "5. Horse Race\n";
        cout << "6. View History\n";
        cout << "0. Exit\n";
        cout << "Choose a game: ";

        int gameChoice;
        while (!(cin >> gameChoice))
        {
            cout << "Invalid choice! Try again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (gameChoice == 0)
            break;

        switch (gameChoice)
        {
        case 1:
        {
            diceGuess dg(*player);
            dg.playDiceGame();
            break;
        }
        case 2:
        {
            slotMachine sm(*player);
            sm.playSlot();
            break;
        }
        case 3:
        {
            russian_roulete rr(*player);
            rr.playRoulette();
            break;
        }
        case 4:
        {
            blackJack bj(*player);
            bj.playBlackJack();
            break;
        }
        case 5:
        {
            horseRace hr(*player);
            hr.playRace();
            break;
        }
        case 6:
        {
            User::showHistory(player->getCID());
            break;
        }
        default:
            cout << "Invalid choice!\n";
        }

        player->saveToFile();

        if (player->getAmount() <= 0)
        {
            cout << "\nYou're out of balance! Game Over.\n";
            break;
        }
    }

    cout << "\n==============================\n";
    cout << "        SESSION SUMMARY\n";
    cout << "==============================\n";
    cout << "Final Balance : " << player->getAmount() << "\n";
    cout << "Total Points  : " << player->gettotalpoints() << "\n";

    player->saveToFile();
    delete player;

    return 0;
}
