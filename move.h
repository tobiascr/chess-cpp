#ifndef MOVE_H
#define MOVE_H

enum MoveType {non_capture, capture, en_passant, promotion_to_queen,
               promotion_to_rook, promotion_to_bishop, promotion_to_knight,
               promotion_capture_to_queen, promotion_capture_to_rook,
               promotion_capture_to_bishop, promotion_capture_to_knight,
               kingside_castling, queenside_castling};

class Move
{
public:
    MoveType type;
    int from_square, to_square;
};

#endif
