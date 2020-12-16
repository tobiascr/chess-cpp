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

int position_value(GameState& game_state)
/* Give a positive value for positions favorable for the player in turn and negative
if favorable for the other player.*/
{
    int value = 0;

    for(int r=20; r<=90; r+=10)
    {
        for(int c=1; c<=8; c++)
        {
            switch(game_state.board[r+c].type)
            {
                case BoardItem::king:
                    value += 1000 * game_state.board[r+c].color;
                    break;
                case BoardItem::queen:
                    value += 9 * game_state.board[r+c].color;
                    break;
                case BoardItem::rook:
                    value += 5 * game_state.board[r+c].color;
                    break;
                case BoardItem::bishop:
                    value += 3 * game_state.board[r+c].color;
                    break;
                case BoardItem::knight:
                    value += 3 * game_state.board[r+c].color;
                    break;
                case BoardItem::pawn:
                    value += 1 * game_state.board[r+c].color;
                    break;
            }
        }
    }
    return value * game_state.player_in_turn;
}

int negamax(GameState& game_state, const int depth)
{
    int value = position_value(game_state);

    if(depth == 0 or value < -500)
    {
        return value;
    }

    MoveGenerator generator;
    std::vector<Move>& move_list =
               generator.get_moves_no_castlings_only_queen_promotions(game_state);

    if(move_list.size() == 0)
    {
        return 0;
    }

    int best_value = -10000;
    for(Move move : move_list)
    {
        game_state.make_move(move);
        value = -negamax(game_state, depth - 1);
        game_state.undo_move(move);
        if(value > best_value)
        {
            best_value = value;
        }
    }

    return best_value;
}

std::string root_negamax(GameState& game_state, const int depth)
{
    MoveGenerator generator;
    std::vector<Move>& move_list =
               generator.get_moves_no_castlings_only_queen_promotions(game_state);

    if(move_list.size() == 0)
    {
        return "No moves found.";
    }

    int best_value = -10000;

    Move best_move = move_list[0];

    for(Move move : move_list)
    {
        game_state.make_move(move);
        int value = -negamax(game_state, depth - 1);
        game_state.undo_move(move);
        if(value > best_value)
        {
            best_value = value;
            best_move = move;
        }
    }

    return "bestmove " + best_move.UCI_format() + " score " + std::to_string(best_value);
}

int main()
{
    GameState game_state;
    std::string FEN_string;
    std::cout << "FEN: ";
    std::getline(std::cin, FEN_string);
    std::cout << std::endl;
    game_state.load_FEN_string(FEN_string);
    print_board(game_state);
    std::cout << root_negamax(game_state, 5) << std::endl;

    return 0;
}
