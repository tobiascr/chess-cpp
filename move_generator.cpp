#include "move_generator.h"

std::vector<Move>& MoveGenerator::
                   get_moves_no_castlings_only_queen_promotions(GameState& game_state)
{
    move_list.reserve(100);

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
                        case BoardItem::queen:
                            generate_queen_moves(game_state, square);
                            break;
                        case BoardItem::rook:
                            generate_rook_moves(game_state, square);
                            break;
                        case BoardItem::bishop:
                            generate_bishop_moves(game_state, square);
                            break;
                        case BoardItem::knight:
                            generate_knight_moves(game_state, square);
                            break;
                        case BoardItem::pawn:
                            generate_pawn_moves(game_state, square, true);
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
            move_list.emplace_back(MoveType::non_capture, from_square, to_square);
            continue;
        }
        if(game_state.board[to_square].color != game_state.player_in_turn)
        {
            move_list.emplace_back(MoveType::capture, from_square, to_square);
        }
    }
}

void MoveGenerator::generate_pawn_moves(GameState& game_state, int from_square,
                                        bool only_queen_promotions)
{
    const int row = from_square / 10;
    int to_square;
    int push_distance;
    int initial_row;
    int en_passant_row;
    int promotion_row;

    if(game_state.player_in_turn == Color::white)
    {
        push_distance = 10;
        initial_row = 3; // 2nd rank
        en_passant_row = 6; // 5th rank
        promotion_row = 8; // 7th rank
    }
    else
    {
        push_distance = -10;
        initial_row = 8; // 7th rank
        en_passant_row = 5; // 4th rank
        promotion_row = 3; // 2nd rank
    }

    if(row == promotion_row)
    {
        // Non capture promotion
        to_square = from_square + push_distance;
        if(game_state.board[to_square].type == BoardItem::empty_square)
        {
            if(only_queen_promotions)
            {
                move_list.emplace_back(MoveType::promotion_to_queen, from_square, to_square);
            }
            else
            {
                move_list.emplace_back(MoveType::promotion_to_queen, from_square, to_square);
                move_list.emplace_back(MoveType::promotion_to_rook, from_square, to_square);
                move_list.emplace_back(MoveType::promotion_to_bishop, from_square, to_square);
                move_list.emplace_back(MoveType::promotion_to_knight, from_square, to_square);
            }
        }
        // Promotion captures
        for(int d = -1; d <= 1; d += 2)
        {
            to_square = from_square + push_distance + d;
            if(game_state.board[to_square].type != BoardItem::empty_square
               and game_state.board[to_square].type != BoardItem::outside_of_board)
            {
                if(only_queen_promotions)
                {
                    move_list.emplace_back(MoveType::promotion_capture_to_queen, from_square, to_square);
                }
                else
                {
                    move_list.emplace_back(MoveType::promotion_capture_to_queen, from_square, to_square);
                    move_list.emplace_back(MoveType::promotion_capture_to_rook, from_square, to_square);
                    move_list.emplace_back(MoveType::promotion_capture_to_bishop, from_square, to_square);
                    move_list.emplace_back(MoveType::promotion_capture_to_knight, from_square, to_square);
                }
            }
        }
    }
    else
    {
        // Non capture
        to_square = from_square + push_distance;
        if(game_state.board[to_square].type == BoardItem::empty_square)
        {
            move_list.emplace_back(MoveType::non_capture, from_square, to_square);

            if(row == initial_row)
            {
                to_square += push_distance;
                if(game_state.board[to_square].type == BoardItem::empty_square)
                {
                    move_list.emplace_back(MoveType::double_pawn_push, from_square, to_square);
                }
            }
        }
        // Ordinary captures
        for(int d = -1; d <= 1; d += 2)
        {
            to_square = from_square + push_distance + d;
            if(game_state.board[to_square].type != BoardItem::empty_square
               and game_state.board[to_square].type != BoardItem::outside_of_board)
            {
                if(game_state.board[to_square].color != game_state.player_in_turn)
                {
                    move_list.emplace_back(MoveType::capture, from_square, to_square);
                }
            }
        }
        // En passant
        if(row == en_passant_row)
        {
            if(game_state.en_passant_target_square)
            {
                if(from_square + push_distance - 1 == game_state.en_passant_target_square)
                {
                    move_list.emplace_back(MoveType::en_passant, from_square, game_state.en_passant_target_square);
                }
                else if(from_square + push_distance + 1 == game_state.en_passant_target_square)
                {
                    move_list.emplace_back(MoveType::en_passant, from_square, game_state.en_passant_target_square);
                }
            }
        }
    }

}

void MoveGenerator::generate_knight_moves(GameState& game_state, int from_square)
{
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
            move_list.emplace_back(MoveType::non_capture, from_square, to_square);
            continue;
        }
        if(game_state.board[to_square].color != game_state.player_in_turn)
        {
            move_list.emplace_back(MoveType::capture, from_square, to_square);
        }
    }
}

void MoveGenerator::generate_bishop_moves(GameState& game_state, int from_square)
{
    const int changes[4] = {9, 11, -9, -11};

    for(int change : changes)
    {
        int to_square = from_square + change;
        while(true)
        {
            if(game_state.board[to_square].type == BoardItem::empty_square)
            {
                move_list.emplace_back(MoveType::non_capture, from_square, to_square);
                to_square += change;
                continue;
            }
            if(game_state.board[to_square].type == BoardItem::outside_of_board)
            {
                break;
            }
            if(game_state.board[to_square].color != game_state.player_in_turn)
            {
                move_list.emplace_back(MoveType::capture, from_square, to_square);
            }
            break;
        }
    }
}

void MoveGenerator::generate_rook_moves(GameState& game_state, int from_square)
{
    const int changes[4] = {10, -10, 1, -1};

    for(int change : changes)
    {
        int to_square = from_square + change;
        while(true)
        {
            if(game_state.board[to_square].type == BoardItem::empty_square)
            {
                move_list.emplace_back(MoveType::non_capture, from_square, to_square);
                to_square += change;
                continue;
            }
            if(game_state.board[to_square].type == BoardItem::outside_of_board)
            {
                break;
            }
            if(game_state.board[to_square].color != game_state.player_in_turn)
            {
                move_list.emplace_back(MoveType::capture, from_square, to_square);
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
