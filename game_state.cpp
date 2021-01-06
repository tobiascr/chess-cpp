#include "game_state.h"

namespace Color
{
    ColorType opposite_color(ColorType color)
    {
        return static_cast<ColorType>(-color);
    }
}

GameState::GameState()
{
    for(int i=0; i<120; i++)
    {
        board[i].type = BoardItem::outside_of_board;
    }
    load_FEN_string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    number_of_captured_pieces = 0;
}

void GameState::load_FEN_string(std::string FEN_string)
{
    position_history.clear();

    // Reset board
    for(int r=20; r<=90; r+=10)
    {
        for(int c=1; c<=8; c++)
        {
            board[r+c].type = BoardItem::empty_square;
        }
    }

    int i = 0; // Index in string
    int square = 91;

    // Load position
    while(FEN_string[i] != ' ' or square == 91) // This allows initial spaces.
    {
        switch(FEN_string[i])
        {
            case 'P':
                board[square].type = BoardItem::pawn;
                board[square].color = Color::white;
                square++;
                break;
            case 'p':
                board[square].type = BoardItem::pawn;
                board[square].color = Color::black;
                square++;
                break;
            case 'N':
                board[square].type = BoardItem::knight;
                board[square].color = Color::white;
                square++;
                break;
            case 'n':
                board[square].type = BoardItem::knight;
                board[square].color = Color::black;
                square++;
                break;
            case 'B':
                board[square].type = BoardItem::bishop;
                board[square].color = Color::white;
                square++;
                break;
            case 'b':
                board[square].type = BoardItem::bishop;
                board[square].color = Color::black;
                square++;
                break;
            case  'R':
                board[square].type = BoardItem::rook;
                board[square].color = Color::white;
                square++;
                break;
            case 'r':
                board[square].type = BoardItem::rook;
                board[square].color = Color::black;
                square++;
                break;
            case 'Q':
                board[square].type = BoardItem::queen;
                board[square].color = Color::white;
                square++;
                break;
            case 'q':
                board[square].type = BoardItem::queen;
                board[square].color = Color::black;
                square++;
                break;
            case 'K':
                board[square].type = BoardItem::king;
                board[square].color = Color::white;
                square++;
                break;
            case 'k':
                board[square].type = BoardItem::king;
                board[square].color = Color::black;
                square++;
                break;
            case '/':
                square -= 18;
                break;
            case '1':
                square += 1;
                break;
            case '2':
                square += 2;
                break;
            case '3':
                square += 3;
                break;
            case '4':
                square += 4;
                break;
            case '5':
                square += 5;
                break;
            case '6':
                square += 6;
                break;
            case '7':
                square += 7;
                break;
            case '8':
                square += 8;
                break;
        }
        i++;
    }

    // Load active color
    i++;
    if(FEN_string[i] == 'w')
    {
        player_in_turn = Color::white;
    }
    else
    {
        player_in_turn = Color::black;
    }

    // Load castling rights
    i += 2;
    white_kingside_castling = false;
    white_queenside_castling = false;
    black_kingside_castling = false;
    black_queenside_castling = false;
    if(FEN_string[i] != '-')
    {
        while(FEN_string[i] != ' ')
        {
            switch(FEN_string[i])
            {
                case 'K':
                    white_kingside_castling = true;
                    break;
                case 'Q':
                    white_queenside_castling = true;
                    break;
                case 'k':
                    black_kingside_castling = true;
                    break;
                case 'q':
                    black_queenside_castling = true;
                    break;
            }
            i++;
        }
    }
    else
    {
        i++;
    }

    // Load en passant target square
    i++;
    if(FEN_string[i] == '-')
    {
        en_passant_target_square = 0;
    }
    else
    {
        std::string square = FEN_string.substr(i, 2);
        en_passant_target_square = internal_format[square];
    }

    position_history.push_back(get_unique_key());
}

void GameState::make_move(Move& move)
{
    int captured_pawn_square;
    en_passant_target_square_history.push(en_passant_target_square);
    en_passant_target_square = 0;

    switch(move.type)
    {
        case MoveType::non_capture:
            board[move.to_square] = board[move.from_square];
            board[move.from_square].type = BoardItem::empty_square;
            break;
        case MoveType::capture:
            captured_pieces[number_of_captured_pieces] = board[move.to_square];
            number_of_captured_pieces++;
            board[move.to_square] = board[move.from_square];
            board[move.from_square].type = BoardItem::empty_square;
            break;
        case MoveType::double_pawn_push:
            board[move.to_square] = board[move.from_square];
            board[move.from_square].type = BoardItem::empty_square;
            en_passant_target_square = move.to_square - 10 * player_in_turn;
            break;
        case MoveType::en_passant:
            captured_pawn_square = move.to_square - 10 * player_in_turn;
            captured_pieces[number_of_captured_pieces] = board[captured_pawn_square];
            number_of_captured_pieces++;
            board[move.to_square] = board[move.from_square];
            board[move.from_square].type = BoardItem::empty_square;
            board[captured_pawn_square].type = BoardItem::empty_square;
            break;
        case MoveType::white_kingside_castling:
            board[27] = board[25];
            board[26] = board[28];
            board[25].type = BoardItem::empty_square;
            board[28].type = BoardItem::empty_square;
            break;
        case MoveType::white_queenside_castling:
            board[23] = board[25];
            board[24] = board[21];
            board[21].type = BoardItem::empty_square;
            board[25].type = BoardItem::empty_square;
            break;
        case MoveType::black_kingside_castling:
            board[97] = board[95];
            board[96] = board[98];
            board[95].type = BoardItem::empty_square;
            board[98].type = BoardItem::empty_square;
            break;
        case MoveType::black_queenside_castling:
            board[93] = board[95];
            board[94] = board[91];
            board[91].type = BoardItem::empty_square;
            board[95].type = BoardItem::empty_square;
            break;
        case MoveType::promotion_to_queen:
            board[move.to_square].type = BoardItem::queen;
            board[move.to_square].color = player_in_turn;
            board[move.from_square].type = BoardItem::empty_square;
            break;
        case MoveType::promotion_to_rook:
            board[move.to_square].type = BoardItem::rook;
            board[move.to_square].color = player_in_turn;
            board[move.from_square].type = BoardItem::empty_square;
            break;
        case MoveType::promotion_to_bishop:
            board[move.to_square].type = BoardItem::bishop;
            board[move.to_square].color = player_in_turn;
            board[move.from_square].type = BoardItem::empty_square;
            break;
        case MoveType::promotion_to_knight:
            board[move.to_square].type = BoardItem::knight;
            board[move.to_square].color = player_in_turn;
            board[move.from_square].type = BoardItem::empty_square;
            break;
        case MoveType::promotion_capture_to_queen:
            captured_pieces[number_of_captured_pieces] = board[move.to_square];
            number_of_captured_pieces++;
            board[move.to_square].type = BoardItem::queen;
            board[move.to_square].color = player_in_turn;
            board[move.from_square].type = BoardItem::empty_square;
            break;
        case MoveType::promotion_capture_to_rook:
            captured_pieces[number_of_captured_pieces] = board[move.to_square];
            number_of_captured_pieces++;
            board[move.to_square].type = BoardItem::rook;
            board[move.to_square].color = player_in_turn;
            board[move.from_square].type = BoardItem::empty_square;
            break;
        case MoveType::promotion_capture_to_bishop:
            captured_pieces[number_of_captured_pieces] = board[move.to_square];
            number_of_captured_pieces++;
            board[move.to_square].type = BoardItem::bishop;
            board[move.to_square].color = player_in_turn;
            board[move.from_square].type = BoardItem::empty_square;
            break;
        case MoveType::promotion_capture_to_knight:
            captured_pieces[number_of_captured_pieces] = board[move.to_square];
            number_of_captured_pieces++;
            board[move.to_square].type = BoardItem::knight;
            board[move.to_square].color = player_in_turn;
            board[move.from_square].type = BoardItem::empty_square;
            break;
    }
    player_in_turn = Color::opposite_color(player_in_turn);
}

void GameState::make_move(std::string uci_format_move)
{
    player_in_turn = Color::opposite_color(player_in_turn);

    std::string conventional_format_from_square = uci_format_move.substr(0, 2);
    std::string conventional_format_to_square = uci_format_move.substr(2, 2);

    int from_square = internal_format[conventional_format_from_square];
    int to_square = internal_format[conventional_format_to_square];

    board[to_square] = board[from_square];
    board[from_square].type = BoardItem::empty_square;

    if(board[to_square].type == BoardItem::pawn)
    {
        // If en passant
        if(to_square == en_passant_target_square)
        {
            board[to_square - 10 * board[to_square].color].type = BoardItem::empty_square;
        }
    }

    en_passant_target_square = 0;

    if(board[to_square].type == BoardItem::pawn)
    {
        // If white double pawn push
        if(to_square - from_square == 20)
        {
            en_passant_target_square = to_square - 10;
        }

        // If black double pawn push
        if(to_square - from_square == -20)
        {
            en_passant_target_square = to_square + 10;
        }
    }

    // Update castling rights
    if(conventional_format_from_square == "a1")
    {
        white_queenside_castling = false;
    }
    if(conventional_format_to_square == "a1")
    {
        white_queenside_castling = false;
    }
    if(conventional_format_from_square == "e1")
    {
        white_kingside_castling = false;
        white_queenside_castling = false;
    }
    if(conventional_format_from_square == "h1")
    {
        white_kingside_castling = false;
    }
    if(conventional_format_to_square == "h1")
    {
        white_kingside_castling = false;
    }
    if(conventional_format_from_square == "a8")
    {
        black_queenside_castling = false;
    }
    if(conventional_format_to_square == "a8")
    {
        black_queenside_castling = false;
    }
    if(conventional_format_from_square == "e8")
    {
        black_kingside_castling = false;
        black_queenside_castling = false;
    }
    if(conventional_format_from_square == "h8")
    {
        black_kingside_castling = false;
    }
    if(conventional_format_to_square == "h8")
    {
        black_kingside_castling = false;
    }

    // If castling also move the rook.
    if(uci_format_move == "e1g1")
    {
        board[26] = board[28];
        board[28].type = BoardItem::empty_square;
    }
    if(uci_format_move == "e1c1")
    {
        board[24] = board[21];
        board[21].type = BoardItem::empty_square;
    }
    if(uci_format_move == "e8g8")
    {
        board[96] = board[98];
        board[98].type = BoardItem::empty_square;
    }
    if(uci_format_move == "e8c8")
    {
        board[94] = board[91];
        board[91].type = BoardItem::empty_square;
    }

    // If promotion
    if(uci_format_move.size() == 5)
    {
        if(uci_format_move[4] == 'q')
        {
            board[to_square].type = BoardItem::queen;
        }
        if(uci_format_move[4] == 'r')
        {
            board[to_square].type = BoardItem::rook;
        }
        if(uci_format_move[4] == 'b')
        {
            board[to_square].type = BoardItem::bishop;
        }
        if(uci_format_move[4] == 'n')
        {
            board[to_square].type = BoardItem::knight;
        }
    }

    position_history.push_back(get_unique_key());
}

void GameState::undo_move(Move& move)
{
    int captured_pawn_square;
    player_in_turn = Color::opposite_color(player_in_turn);
    en_passant_target_square = en_passant_target_square_history.top();
    en_passant_target_square_history.pop();

    switch(move.type)
    {
        case MoveType::non_capture:
            board[move.from_square] = board[move.to_square];
            board[move.to_square].type = BoardItem::empty_square;
            break;
        case MoveType::capture:
            number_of_captured_pieces--;
            board[move.from_square] = board[move.to_square];
            board[move.to_square] = captured_pieces[number_of_captured_pieces];
            break;
        case MoveType::double_pawn_push:
            board[move.from_square] = board[move.to_square];
            board[move.to_square].type = BoardItem::empty_square;
            break;
        case MoveType::en_passant:
            number_of_captured_pieces--;
            captured_pawn_square = move.to_square - 10 * player_in_turn;
            board[captured_pawn_square] = captured_pieces[number_of_captured_pieces];
            board[move.from_square] = board[move.to_square];
            board[move.to_square].type = BoardItem::empty_square;
            break;
        case MoveType::white_kingside_castling:
            board[25] = board[27];
            board[28] = board[26];
            board[26].type = BoardItem::empty_square;
            board[27].type = BoardItem::empty_square;
            break;
        case MoveType::white_queenside_castling:
            board[25] = board[23];
            board[21] = board[24];
            board[23].type = BoardItem::empty_square;
            board[24].type = BoardItem::empty_square;
            break;
        case MoveType::black_kingside_castling:
            board[95] = board[97];
            board[98] = board[96];
            board[96].type = BoardItem::empty_square;
            board[97].type = BoardItem::empty_square;
            break;
        case MoveType::black_queenside_castling:
            board[95] = board[93];
            board[91] = board[94];
            board[93].type = BoardItem::empty_square;
            board[94].type = BoardItem::empty_square;
            break;
        case MoveType::promotion_to_queen:
            board[move.from_square].type = BoardItem::pawn;
            board[move.from_square].color = player_in_turn;
            board[move.to_square].type = BoardItem::empty_square;
            break;
        case MoveType::promotion_to_rook:
            board[move.from_square].type = BoardItem::pawn;
            board[move.from_square].color = player_in_turn;
            board[move.to_square].type = BoardItem::empty_square;
            break;
        case MoveType::promotion_to_bishop:
            board[move.from_square].type = BoardItem::pawn;
            board[move.from_square].color = player_in_turn;
            board[move.to_square].type = BoardItem::empty_square;
            break;
        case MoveType::promotion_to_knight:
            board[move.from_square].type = BoardItem::pawn;
            board[move.from_square].color = player_in_turn;
            board[move.to_square].type = BoardItem::empty_square;
            break;
        case MoveType::promotion_capture_to_queen:
            number_of_captured_pieces--;
            board[move.to_square] = captured_pieces[number_of_captured_pieces];
            board[move.from_square].type = BoardItem::pawn;
            board[move.from_square].color = player_in_turn;
            break;
        case MoveType::promotion_capture_to_rook:
            number_of_captured_pieces--;
            board[move.to_square] = captured_pieces[number_of_captured_pieces];
            board[move.from_square].type = BoardItem::pawn;
            board[move.from_square].color = player_in_turn;
            break;
        case MoveType::promotion_capture_to_bishop:
            number_of_captured_pieces--;
            board[move.to_square] = captured_pieces[number_of_captured_pieces];
            board[move.from_square].type = BoardItem::pawn;
            board[move.from_square].color = player_in_turn;
            break;
        case MoveType::promotion_capture_to_knight:
            number_of_captured_pieces--;
            board[move.to_square] = captured_pieces[number_of_captured_pieces];
            board[move.from_square].type = BoardItem::pawn;
            board[move.from_square].color = player_in_turn;
            break;
    }
}

void GameState::make_null_move()
{
    player_in_turn = Color::opposite_color(player_in_turn);
    en_passant_target_square_history.push(en_passant_target_square);
    en_passant_target_square = 0;
}

void GameState::undo_null_move()
{
    player_in_turn = Color::opposite_color(player_in_turn);
    en_passant_target_square = en_passant_target_square_history.top();
    en_passant_target_square_history.pop();
}

std::string GameState::get_unique_key()
{
    std::string key;
    key.resize(64);
    int i = 0;

    for(int square=21; square<=98; square++)
    {
        switch(board[square].type)
        {
            case BoardItem::king:
                if(board[square].color == Color::white)
                {
                    key[i] = 'K';
                }
                else
                {
                    key[i] = 'k';
                }
                break;
            case BoardItem::queen:
                if(board[square].color == Color::white)
                {
                    key[i] = 'Q';
                }
                else
                {
                    key[i] = 'q';
                }
                break;
            case BoardItem::rook:
                if(board[square].color == Color::white)
                {
                    key[i] = 'R';
                }
                else
                {
                    key[i] = 'r';
                }
                break;
            case BoardItem::bishop:
                if(board[square].color == Color::white)
                {
                    key[i] = 'B';
                }
                else
                {
                    key[i] = 'b';
                }
                break;
            case BoardItem::knight:
                if(board[square].color == Color::white)
                {
                    key[i] = 'N';
                }
                else
                {
                    key[i] = 'n';
                }
                break;
            case BoardItem::pawn:
                if(board[square].color == Color::white)
                {
                    key[i] = 'P';
                }
                else
                {
                    key[i] = 'p';
                }
                break;
            case BoardItem::empty_square:
                key[i] = 'x';
                break;
            case BoardItem::outside_of_board:
                i--;
                break;
        }
        i++;
    }

    key += std::to_string(player_in_turn);
    key += std::to_string(en_passant_target_square);

    if(white_kingside_castling)
    {
        key += 'K';
    }
    if(white_queenside_castling)
    {
        key += 'Q';
    }
    if(black_kingside_castling)
    {
        key += 'k';
    }
    if(black_queenside_castling)
    {
        key += 'q';
    }

    return key;
}

bool GameState::threefold_repetition()
{
    std::string current_key = get_unique_key();
    int times = 0;
    for(std::string key : position_history)
    {
        if(current_key == key)
        {
            times++;
        }
    }
    return times >= 3;
}
