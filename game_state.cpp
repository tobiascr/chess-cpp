#include <stdlib.h>
#include <iostream>
#include "game_state.h"

GameState::GameState()
{
    for(int i=0; i<120; i++)
    {
        board[i].type = BoardItemType::outside_of_board;
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
            board[r+c].type = BoardItemType::empty_square;
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
            board[square].type = BoardItemType::pawn;
            board[square].color = Color::white;
        }
        if(c == 'p')
        {
            board[square].type = BoardItemType::pawn;
            board[square].color = Color::black;
        }
        if(c == 'N')
        {
            board[square].type = BoardItemType::knight;
            board[square].color = Color::white;
        }
        if(c == 'n')
        {
            board[square].type = BoardItemType::knight;
            board[square].color = Color::black;
        }
        if(c == 'B')
        {
            board[square].type = BoardItemType::bishop;
            board[square].color = Color::white;
        }
        if(c == 'b')
        {
            board[square].type = BoardItemType::bishop;
            board[square].color = Color::black;
        }
        if(c == 'R')
        {
            board[square].type = BoardItemType::rook;
            board[square].color = Color::white;
        }
        if(c == 'r')
        {
            board[square].type = BoardItemType::rook;
            board[square].color = Color::black;
        }
        if(c == 'Q')
        {
            board[square].type = BoardItemType::queen;
            board[square].color = Color::white;
        }
        if(c == 'q')
        {
            board[square].type = BoardItemType::queen;
            board[square].color = Color::black;
        }
        if(c == 'K')
        {
            board[square].type = BoardItemType::king;
            board[square].color = Color::white;
        }
        if(c == 'k')
        {
            board[square].type = BoardItemType::king;
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
}

void GameState::make_move(Move& move)
{

}

void GameState::undo_move(Move& move)
{

}
