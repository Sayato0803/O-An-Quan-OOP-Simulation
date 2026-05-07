#ifndef O_AN_QUAN_BOARD_H
#define O_AN_QUAN_BOARD_H
#include "square.h"
class board {
private:
    square sq[12];
    int scoreP1, scoreP2;
    int turn;
public:
    board() {
        scoreP1 = 0; scoreP2 = 0;
        turn = 1;
        sq[5].set_king();
        sq[11].set_king();
    }
    int evaluate_state(int w_score = 10,int w_board = 2,int w_starve = 50);
    int get_turn() const {
        return turn;
    }
    int get_scoreP1() const {
        return scoreP1;
    }
    int get_scoreP2() const {
        return scoreP2;
    }
    void print_board();
    bool is_game_over();
    void move(int start_idx, int direction);
    bool null_chess() {
        int s = (turn == 1)? 0 :  6;
        int e = (turn == 1)? 4 : 10;
        int ok = 0;
        for (int i = s; i <= e; ++i) {
            if (sq[i].get_count_chess() != 0) {
                ok = 1; break;
            }
        }
        return ok;
    }
    void update_board();
    void change_turn() {
        this->turn = (this->turn == 1) ? 2 : 1;
    }
    int check_square(int idx) {
        return (sq[idx].get_count_chess());
    }
};

#endif //O_AN_QUAN_BOARD_H
