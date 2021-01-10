#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <utility>
#include <unordered_map>
#include "move.h"
#include "game_state.h"
#include "move_generator.h"

std::mt19937 random_generator;

class TranspositionTableData
{
public:
    enum Type{lower_bound, upper_bound};
    int value;
    int depth;
    Move best_move;
    Type type;
};

std::unordered_map<std::string, TranspositionTableData> transposition_table;

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

    for(int row=0; row<=7; row++)
    {
        for(int col=0; col<=7; col++)
        {
            int square = row * 10 + col + 21;
            switch(game_state.board[square].type)
            {
                case BoardItem::king:
                    value += 100000 * game_state.board[square].color;
                    break;
                case BoardItem::queen:
                    value += 900 * game_state.board[square].color;
                    break;
                case BoardItem::rook:
                    value += 500 * game_state.board[square].color;
                    break;
                case BoardItem::bishop:
                    value += 300 * game_state.board[square].color;
                    break;
                case BoardItem::knight:
                    // Knights gets more valueable when they are closer to the center.
                    value += (305 - (2 * row - 7) * (2 * row - 7) *
                             (2 * col - 7) * (2 * col - 7) / 240) * game_state.board[square].color;
                    break;
                case BoardItem::pawn:
                    // Pawns gets more valuable when they are closer to promotion.
                    if(game_state.board[square].color == Color::white)
                    {
                        value += 100 + row;
                    }
                    else
                    {
                        value -= 107 - row;
                    }
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
    std::string unique_key;
    int value = position_value(game_state);

    // If the king is captured.
    if(value < -50000)
    {
        return -100000 - depth;
    }

    // If full depth is reached.
    if(depth == 0)
    {
        if(value >= 0)
        {
            return value + depth;
        }
        else
        {
            return value - depth;
        }
    }

    const bool use_transposition_table = depth > 2;
    if(use_transposition_table)
    {
        unique_key = game_state.get_unique_key();
        if(transposition_table.count(unique_key))
        {
            const auto tt_data = transposition_table[unique_key];

            if(tt_data.depth >= depth and tt_data.type == TranspositionTableData::lower_bound)
            {
                if (tt_data.value > alpha)
                {
                    alpha = tt_data.value;
                }
                if(alpha >= beta)
                {
                    return beta;
                }
            }

            if(tt_data.depth >= depth and tt_data.type == TranspositionTableData::upper_bound)
            {
                if (tt_data.value < beta)
                {
                    beta = tt_data.value;
                }
                if(alpha >= beta)
                {
                    return alpha;
                }
            }
        }
    }

    MoveGenerator generator;
    std::vector<Move>& move_list =
               generator.get_pseudo_legal_moves_no_castlings_only_queen_promotions(game_state);

    if(move_list.size() == 0)
    {
        return 0;
    }

    move_sort(move_list);

    if(use_transposition_table)
    {
        if(transposition_table.count(unique_key))
        {
            const auto tt_data = transposition_table[unique_key];

            for(Move move : move_list)
            {
                if(move.type == tt_data.best_move.type and
                   move.from_square == tt_data.best_move.from_square and
                   move.to_square == tt_data.best_move.to_square)
                {
                    move_list.insert(move_list.begin(), tt_data.best_move);
                }
            }
        }
    }

    Move best_move = move_list[0];

    for(Move move : move_list)
    {
        game_state.make_move(move);
        value = -negamax(game_state, depth - 1, -beta, -alpha);
        game_state.undo_move(move);
        if (value >= beta)
        {
            if(use_transposition_table)
            {
                TranspositionTableData tt_data;
                tt_data.value = beta;
                tt_data.depth = depth;
                tt_data.type = TranspositionTableData::lower_bound;
                tt_data.best_move = move;
                transposition_table[unique_key] = tt_data;
             }
            return beta;
        }
        if(value > alpha)
        {
            alpha = value;
            best_move = move;
        }
    }

    if(use_transposition_table)
    {
        TranspositionTableData tt_data;
        tt_data.value = alpha;
        tt_data.depth = depth;
        tt_data.best_move = best_move;
        tt_data.type = TranspositionTableData::upper_bound;
        transposition_table[unique_key] = tt_data;
    }

    return alpha;
}

std::pair<std::string, int> root_negamax(GameState& game_state, const int depth)
// Return a best move in uci format and a position value.
{
    MoveGenerator generator;
    std::vector<Move>& move_list =
               generator.get_moves_no_castlings_only_queen_promotions(game_state);

    move_sort(move_list);

    int value;
    int alpha = -1000000;
    int beta = 1000000;
    Move best_move = move_list[0];

    std::string unique_key = game_state.get_unique_key();
    if(transposition_table.count(unique_key))
    {
        const auto tt_data = transposition_table[unique_key];

        for(Move move : move_list)
        {
            if(move.type == tt_data.best_move.type and
               move.from_square == tt_data.best_move.from_square and
               move.to_square == tt_data.best_move.to_square)
            {
                move_list.insert(move_list.begin(), tt_data.best_move);
            }
        }
    }

    for(Move move : move_list)
    {
        game_state.make_move(move);
        if(stale_mate(game_state))
        {
            value = 0;
        }
        else if(game_state.threefold_repetition())
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

    TranspositionTableData tt_data;
    tt_data.value = alpha;
    tt_data.depth = depth;
    tt_data.best_move = best_move;
    tt_data.type = TranspositionTableData::upper_bound;
    transposition_table[unique_key] = tt_data;

    std::cout << "info depth " << depth << " score cp " << alpha << std::endl;
    return std::pair<std::string, int> {best_move.UCI_format(), alpha};
}

void iterative_deepening(GameState& game_state, int max_time_milliseconds)
{
    std::string best_move;
    std::pair<std::string, int> results;
    std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point t1;
    int depth = 1;

    while(std::chrono::steady_clock::now() - t0 <
          std::chrono::milliseconds(max_time_milliseconds))
    {
        results = root_negamax(game_state, depth);
        if(results.second > 50000 or results.second < -50000)
        {
            break;
        }
        depth++;
    }

    std::cout << "bestmove " + results.first << std::endl;
}

void iterative_deepening_fixed_depth(GameState& game_state, int depth)
{
    std::string best_move;
    std::pair<std::string, int> results;
    int d = 1;

    while(d <= depth)
    {
        results = root_negamax(game_state, d);
        if(results.second > 50000 or results.second < -50000)
        {
            break;
        }
        d++;
    }

    std::cout << "bestmove " + results.first << std::endl;
}

void remove_substring(std::string& string, std::string substring)
/* Remove the first occurence of substring from the string.*/
{
    string.erase(string.find(substring), substring.size());
}

bool string_in_list(std::vector<std::string> list, std::string string)
/* True iff the string is in the list.*/
{
    for(std::string element : list)
    {
        if(element == string)
        {
            return true;
        }
    }
    return false;
}

std::string next_string(std::vector<std::string> list, std::string string)
/* Return the element in the list following the first occurence of the string.
If the string is not in the list or there is not element after the string an empty
string is returned.*/
{
    int i = 0;
    while(i < list.size() - 1)
    {
        if(list[i] == string)
        {
            return list[i + 1];
        }
        i++;
    }
    return "";
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
    std::string line;
    std::string FEN_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::cout << "id name chess-cpp" << std::endl;
    std::cout << "id author Tobias C" << std::endl;
    std::cout << "uciok" << std::endl;

    while(true)
    {
        std::getline(std::cin, line);
        std::vector<std::string> input_list = split(line);

        if(input_list[0] == "ucinewgame")
        {
            transposition_table.clear();
        }
        if(input_list[0] == "position")
        {
            if(string_in_list(input_list, "startpos"))
            {
                FEN_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
                game_state.load_FEN_string(FEN_string);
            }
            if(string_in_list(input_list, "fen"))
            {
                remove_substring(line, "position");
                remove_substring(line, "fen");
                game_state.load_FEN_string(line);
            }
            if(string_in_list(input_list, "moves"))
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
        else if(input_list[0] == "isready")
        {
            std::cout << "readyok" << std::endl;
        }
        else if(input_list[0] == "go")
        {
            if(string_in_list(input_list, "depth"))
            {
                int depth = std::stoi(next_string(input_list, "depth"));
                iterative_deepening_fixed_depth(game_state, depth);
            }
            else
            {
                iterative_deepening(game_state, 100);
            }
        }
        else if(input_list[0] == "quit")
        {
            break;
        }
    }
}

int main()
{
    std::random_device rd;
    random_generator.seed(rd());

    transposition_table.reserve(100000);

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
