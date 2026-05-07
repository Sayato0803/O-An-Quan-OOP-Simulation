#include "ai.h"
#include <iostream>
#include <algorithm>
#include "board.h"

void AI::find_best_move_P1(board draft_board, int& best_square, int& best_direction, int depth, int w_score, int w_board, int w_starve) {
    int min_heuristic = 999999;
    int alpha = -999999;
    int beta = 999999;
    best_square = -1;
    best_direction = 0;

    for (int i = 0; i <= 4; ++i) {
        if (draft_board.check_square(i) > 0) {
            for (int dir: {-1, 1}) {
                board test_board = draft_board;
                test_board.move(i, dir);
                test_board.change_turn();

                int score = minimax(test_board, depth - 1, alpha, beta, true, w_score, w_board, w_starve);
                if (score < min_heuristic) {
                    min_heuristic = score;
                    best_square = i;
                    best_direction = dir;
                }
                beta = std::min(beta, min_heuristic);
            }
        }
    }
}

void AI::find_best_move_P2(board draft_board, int& best_square, int& best_direction, int depth, int w_score, int w_board, int w_starve) {
    int max_heuristic = -999999;
    int alpha = -999999;
    int beta = 999999;
    best_square = -1;
    best_direction = 0;

    for (int i = 6; i <= 10; ++i) {
        if (draft_board.check_square(i) > 0) {
            for (int dir: {-1, 1}) {
                board test_board = draft_board;
                test_board.move(i, dir);
                test_board.change_turn();

                int score = minimax(test_board, depth - 1, alpha, beta, false, w_score, w_board, w_starve);
                if (score > max_heuristic) {
                    max_heuristic = score;
                    best_square = i;
                    best_direction = dir;
                }
                alpha = std::max(alpha, max_heuristic);
            }
        }
    }
}

int AI::minimax(board current_board, int depth, int alpha, int beta, bool is_maximizing, int w_score, int w_board, int w_starve) {

    if (depth == 0 || current_board.is_game_over()) {
        return current_board.evaluate_state(w_score, w_board, w_starve);
    }

    if (is_maximizing) {
        int max_eval = -9999999;
        bool has_move = false;

        for (int i = 6; i <= 10; ++i) {
            if (current_board.check_square(i) > 0) {
                for (int dir: {-1, 1}) {
                    has_move = true;
                    board next_board = current_board;
                    next_board.move(i, dir);
                    next_board.change_turn();

                    int eval = minimax(next_board, depth - 1, alpha, beta, false, w_score, w_board, w_starve);
                    max_eval = std::max(eval, max_eval);

                    alpha = std::max(alpha, eval);
                    if (beta <= alpha) break;
                }
                if (beta <= alpha) break;
            }
        }
        return has_move ? max_eval : current_board.evaluate_state(w_score, w_board, w_starve);

    } else {
        int min_eval = 999999;
        bool has_move = false;

        for (int i = 0; i <= 4; ++i) {
            if (current_board.check_square(i) > 0) {
                for (int dir: {-1, 1}) {
                    has_move = true;
                    board next_board = current_board;
                    next_board.move(i, dir);
                    next_board.change_turn();

                    int eval = minimax(next_board, depth - 1, alpha, beta, true, w_score, w_board, w_starve);
                    min_eval = std::min(eval, min_eval);

                    beta = std::min(beta, eval);
                    if (beta <= alpha) break;
                }
                if (beta <= alpha) break;
            }
        }
        return has_move ? min_eval : current_board.evaluate_state(w_score, w_board, w_starve);
    }
}