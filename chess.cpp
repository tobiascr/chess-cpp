#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include "move.h"
#include "game_state.h"
#include "move_generator.h"

std::mt19937 random_generator;

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
if favorable for the other player. The value is given in centipawns.*/
{
    int value = 0;

    for(int r=20; r<=90; r+=10)
    {
        for(int c=1; c<=8; c++)
        {
            switch(game_state.board[r+c].type)
            {
                case BoardItem::king:
                    value += 100000 * game_state.board[r+c].color;
                    break;
                case BoardItem::queen:
                    value += 900 * game_state.board[r+c].color;
                    break;
                case BoardItem::rook:
                    value += 500 * game_state.board[r+c].color;
                    break;
                case BoardItem::bishop:
                    value += 300 * game_state.board[r+c].color;
                    break;
                case BoardItem::knight:
                    value += 300 * game_state.board[r+c].color;
                    break;
                case BoardItem::pawn:
                    value += 100 * game_state.board[r+c].color;
                    break;
            }
        }
    }
    return value * game_state.player_in_turn;
}

void move_sort(std::vector<Move>& move_list)
{
    std::vector<Move> list_1, list_2;

    // Creating some randomness by shuffling the list before sorting it.
    std::shuffle(move_list.begin(), move_list.end(), random_generator);

    for(Move move : move_list)
    {
        if(move.type == MoveType::capture)
        {
            list_1.push_back(move);
        }
        else
        {
            list_2.push_back(move);
        }
    }

    for(int i=0; i< list_1.size(); i++)
    {
        move_list[i] = list_1[i];
    }

    for(int i=0; i< list_2.size(); i++)
    {
        move_list[i + list_1.size()] = list_2[i];
    }
}

bool stale_mate(GameState& game_state)
{
    MoveGenerator generator;

    // Is there any legal moves?
    std::vector<Move>& move_list =
                       generator.get_moves_no_castlings_only_queen_promotions(game_state);
    if(move_list.size() > 0)
    {
        return false; // If there are legal moves, it's not a stale mate.
    }

    // Is the player in turn in check?
    game_state.make_null_move();
    move_list = generator.get_pseudo_legal_moves_no_castlings_only_queen_promotions(game_state);
    for(Move move : move_list)
    {
        if(game_state.board[move.to_square].type == BoardItem::king)
        {
            game_state.undo_null_move();
            return false; // If the king is in check, it's not a stale mate.
        }
    }
    game_state.undo_null_move();
    return true;
}

int negamax(GameState& game_state, const int depth, int alpha, int beta)
{
    int value = position_value(game_state);

    // If full depth is reached or the king is captured.
    if(depth == 0 or value < -50000)
    {
        if(value < -50000)
        {
            return value - depth;
        }
        return value;
    }

    MoveGenerator generator;
    std::vector<Move>& move_list =
               generator.get_pseudo_legal_moves_no_castlings_only_queen_promotions(game_state);

    if(move_list.size() == 0)
    {
        return 0;
    }

    move_sort(move_list);

    for(Move move : move_list)
    {
        game_state.make_move(move);
        value = -negamax(game_state, depth - 1, -beta, -alpha);
        game_state.undo_move(move);
        if (value >= beta)
        {
            return beta;
        }
        if(value > alpha)
        {
            alpha = value;
        }
    }

    return alpha;
}

std::string root_negamax(GameState& game_state, const int depth)
// Return a best move.
{
    MoveGenerator generator;
    std::vector<Move>& move_list =
               generator.get_moves_no_castlings_only_queen_promotions(game_state);

    if(move_list.size() == 0)
    {
        return "No moves found.";
    }

    move_sort(move_list);

    int value;
    int alpha = -1000000;
    int beta = 1000000;
    Move best_move = move_list[0];

    for(Move move : move_list)
    {
        game_state.make_move(move);
        if(stale_mate(game_state))
        {
            value = 0;
        }
        else
        {
            value = -negamax(game_state, depth - 1, -beta, -alpha);
        }
        game_state.undo_move(move);
        if(value > alpha)
        {
            alpha = value;
            best_move = move;
        }
    }

    std::cout << "info depth " << depth << " score cp " << alpha << std::endl;
    return best_move.UCI_format();
}

void iterative_deepening(GameState& game_state, int max_time_milliseconds)
{
    std::string best_move;
    std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point t1;
    int depth = 1;

    while(std::chrono::steady_clock::now() - t0 <
          std::chrono::milliseconds(max_time_milliseconds))
    {
        best_move = root_negamax(game_state, depth);
        depth++;
    }
    std::cout << "bestmove " + best_move << std::endl;
}

bool substring_in_string(std::string& string, std::string substring)
/* True if and only if substring is in the string.*/
{
    return string.find(substring) != std::string::npos;
}

void remove_substring(std::string& string, std::string substring)
/* Remove the first occurence of substring from the string.*/
{
    string.erase(string.find(substring), substring.size());
}

std::vector<std::string> split(std::string string)
/* Return a list of strings found in the given string that are separated by
whitespaces.*/
{
    std::stringstream s;
    s << string;
    std::string substring;
    std::vector<std::string> substrings;
    while(s >> substring)
    {
        substrings.push_back(substring);
    }
    return substrings;
}

void UCI_loop()
{
    GameState game_state;
    std::string input, line;
    std::string FEN_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::cout << "id name chess-cpp" << std::endl;
    std::cout << "id author Tobias C" << std::endl;
    std::cout << "uciok" << std::endl;

    while(true)
    {
        std::cin >> input;
        if(input == "position")
        {
            std::getline(std::cin, line);
            if(substring_in_string(line, "startpos"))
            {
                FEN_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
                game_state.load_FEN_string(FEN_string);
            }
            if(substring_in_string(line, "fen"))
            {
                remove_substring(line, "fen");
                game_state.load_FEN_string(line);
            }
            if(substring_in_string(line, "moves"))
            {
                // Remove everything in the string to the left of and including "moves".
                line.erase(0, line.find("moves") + 5);
                std::vector<std::string> move_list = split(line);
                for(std::string uci_format_move : move_list)
                {
                    game_state.make_move(uci_format_move);
                }
            }
        }
        else if(input == "isready")
        {
            std::cout << "readyok" << std::endl;
        }
        else if(input == "go")
        {
            iterative_deepening(game_state, 100);
        }
        else if(input == "quit")
        {
            break;
        }
    }
}

int main()
{
    std::random_device rd;
    random_generator.seed(rd());

    std::string input;
    std::cout << "Type uci for UCI-mode, or give a FEN-string to compute a move:" << std::endl;
    std::cout << std::endl;
    std::cout << "Input: ";
    std::getline(std::cin, input);

    if(input == "uci")
    {
        UCI_loop();
    }
    else
    {
        GameState game_state;
        game_state.load_FEN_string(input);
        std::cout << std::endl;
        print_board(game_state);
        iterative_deepening(game_state, 100);
    }

    return 0;
}
