#include "move_generator.h"

std::vector<Move>& MoveGenerator::get_moves(GameState& game_state)
{
    for(int r=20; r<=90; r+=10)
    {
        for(int c=1; c<=8; c++)
        {
            const int square = r + c;
            if(game_state.board[square].type != BoardItem::empty_square)
            {
                if(game_state.board[square].color == game_state.player_in_turn)
                {
                    if(game_state.board[square].type == BoardItem::king)
                    {
                        generate_king_moves(game_state, square);
                        continue;
                    }
                    if(game_state.board[square].type == BoardItem::knight)
                    {
                        generate_knight_moves(game_state, square);
                        continue;
                    }
                }
            }
        }
    }

    return move_list;
}

void MoveGenerator::generate_king_moves(GameState& game_state, int from_square)
{
    Move move;
    const int changes[8] = {9, 10, 11, 1, - 9, - 10, -11, -1};

    for(int change : changes)
    {
        int to_square = from_square + change;
        if(game_state.board[to_square].type == BoardItem::outside_of_board)
        {
            continue;
        }
        if(game_state.board[to_square].type == BoardItem::empty_square)
        {
            move.type = MoveType::non_capture;
            move.from_square = from_square;
            move.to_square = to_square;
            move_list.push_back(move);
            continue;
        }
        if(game_state.board[to_square].color != game_state.player_in_turn)
        {
            move.type = MoveType::capture;
            move.from_square = from_square;
            move.to_square = to_square;
            move_list.push_back(move);
        }
    }
}

void MoveGenerator::generate_knight_moves(GameState& game_state, int from_square)
{
    Move move;
    const int changes[8] = {8, 12, 19, 21, -8, -12, -19, -21};

    for(int change : changes)
    {
        int to_square = from_square + change;
        if(game_state.board[to_square].type == BoardItem::outside_of_board)
        {
            continue;
        }
        if(game_state.board[to_square].type == BoardItem::empty_square)
        {
            move.type = MoveType::non_capture;
            move.from_square = from_square;
            move.to_square = to_square;
            move_list.push_back(move);
            continue;
        }
        if(game_state.board[to_square].color != game_state.player_in_turn)
        {
            move.type = MoveType::capture;
            move.from_square = from_square;
            move.to_square = to_square;
            move_list.push_back(move);
        }
    }
}