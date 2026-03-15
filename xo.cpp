// ──────────────────────────────────────────────────────────────────────────── libraries

#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

// ──────────────────────────────────────────────────────────────────────────── macros

#define wait(ms) cout << flush, this_thread::sleep_for(chrono::milliseconds(ms))
#define cls cout << "\e[2J\e[H"
#define hidecur cout << "\e[?25l"
#define showcur cout << "\e[?25h"
#define red "\e[91m"
#define blue "\e[96m"
#define purple "\e[95m"
#define yellow "\e[93m"
#define reset "\e[m"

// ──────────────────────────────────────────────────────────────────────────── globals

char player, character1, character2, grid[9], winner;
int mode, scorex, scoreo, draws, wincells[3] = {-1, -1, -1};

// ──────────────────────────────────────────────────────────────────────────── functions

void startup()
{
    srand(time(0));
    cls;
    hidecur;
    
    cout << "Loading";
    for (int i = 0; i < 3; i++, wait(500)) cout << '.';
    cout << "\nWelcome to terminal XO.";
    wait(1000);
    cls;
}

void setup()
{
    string input;
    showcur;

    if (mode)
    {
        cout << "Change settings? [y/N] ";
        getline(cin, input);
        cls;
        hidecur;
        if (input != "y" && input != "Y") return;
    }

    cout << "Choose your character. [" << red << 'X' << reset << '/' << blue << 'O' << reset << "] ";
    getline(cin, input);
    if (input == "x" || input == "X") player = 'X';
    else if (input == "o" || input == "O") player = 'O';
    else
    {
        cout << yellow << "Invalid input, try again." << reset;
        wait(1000);
        mode = 0;
        cls;
        setup();
        return;
    }
    
    cout << "Start first? [Y/n] ";
    getline(cin, input);
    character1 = ((input == "n" || input == "N") ? (player == 'X' ? 'O' : 'X') : player);
    character2 = (character1 == 'X' ? 'O' : 'X');
    
    cout << "Play against AI? [Y/n] ";
    getline(cin, input);
    mode = ((input == "n" || input == "N") ? 2 : 1);

    cls;
    hidecur;
}

bool checkstate()
{
    for (int i = 0; i < 7; i += 3)
    {
        if (grid[i] && grid[i] == grid[i + 1] && grid[i] == grid[i + 2])
        {
            winner = grid[i];
            wincells[0] = i;
            wincells[1] = i + 1;
            wincells[2] = i + 2;
            return true;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        if (grid[i] && grid[i] == grid[i + 3] && grid[i] == grid[i + 6])
        {
            winner = grid[i];
            wincells[0] = i;
            wincells[1] = i + 3;
            wincells[2] = i + 6;
            return true;
        }
    }

    if (grid[4])
    {
        if (grid[4] == grid[0] && grid[4] == grid[8])
        {
            winner = grid[4];
            wincells[0] = 0;
            wincells[1] = 4;
            wincells[2] = 8;
            return true;
        }
        if (grid[4] == grid[2] && grid[4] == grid[6])
        {
            winner = grid[4];
            wincells[0] = 2;
            wincells[1] = 4;
            wincells[2] = 6;
            return true;
        }
    }

    winner = 0;
    for (int &wincell : wincells) wincell = -1;
    for (char cell : grid) if (!cell) return false;
    return true;
}

void printstate()
{
    cout << "\n ┌───┬───┬───┐\n";
    for (int i = 0; i < 9; i++)
    {
        bool iswincell = (i == wincells[0] || i == wincells[1] || i == wincells[2]);
        cout << " │ ";
        grid[i] ? (cout << (iswincell ? yellow : (grid[i] == 'X' ? red : blue)) << grid[i] << reset) : (cout << i + 1);
        if (i % 3 == 2) cout << " │\n " << (i != 8 ? "├───┼───┼───┤\n" : "└───┴───┴───┘\n\n ");
    }

    cout << red << "X: " << scorex << reset << "  |  " << purple << "Draws: " << draws << reset << "  |  " << blue << "O: " << scoreo << reset << "\n\n";
}

void humanmove(char character)
{
    cout << (character == 'X' ? red : blue) << character << reset << "'s turn. [1-9] ";

    int input;
    showcur;
    bool valid = (cin >> input && input >= 1 && input <= 9 && !grid[input - 1]);
    hidecur;
    cin.clear();
    cin.ignore(9e9, '\n');
    
    if (!valid)
    {
        cout << yellow << "Invalid input, try again." << reset;
        wait(1000);
        cls;
        printstate();
        humanmove(character);
        return;
    }

    grid[input - 1] = character;
}

void aimove(char character)
{
    cout << (character == 'X' ? red : blue) << character << reset << "'s turn. [AI] ";
    for (int i = 0; i < 3; i++, wait(500)) cout << '.';

    for (int i = 0; i < 9; i++)
    {
        if (!grid[i])
        {
            grid[i] = character;
            if (checkstate() && winner) return;
            grid[i] = 0;
        }
    }

    for (int i = 0; i < 9; i++)
    {
        if (!grid[i])
        {
            grid[i] = player;
            if (checkstate() && winner)
            {
                grid[i] = character;
                return;
            }
            grid[i] = 0;
        }
    }

    int randcell;
    do randcell = rand() % 9;
    while (grid[randcell]);
    grid[randcell] = character;
}

bool gameover()
{
    winner == 'X' ? scorex++ : (winner == 'O' ? scoreo++ : draws++);
    
    printstate();
    winner ? (cout << (winner == 'X' ? red : blue) << winner << reset << " won the game.") : (cout << "It's a " << purple << "draw" << reset << '.');
    wait(1000);
    
    cout << " Play again? [Y/n] ";
    string input;
    showcur;
    getline(cin, input);
    hidecur;

    cls;
    for (char &cell : grid) cell = 0;
    return (input == "n" || input == "N");
}

void gameloop()
{
    do
    {
        setup();

        for (bool turn = true; !checkstate(); turn = !turn)
        {
            printstate();
            char character = (turn ? character1 : character2);
            (mode == 1 && character != player) ? aimove(character) : humanmove(character);
            cls;
        }
    }
    while (!gameover());
}

void shutdown()
{
    cout << "Goodbye.";
    wait(1000);
    cls;
    showcur;
}

// ──────────────────────────────────────────────────────────────────────────── main

int main()
{
    startup();
    gameloop();
    shutdown();
}
