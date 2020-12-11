#include <iostream>
#include "move.h"
#include "game_state.h"
#include "move_generator.h"

void print_board_item(BoardItem board_item)
{
    if(board_item.type == BoardItem::empty_square)
    {
        std::cout << ".";
    }

    if(board_item.type == BoardItem::pawn)
    {
        if(board_item.color == Color::white)
            std::cout << "P";
        else
            std::cout << "p";
    }

    if(board_item.type == BoardItem::knight)
    {
        if(board_item.color == Color::white)
            std::cout << "N";
        else
            std::cout << "n";
    }

    if(board_item.type == BoardItem::bishop)
    {
        if(board_item.color == Color::white)
            std::cout << "B";
        else
            std::cout << "b";
    }

    if(board_item.type == BoardItem::rook)
    {
        if(board_item.color == Color::white)
            std::cout << "R";
        else
            std::cout << "r";
    }

    if(board_item.type == BoardItem::queen)
    {
        if(board_item.color == Color::white)
            std::cout << "Q";
        else
            std::cout << "q";
    }

    if(board_item.type == BoardItem::king)
    {
        if(board_item.color == Color::white)
            std::cout << "K";
        else
            std::cout << "k";
    }
}

void print_board(GameState& game_state)
{
    for(int row=8; row>=1; row--)
    {
        std::cout << row << " ";
        for(int col=1; col<=8; col++)
        {
            print_board_item(game_state.board[10*row + 10 + col]);
        }
        std::cout << std::endl;
    }
    std::cout << "  abcdefgh" << std::endl;
    std::cout << std::endl;
}

int main()
{
    std::cout << "Chess" << std::endl;

    GameState game_state;

    print_board(game_state);

    std::cout << std::endl;
    game_state.load_FEN_string("Rr5k/8/8/N5n1/8/8/8/3Kq3 b - - 0 1");

    MoveGenerator generator;
    std::vector<Move>& move_list = generator.get_moves(game_state);

    std::cout << "Size: " << move_list.size() << std::endl;

    for(Move move : move_list)
    {
        game_state.make_move(move);
        print_board(game_state);
        std::cout << move.UCI_format() << std::endl << std::endl;
        game_state.undo_move(move);
        print_board(game_state);
    }
 
    return 0;
}
