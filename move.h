#ifndef MOVE_H
#define MOVE_H

#include <string>

enum MoveType {non_capture, capture, en_passant, double_pawn_push, promotion_to_queen,
               promotion_to_rook, promotion_to_bishop, promotion_to_knight,
               promotion_capture_to_queen, promotion_capture_to_rook,
               promotion_capture_to_bishop, promotion_capture_to_knight,
               white_kingside_castling, white_queenside_castling,
               black_kingside_castling, black_queenside_castling};

class Move
{
public:
    MoveType type;
    int from_square, to_square;

    std::string UCI_format();
    /* Return the move in UCI format. Examples:  e2e4, e7e5, e1g1 (white short castling),
    e7e8q (for promotion), e4f3 (en passant), f7g8q (promotion capture).*/
};

#endif
