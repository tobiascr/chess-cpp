#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <map>
#include <stack>
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

    void make_move(std::string uci_format_move);
    /* A way of making a move intended for setting up the board before a search.*/

    void undo_move(Move& move);

    BoardItem board[120];

    Color::ColorType player_in_turn;

    int en_passant_target_square;
    /* The square that the capturing pawn moves to. It's does not guarantee that an en
    passant is possible. If a double pawn push has not been done the move before it
    should be set to zero.*/

    // Castling rights are set to true iff the king and corresponding rook has never been
    // moved.
    bool white_kingside_castling;
    bool white_queenside_castling;
    bool black_kingside_castling;
    bool black_queenside_castling;

private:
    BoardItem captured_pieces[32];
    int number_of_captured_pieces;
    std::stack<int> en_passant_target_square_history;

    std::map<std::string, int> internal_format =
        {{"a1", 21}, {"b1", 22}, {"c1", 23}, {"d1", 24}, {"e1", 25}, {"f1", 26}, {"g1", 27}, {"h1", 28},
         {"a2", 31}, {"b2", 32}, {"c2", 33}, {"d2", 34}, {"e2", 35}, {"f2", 36}, {"g2", 37}, {"h2", 38},
         {"a3", 41}, {"b3", 42}, {"c3", 43}, {"d3", 44}, {"e3", 45}, {"f3", 46}, {"g3", 47}, {"h3", 48},
         {"a4", 51}, {"b4", 52}, {"c4", 53}, {"d4", 54}, {"e4", 55}, {"f4", 56}, {"g4", 57}, {"h4", 58},
         {"a5", 61}, {"b5", 62}, {"c5", 63}, {"d5", 64}, {"e5", 65}, {"f5", 66}, {"g5", 67}, {"h5", 68},
         {"a6", 71}, {"b6", 72}, {"c6", 73}, {"d6", 74}, {"e6", 75}, {"f6", 76}, {"g6", 77}, {"h6", 78},
         {"a7", 81}, {"b7", 82}, {"c7", 83}, {"d7", 84}, {"e7", 85}, {"f7", 86}, {"g7", 87}, {"h7", 88},
         {"a8", 91}, {"b8", 92}, {"c8", 93}, {"d8", 94}, {"e8", 95}, {"f8", 96}, {"g8", 97}, {"h8", 98}};
};

#endif
