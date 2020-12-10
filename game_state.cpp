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
    }
}
