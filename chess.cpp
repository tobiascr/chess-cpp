#include <iostream>
#include <vector>
#include "move.h"
#include "game_state.h"

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

    std::cout << -game_state.player_in_turn << std::endl;

    game_state.make_move(move);
    game_state.undo_move(move);

    game_state.print_board();

    BoardItem piece;
    piece.type = BoardItemType::rook;
    piece.color = Color::black;
    game_state.board[25] = piece;

    std::cout << "type: " << game_state.board[25].type << std::endl;
    std::cout << "color: " << game_state.board[25].color << std::endl;

    return 0;
}
