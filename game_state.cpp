#include <map>
#include "game_state.h"

namespace Color
{
    ColorType opposite_color(ColorType color)
    {
        return static_cast<ColorType>(-color);
    }
}

GameState::GameState()
{
    for(int i=0; i<120; i++)
    {
        board[i].type = BoardItem::outside_of_board;
    }
    load_FEN_string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    number_of_captured_pieces = 0;
}

void GameState::load_FEN_string(std::string FEN_string)
{
    // Reset board
    for(int r=20; r<=90; r+=10)
    {
        for(int c=1; c<=8; c++)
        {
            board[r+c].type = BoardItem::empty_square;
        }
    }

    int i = 0; // Index in string
    int square = 91;

    // Load position
    while(FEN_string[i] != ' ')
    {
        char c = FEN_string[i];
        if(c == 'P')
        {
            board[square].type = BoardItem::pawn;
            board[square].color = Color::white;
        }
        if(c == 'p')
        {
            board[square].type = BoardItem::pawn;
            board[square].color = Color::black;
        }
        if(c == 'N')
        {
            board[square].type = BoardItem::knight;
            board[square].color = Color::white;
        }
        if(c == 'n')
        {
            board[square].type = BoardItem::knight;
            board[square].color = Color::black;
        }
        if(c == 'B')
        {
            board[square].type = BoardItem::bishop;
            board[square].color = Color::white;
        }
        if(c == 'b')
        {
            board[square].type = BoardItem::bishop;
            board[square].color = Color::black;
        }
        if(c == 'R')
        {
            board[square].type = BoardItem::rook;
            board[square].color = Color::white;
        }
        if(c == 'r')
        {
            board[square].type = BoardItem::rook;
            board[square].color = Color::black;
        }
        if(c == 'Q')
        {
            board[square].type = BoardItem::queen;
            board[square].color = Color::white;
        }
        if(c == 'q')
        {
            board[square].type = BoardItem::queen;
            board[square].color = Color::black;
        }
        if(c == 'K')
        {
            board[square].type = BoardItem::king;
            board[square].color = Color::white;
        }
        if(c == 'k')
        {
            board[square].type = BoardItem::king;
            board[square].color = Color::black;
        }
        if(c == '/')
        {
            square -= 19;
        }
        if(c == '2')
        {
            square++;
        }
        if(c == '3')
        {
            square += 2;
        }
        if(c == '4')
        {
            square += 3;
        }
        if(c == '5')
        {
            square += 4;
        }
        if(c == '6')
        {
            square += 5;
        }
        if(c == '7')
        {
            square += 6;
        }
        if(c == '8')
        {
            square += 7;
        }
        square++;
        i++;
    }

    // Load active color
    i++;
    if(FEN_string[i] == 'w')
    {
        player_in_turn = Color::white;
    }
    else
    {
        player_in_turn = Color::black;
    }

    // Load castling rights
    i += 2;
    white_kingside_castling = false;
    white_queenside_castling = false;
    black_kingside_castling = false;
    black_queenside_castling = false;
    if(FEN_string[i] != '-')
    {
        while(FEN_string[i] != ' ')
        {
            switch(FEN_string[i])
            {
                case 'K':
                    white_kingside_castling = true;
                    break;
                case 'Q':
                    white_queenside_castling = true;
                    break;
                case 'k':
                    black_kingside_castling = true;
                    break;
                case 'q':
                    black_queenside_castling = true;
                    break;
            }
            i++;
        }
    }
    else
    {
        i++;
    }

    // Load en passant target square
    i++;
    if(FEN_string[i] == '-')
    {
        en_passant_target_square = 0;
    }
    else
    {
        char col = FEN_string[i];
        char row = FEN_string[i + 1];

        std::string square = FEN_string.substr(i, 2);

        std::map<std::string, int> internal_format =
        {{"a1", 21}, {"b1", 22}, {"c1", 23}, {"d1", 24}, {"e1", 25}, {"f1", 26}, {"g1", 27}, {"h1", 28},
         {"a2", 31}, {"b2", 32}, {"c2", 33}, {"d2", 34}, {"e2", 35}, {"f2", 36}, {"g2", 37}, {"h2", 38},
         {"a3", 41}, {"b3", 42}, {"c3", 43}, {"d3", 44}, {"e3", 45}, {"f3", 46}, {"g3", 47}, {"h3", 48},
         {"a4", 51}, {"b4", 52}, {"c4", 53}, {"d4", 54}, {"e4", 55}, {"f4", 56}, {"g4", 57}, {"h4", 58},
         {"a5", 61}, {"b5", 62}, {"c5", 63}, {"d5", 64}, {"e5", 65}, {"f5", 66}, {"g5", 67}, {"h5", 68},
         {"a6", 71}, {"b6", 72}, {"c6", 73}, {"d6", 74}, {"e6", 75}, {"f6", 76}, {"g6", 77}, {"h6", 78},
         {"a7", 81}, {"b7", 82}, {"c7", 83}, {"d7", 84}, {"e7", 85}, {"f7", 86}, {"g7", 87}, {"h7", 88},
         {"a8", 91}, {"b8", 92}, {"c8", 93}, {"d8", 94}, {"e8", 95}, {"f8", 96}, {"g8", 97}, {"h8", 98}};

//        switch(col)
//        {
//            case 'a':
//                break;
//            case 'b':
//                break;
//            case 'c':
//                break;
//            case 'd':
//                break;
//            case 'e':
//                break;
//            case 'f':
//                break;
//            case 'g':
//                break;
//            case 'h':
//                break;
//        }
        en_passant_target_square = internal_format[square];
    }


}

void GameState::make_move(Move& move)
{
    player_in_turn = Color::opposite_color(player_in_turn);
    switch(move.type)
    {
        case MoveType::non_capture:
            board[move.to_square] = board[move.from_square];
            board[move.from_square].type = BoardItem::empty_square;
            break;
        case MoveType::capture:
            captured_pieces[number_of_captured_pieces] = board[move.to_square];
            number_of_captured_pieces++;
            board[move.to_square] = board[move.from_square];
            board[move.from_square].type = BoardItem::empty_square;
            break;
        case MoveType::white_kingside_castling:
            board[27] = board[25];
            board[26] = board[28];
            board[25].type = BoardItem::empty_square;
            board[28].type = BoardItem::empty_square;
            break;
        case MoveType::white_queenside_castling:
            board[23] = board[25];
            board[24] = board[21];
            board[21].type = BoardItem::empty_square;
            board[25].type = BoardItem::empty_square;
            break;
        case MoveType::black_kingside_castling:
            board[97] = board[95];
            board[96] = board[98];
            board[95].type = BoardItem::empty_square;
            board[98].type = BoardItem::empty_square;
            break;
        case MoveType::black_queenside_castling:
            board[93] = board[95];
            board[94] = board[91];
            board[91].type = BoardItem::empty_square;
            board[95].type = BoardItem::empty_square;
            break;
    }
}

void GameState::undo_move(Move& move)
{
    player_in_turn = Color::opposite_color(player_in_turn);
    switch(move.type)
    {
        case MoveType::non_capture:
            board[move.from_square] = board[move.to_square];
            board[move.to_square].type = BoardItem::empty_square;
            break;
        case MoveType::capture:
            number_of_captured_pieces--;
            board[move.from_square] = board[move.to_square];
            board[move.to_square] = captured_pieces[number_of_captured_pieces];
            break;
        case MoveType::white_kingside_castling:
            board[25] = board[27];
            board[28] = board[26];
            board[26].type = BoardItem::empty_square;
            board[27].type = BoardItem::empty_square;
            break;
        case MoveType::white_queenside_castling:
            board[25] = board[23];
            board[21] = board[24];
            board[23].type = BoardItem::empty_square;
            board[24].type = BoardItem::empty_square;
            break;
        case MoveType::black_kingside_castling:
            board[95] = board[97];
            board[98] = board[96];
            board[96].type = BoardItem::empty_square;
            board[97].type = BoardItem::empty_square;
            break;
        case MoveType::black_queenside_castling:
            board[95] = board[93];
            board[91] = board[94];
            board[93].type = BoardItem::empty_square;
            board[94].type = BoardItem::empty_square;
            break;
    }
}
