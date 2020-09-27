#include <iostream>
#include "../include/board.h"
#include <stdio.h>
#include <stdexcept>

using std::cout;
using std::cin;
using std::endl;

#ifdef DEBUG
#include <iostream>
#define PRINT(x) std::cout << x << std::endl;
#else
#define PRINT(x)
#endif

Board game{};

static void placePiece(Board::Piece player) {
    unsigned collumn{ 0 };
    char buf[100]; 
    unsigned count{ 0 };
    loop:
        
        cout << (char) player << "> Choose Your Collumn (1-7): ";
        scanf("%i", &collumn);
        
    if(collumn == 0) { 
        scanf("%100s", buf); 
        goto statement; 
    }
    else if(!game.place(collumn - 1, player)){
        statement:
        cout << "Please Choose a Valid Collumn" << endl;
        count++;
        #ifdef DEBUG
        if(count > 5) { throw "Input Error Detected"; }
        #endif
        goto loop;
    }
}

int main() {
    Board::Piece winner;
    try {
    do {
        cout << game << endl;
        placePiece(Board::Piece::RED);
        winner = game.checkWinner();
        if(winner != Board::Piece::EMPTY){ break; }
        
        //Now do the same thing but for yellow
        cout << game << endl;
        

        placePiece(Board::Piece::YELLOW);
        winner = game.checkWinner();
        if(winner != Board::Piece::EMPTY){ break; }
        if(game.checkFull()){ break; }
        
    } while(true);
    } catch(char const* e) {
        std::cerr << e << endl;
        return 0;
    }

    cout << game << endl;
    switch(winner){
        case Board::Piece::RED:
        cout << "Red Wins!" << endl;
        break;
        case Board::Piece::YELLOW:
        cout << "YELLOW Wins!" << endl;
        break;
        case Board::Piece::EMPTY:
        cout << "It's a Tie." << endl;
        break;
    }

    return 0;
}