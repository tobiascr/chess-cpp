#include "move_generator.h"

std::vector<Move>& MoveGenerator::
                   get_moves_no_castlings_only_queen_promotions(GameState& game_state)
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
                    switch(game_state.board[square].type)
                    {
                        case BoardItem::king:
                            generate_king_moves(game_state, square);
                            break;
                        case BoardItem::knight:
                            generate_knight_moves(game_state, square);
                            break;
                        case BoardItem::bishop:
                            generate_bishop_moves(game_state, square);
                            break;
                        case BoardItem::rook:
                            generate_rook_moves(game_state, square);
                            break;
                        case BoardItem::queen:
                            generate_queen_moves(game_state, square);
                            break;
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

void MoveGenerator::generate_pawn_moves(GameState& game_state, int from_square)
{
    // Ordinary non capture

    // Capture

    // Double pawn push

    // En passant

    // Promotion

    // Capture promotion
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

void MoveGenerator::generate_bishop_moves(GameState& game_state, int from_square)
{
    Move move;
    const int changes[4] = {9, 11, -9, -11};

    for(int change : changes)
    {
        int to_square = from_square + change;
        while(true)
        {
            if(game_state.board[to_square].type == BoardItem::empty_square)
            {
                move.type = MoveType::non_capture;
                move.from_square = from_square;
                move.to_square = to_square;
                move_list.push_back(move);
                to_square += change;
                continue;
            }
            if(game_state.board[to_square].type == BoardItem::outside_of_board)
            {
                break;
            }
            if(game_state.board[to_square].color != game_state.player_in_turn)
            {
                move.type = MoveType::capture;
                move.from_square = from_square;
                move.to_square = to_square;
                move_list.push_back(move); 
            }
            break;
        }
    }
}

void MoveGenerator::generate_rook_moves(GameState& game_state, int from_square)
{
    Move move;
    const int changes[4] = {10, -10, 1, -1};

    for(int change : changes)
    {
        int to_square = from_square + change;
        while(true)
        {
            if(game_state.board[to_square].type == BoardItem::empty_square)
            {
                move.type = MoveType::non_capture;
                move.from_square = from_square;
                move.to_square = to_square;
                move_list.push_back(move);
                to_square += change;
                continue;
            }
            if(game_state.board[to_square].type == BoardItem::outside_of_board)
            {
                break;
            }
            if(game_state.board[to_square].color != game_state.player_in_turn)
            {
                move.type = MoveType::capture;
                move.from_square = from_square;
                move.to_square = to_square;
                move_list.push_back(move); 
            }
            break;
        }
    }
}

void MoveGenerator::generate_queen_moves(GameState& game_state, int from_square)
{
    generate_bishop_moves(game_state, from_square);
    generate_rook_moves(game_state, from_square);
}
