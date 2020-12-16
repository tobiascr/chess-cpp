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
    while(FEN_string[i] != ' ')
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
