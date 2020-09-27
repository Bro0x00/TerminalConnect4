#include <string>
#include <ostream>
#pragma once

using std::ostream;

class Board {
    public:
    //ascii for O, R, and Y respectively. convert to char to get ascii
    enum class Piece : char{ EMPTY = 79, RED = 82, YELLOW = 89};
    friend ostream& operator<<(ostream& out, const Board& game);
    private:
    Piece** board; //points to collumns, which are arrays of pieces
    //get the row a piece will fall into in a given collumn. returns -1 if full
    int getFallRow(unsigned short) const; //DONE
    
    /*the following functions are used by the check winner function
    to see if various win conditions have been met*/

    //check for four in a row on horizontal
    Piece checkWinHoriz() const; //DONE
    //check for four in a row on vertical
    Piece checkWinVert() const; //DONE
    //check for four in a row diagonally at 45 degree angle
    Piece checkWin45() const; //DONE
    //check for four in a row diagonally at 135 degree angle
    Piece checkWin135() const; //DONE
    //get row, top is 0. returns nullptr if index out of range
    Piece* getRow(unsigned short rowIndex) const; //DONE

    public:
    //height and width of board
    static const unsigned short width;
    static const unsigned short height;

    Board(); //DONE
    Board(const Piece** state); //DONE
    //place piece into specified collumn. returns false if cannot place
    bool place(unsigned collumn, Piece piece); //DONE
    //return string representation of the board
    std::string getBoard() const; //DONE
    //returns winner: RED if red, YELLOW if yellow, and EMPTY if none
    Piece checkWinner() const; //DONE
    //check if the board is full. false if not full
    bool checkFull() const; //DONE
    ~Board(); //DONE

};