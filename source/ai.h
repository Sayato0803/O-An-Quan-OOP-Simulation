#ifndef O_AN_QUAN_AI_H
#define O_AN_QUAN_AI_H
#include "board.h"
class AI {
public:
    static int minimax(board current_board, int depth, int alpha, int beta, bool is_maximizing, int w_score, int w_board, int w_starve);
    static void find_best_move_P2(board draft_board, int& best_square, int& best_direction, int depth, int w_score = 10, int w_board = 2, int w_starve = 50);
    static void find_best_move_P1(board draft_board, int& best_square, int& best_direction, int depth, int w_score = 10, int w_board = 2, int w_starve = 50);
};



#endif
