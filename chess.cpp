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
    GameState game_state;
    game_state.load_FEN_string("5k2/1r1n4/8/1Qb5/1nK5/8/8/8 w - - 0 1");

    MoveGenerator generator;
    std::vector<Move>& move_list =
               generator.get_moves_no_castlings_only_queen_promotions(game_state);

    std::cout << "Number of moves: " << move_list.size() << std::endl;

    for(Move move : move_list)
    {
        game_state.make_move(move);
        print_board(game_state);
        std::cout << move.UCI_format() << std::endl << std::endl;
        game_state.undo_move(move);
        print_board(game_state);
    }

    game_state.load_FEN_string("r3k2r/8/8/8/8/8/8/R3K2R w Kq a1 0 1");
    std::cout << game_state.white_kingside_castling << std::endl;
    std::cout << game_state.white_queenside_castling << std::endl;
    std::cout << game_state.black_kingside_castling << std::endl;
    std::cout << game_state.black_queenside_castling << std::endl;
    std::cout << game_state.en_passant_target_square << std::endl;
    return 0;
}
