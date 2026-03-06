// ### XO game written in C++ ###

#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

#define wait(ms) this_thread::sleep_for(chrono::milliseconds(ms))
#define dots for (int i = 0; i < 3; i++, wait(500)) cout << '.'
#define cls cout << "\e[2J\e[H"
#define red "\e[31m"
#define blue "\e[34m"
#define purple "\e[35m"
#define yellow "\e[33m"
#define reset "\e[m"

char grid[9], winner;
int winLine[3] = {-1, -1, -1};

// plays the intro animation.
void intro() {
    cls;
    cout << "starting";
    dots;
    cout << "\nwelcome to terminal XO.";
    wait(1000);
    cls;
}

// prints the board. empty cells show their number [1-9], taken cells show X or O.
void printGrid() {
    cout << "\n " << string(13, '-') << '\n';
    for (int i = 0; i < 9; i++) {
        bool isWinCell = (i == winLine[0] || i == winLine[1] || i == winLine[2]);
        grid[i] ? cout << " | " << (isWinCell ? yellow : (grid[i] == 'X' ? red : blue)) << grid[i] << reset : cout << " | " << (i + 1);
        if (i % 3 == 2) cout << " |\n " << string(13, '-') << '\n';
    }
    cout << '\n';
}

// checks for a win or draw. sets winner and returns true if the game is over.
bool checkGameOver() {
    winner = 0;
    winLine[0] = winLine[1] = winLine[2] = -1;
    
    // check rows
    for (int i = 0; i < 7; i += 3) {
        if (grid[i] && grid[i] == grid[i + 1] && grid[i] == grid[i + 2]) {
            winner = grid[i];
            winLine[0] = i; winLine[1] = i + 1; winLine[2] = i + 2;
            return true;
        }
    }
    
    // check columns
    for (int i = 0; i < 3; i++) {
        if (grid[i] && grid[i] == grid[i + 3] && grid[i] == grid[i + 6]) {
            winner = grid[i];
            winLine[0] = i; winLine[1] = i + 3; winLine[2] = i + 6;
            return true;
        }
    }
    
    // check diagonals
    if (grid[4]) {
        if (grid[4] == grid[0] && grid[4] == grid[8]) {
            winner = grid[4];
            winLine[0] = 0; winLine[1] = 4; winLine[2] = 8;
            return true;
        }
        if (grid[4] == grid[2] && grid[4] == grid[6]) {
            winner = grid[4];
            winLine[0] = 2; winLine[1] = 4; winLine[2] = 6;
            return true;
        }
    }
    
    // draw: board is full and no winner
    for (int i = 0; i < 9; i++) if (!grid[i]) return false;
    return true;
}

// AI tries to win, blocks the human, or plays a random empty cell.
void play(char ai) {
    char human = (ai == 'O' ? 'X' : 'O');
    
    // 1. try winning move
    for (int i = 0; i < 9; i++) {
        if (!grid[i]) {
            grid[i] = ai;
            if (checkGameOver() && winner) {
                dots;
                cout << ' ' << i + 1;
                wait(500);
                return;
            }
            grid[i] = 0;
        }
    }
    
    // 2. block human
    for (int i = 0; i < 9; i++) {
        if (!grid[i]) {
            grid[i] = human;
            if (checkGameOver() && winner) {
                grid[i] = ai;
                dots;
                cout << ' ' << i + 1;
                wait(500);
                return;
            }
            grid[i] = 0;
        }
    }
    
    // 3. otherwise random
    int num;
    do num = rand() % 9; while (grid[num]);
    grid[num] = ai;
    dots;
    cout << ' ' << num + 1;
    wait(500);
}

int main() {
    srand(chrono::steady_clock::now().time_since_epoch().count()); // uses current time as a seed
    cout << unitbuf; // flushes output automatically
    intro();
    string input;
    
    while (true) {
        bool solo = true, startFirst = true;
        cout << "play solo? [Y/n] ";
        getline(cin, input);
        if (input == "n" || input == "N") solo = false;
        else {
            cout << "start first? [Y/n] ";
            getline(cin, input);
            if (input == "n" || input == "N") startFirst = false;
        }
        
        // 'i' tracks turns: even = X, odd = O
        for (int i = 0; !checkGameOver(); i++) {
            cls;
            printGrid();
            char player = (i % 2 == 0 ? 'X' : 'O');
            
            if (solo && player == (startFirst ? 'O' : 'X')) {
                cout << (player == 'X' ? red : blue) << player << reset "'s turn. [AI] ";
                play(player);
            } else {
                cout << (player == 'X' ? red : blue) << player << reset "'s turn. [1-9] ";
                int position;
                if (!(cin >> position) || position > 9 || position < 1 || grid[position - 1]) {
                    cout << yellow << "invalid input, try again." << reset;
                    cin.clear();
                    i--;
                    wait(1000);
                } else grid[position - 1] = player;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        
        cls;
        printGrid();
        winner ? cout << (winner == 'X' ? red : blue) << winner << reset " won the game." : cout << "it's a " purple "draw" reset ".";
        wait(1000);
        cout << " play again? [Y/n] ";
        getline(cin, input);
        if (input == "n" || input == "N") break;
        
        // reset for next game
        for (char &c : grid) c = 0;
        cls;
    }
    
    cls;
    cout << "goodbye.";
    wait(1000);
    cls;
}
