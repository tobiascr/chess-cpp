#include <iostream>
#include <vector>
#include "move.h"
#include "game_state.h"

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

    Move move;

    move.type = MoveType::queenside_castling;
    move.from_square = 1;
    move.to_square = 1;

    std::cout << "type: " << move.type << std::endl;
    std::cout << "from: "<< move.from_square << std::endl;
    std::cout << "to: "<< move.to_square << std::endl;

    std::vector<Move> move_list;

    move_list.push_back(move);

    GameState game_state;

    print_board(game_state);

    move.type = MoveType::non_capture;
    move.from_square = 35;
    move.to_square = 55;

    game_state.make_move(move);
    print_board(game_state);
    game_state.undo_move(move);
    print_board(game_state);

    game_state.make_move(move);
    game_state.undo_move(move);

    std::cout << "type: " << game_state.board[25].type << std::endl;
    std::cout << "color: " << game_state.board[25].color << std::endl;

    std::cout << "type: " << game_state.board[25].type << std::endl;
    std::cout << "color: " << game_state.board[25].color << std::endl;

    std::cout << std::endl;

    game_state.load_FEN_string("brBKpppp/qqqqQQQQ/PBrrrrKq/p1P2q2/8/8/k7/1P6 ");
    print_board(game_state);
    game_state.load_FEN_string("r1bqk2r/pppp1p1p/2n2np1/1Bb1p3/4P3/2N2N2/PPPP1PPP/R1BQ1RK1 w Qkq - 0");
    print_board(game_state);

    auto c = Color::white;
    c = Color::opposite_color(c);
    std::cout << -c << std::endl;
    return 0;
}
