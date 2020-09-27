#include "../include/board.h"
#include <string.h>
#include <stdlib.h>

#ifdef DEBUG
#include <iostream>
#define PRINT(x) std::cout << x << std::endl;
#else
#define PRINT(x)
#endif

using Piece = Board::Piece;

static bool isSubstring(const char* total, unsigned totalSize, const char* segement, unsigned segSize) { 
    if(totalSize < segSize) { return false; }
    const void* seg = (void*) segement;
    for(unsigned i{ 0 }; i <= totalSize - segSize; ++i) {
        //if the two things are equal (memcmp = 0 means they're equal)
        if(!memcmp(seg, total + i, segSize)) { 
            return true; //the segement is a subset of the total
        }
    }
    return false;
}

static Piece* copy(const Piece* src, unsigned size) {
    Piece* cpy{ new Piece[size] };
    for(unsigned i{ 0 }; i < size; ++i){
        cpy[i] = src[i];
    }
    return cpy;
}

//define width and height as width and height of connect 4 board
const unsigned short Board::width{ 7 };
const unsigned short Board::height{ 6 };

Board::Board() {
    //make board into 7x6 grid
    board = new Piece*[width];
    for(unsigned i{ 0 }; i < width; ++i) {
        //allocate collumn
        board[i] = new Piece[height];
        //fill collumn with EMPTY
        for(unsigned x{ 0 }; x < height; ++x) {
            board[i][x] = Piece::EMPTY;
        }
    }
}

Board::Board(const Piece** state) {
    for(unsigned i{ 0 }; i < width; ++i) {
        board[i] = copy(state[i], height);
    }
}

int Board::getFallRow(unsigned short collumn) const {
    int row{ -1 };
    if(collumn >= width){
        return -1;
    }
    //if collumn is empty, return last space in collumn
    if(board[collumn][height - 1] == Piece::EMPTY) { return height - 1; }
    for(unsigned i{ 5 }; i > 0; --i) {
        if(board[collumn][i] == Piece::EMPTY) {
            row = i;
            break;
        }
    }
    return row;
}

bool Board::place(unsigned collumn, Piece piece) {
    if(collumn >= width){ return false; }
    int row = this->getFallRow(collumn);
    if(row == -1) {
        return false;
    }

    board[collumn][row] = piece;
    return true;
}

Piece* Board::getRow(unsigned short rowIndex) const {
    if(rowIndex >= height) {
        return nullptr;
    }
    Piece* row{ new Piece[width] };
    for(unsigned i{ 0 }; i < width; ++i) {
        row[i] = board[i][rowIndex];
    }
    return row;
}

std::string Board::getBoard() const {
    //each row is 7 pieces and a \n except for th last one. 
    //six rows, so size is 8*6, -1 for no \n, but +1 for \0
    char* board{ new char[48] };
    char* row;
    for(unsigned i{ 0 }; i < 48; i += 8) {
        row = (char*) getRow(i/8);
        strncpy(board + i, row, width);
        board[i + 7] = '\n'; 
        delete row;
    }
    board[47] = '\0';
    std::string ret = std::string(board);
    delete board;
    return ret;

}

Piece Board::checkWinVert() const {
    Piece winner = Piece::EMPTY;
    for(unsigned char i{ 0 }; i < width; ++i) {
        //exclude terminating character since it's not in the sequence we want to find
        if(isSubstring((char*) board[i], height, "RRRR", 4)) {
            winner = Piece::RED;
            break;
        }
        if(isSubstring((char*) board[i], height, "YYYY", 4)) {
            winner = Piece::YELLOW;
            break;
        }
    }
    return winner;
}

Piece Board::checkWinHoriz() const {
    std::string board = getBoard();
    
    if(board.find(std::string("RRRR")) != std::string::npos) {
        return Piece::RED;
    }
    if(board.find(std::string("YYYY")) != std::string::npos) {
        return Piece::YELLOW;
    } 
    return Piece::EMPTY;
}

Piece Board::checkWin45() const {
    /*concatinate 45 degree-win-elligable lines into
    one big string, then do a find for RRRR and YYYY*/
    char* bigString{ new char[35] };
    unsigned short upTo{ 0 };
    for(unsigned char i{ 0 }; i < 4; ++i) {
        bigString[upTo] = (char) board[i][3 - i];
        upTo++;
    }
    bigString[upTo] = '\n';
    ++upTo;
    
    for(unsigned char i{ 0 }; i < 5; ++i) {
        bigString[upTo] = (char) board[i][4 - i];
        upTo++;
    }
    bigString[upTo] = '\n';
    ++upTo;
    
    for(unsigned char i{ 0 }; i < 6; ++i) {
        bigString[upTo] = (char) board[i][5 - i];
        upTo++;
    }
    bigString[upTo] = '\n';
    ++upTo;
    
    for(unsigned char i{ 0 }; i < 6; ++i) {
        bigString[upTo] = (char) board[i + 1][5 - i];
        upTo++;
    }
    bigString[upTo] = '\n';
    ++upTo;

    for(unsigned char i{ 0 }; i < 5; ++i) {
        bigString[upTo] = (char) board[i + 2][4 - i];
        upTo++;
    }
    bigString[upTo] = '\n';
    ++upTo;

    for(unsigned char i{ 0 }; i < 4; ++i) {
        bigString[upTo] = (char) board[i + 3][3 - i];
        upTo++;
    }
    
    if(isSubstring(bigString, 35, "RRRR", 4)) { return Piece::RED; }
    if(isSubstring(bigString, 35, "YYYY", 4)) { return Piece::YELLOW; }
    return Piece::EMPTY;
}

Piece Board::checkWin135() const {
    //look at checkWin45 for explanation
    char* bigString{ new char[35] };
    unsigned short upTo{ 0 };

    for(unsigned char i{ 2 }; i < height; ++i) {
        bigString[upTo] = (char) board[i - 2][i];
        ++upTo;
    }
    bigString[upTo] = '\n';
    ++upTo;

    for(unsigned char i{ 1 }; i < height; ++i) {
        bigString[upTo] = (char) board[i - 1][i];
        ++upTo;
    }
    bigString[upTo] = '\n';
    ++upTo;

    for(unsigned char i{ 0 }; i < height; ++i) {
        bigString[upTo] = (char) board[i][i];
        ++upTo;
    }
    bigString[upTo] = '\n';
    ++upTo;

    for(unsigned char i{ 0 }; i < height; ++i) {
        bigString[upTo] = (char) board[i + 1][i];
        ++upTo;
    }
    bigString[upTo] = '\n';
    ++upTo;

    for(unsigned char i{ 1 }; i < height; ++i) {
        bigString[upTo] = (char) board[i + 1][i];
        ++upTo;
    }
    bigString[upTo] = '\n';
    ++upTo;

    for(unsigned char i{ 2 }; i < height; ++i) {
        bigString[upTo] = (char) board[i + 1][i];
        ++upTo;
    }
    
    if(isSubstring(bigString, 35, "RRRR", 4)) { return Piece::RED; }
    if(isSubstring(bigString, 35, "YYYY", 4)) { return Piece::YELLOW; }
    return Piece::EMPTY;
}

bool Board::checkFull() const {
        if(getBoard().find('O') == std::string::npos) { return true; }
        return false;
}

Piece Board::checkWinner() const {
    Piece winner = checkWin135();
    PRINT("135: " << (char) winner)
    if(winner != Piece::EMPTY) {return winner; }
    winner = checkWin45();
    PRINT("45: " << (char) winner)
    if(winner != Piece::EMPTY) {return winner; }
    winner = checkWinVert();
    PRINT("Vert: " << (char) winner)
    if(winner != Piece::EMPTY) {return winner; }
    winner = checkWinHoriz();
    PRINT("Horizontal: " << (char) winner)
    if(winner != Piece::EMPTY) {return winner; }

    return Piece::EMPTY;
}

Board::~Board() {
    for(unsigned i{ 0 }; i < width; ++i) {
        delete[] board[i];
    }
    delete[] board;
}

ostream& operator<<(ostream& out, const Board& game) {
    out << game.getBoard();
    return out;
}