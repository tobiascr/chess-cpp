#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include "move.h"

/* The board is represented as an array of 120 elements, enumerated from 0 to 119.
Some of the numbers represent positions outside of the board, as shown in the diagram:

110 111 112 113 114 115 116 117 118 119 
100 101 102 103 104 105 106 107 108 109
    ------------------------------
90 |91  92  93  94  95  96  97  98| 99
80 |81  82  83  84  85  86  87  88| 89
70 |71  72  73  74  75  76  77  78| 79
60 |61  62  63  64  65  66  67  68| 69
50 |51  52  53  54  55  56  57  58| 59
40 |41  42  43  44  45  46  47  48| 49
30 |31  32  33  34  35  36  37  38| 39
20 |21  22  23  24  25  26  27  28| 29
    ------------------------------
10  11  12  13  14  15  16  17  18  19
0   1   2   3   4   5   6   7   8   9

Position 21 is a1, 22 is b1 etc. The positions are called squares, even if they are outside
of the board.
*/

namespace Color
{
    enum ColorType {white = 1, black = -1};

    ColorType opposite_color(ColorType color);
}

class BoardItem
{
public:
    enum BoardItemType {empty_square, pawn, knight, bishop, rook, queen, king, outside_of_board};
    BoardItemType type;
    Color::ColorType color;
};

class GameState
{
public:
    GameState();

    void load_FEN_string(std::string FEN_string);

    void make_move(Move& move);

    void undo_move(Move& move);

    BoardItem board[120];

    Color::ColorType player_in_turn = Color::white;

private:
    BoardItem captured_pieces[32];
    int number_of_captured_pieces;
};

#endif
