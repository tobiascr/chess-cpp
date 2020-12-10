#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <vector>
#include "move.h"
#include "game_state.h"

class MoveGenerator
{
public:
    std::vector<Move>& get_moves(GameState& game_state);

private:
    std::vector<Move> move_list;

    void generate_king_moves(GameState& game_state, int square);
};

#endif
