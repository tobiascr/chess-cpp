#include <iostream>
#include <stdlib.h>
#include "game_state.h"

GameState::GameState()
{
    reset();
}

void GameState::reset()
{
    for(int i=0; i<120; i++)
    {
        if(board[i].type != BoardItemType::outside_of_board)
        {
            board[i].type = BoardItemType::empty_square;
        }
    }
}

void GameState::print_board()
{
    std::cout << "board" << std::endl;
}

void GameState::make_move(Move& move)
{

}

void GameState::undo_move(Move& move)
{

}
