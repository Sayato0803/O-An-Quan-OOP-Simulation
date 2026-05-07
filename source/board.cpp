#include "board.h"
#include <iostream>
#include <iomanip>
#include "windows.h"

extern long long death_count[12];

void board::print_board(){
    std::cout << "=======================================\n";
    std::cout << "           GAME O AN QUAN\n";
    std::cout << "=======================================\n";
    char fill_char = std::cout.fill('0');
    std::cout << "\n       <--- DIEM P2: " << std::setw(2) << scoreP2 << " <--- \n";
    std::cout << "  ";
    for (int i = 11; i >= 5; --i) std:: cout << std::setw(2) << i << "   ";
    std::cout <<"\n";
    std::cout << "+----+----+----+----+----+----+----+\n";
    std::cout << "|    | "
    << std::setw(2) << sq[10].get_count_chess() << " | "
    << std::setw(2) << sq[9].get_count_chess() << " | "
    << std::setw(2) << sq[8].get_count_chess() << " | "
    << std::setw(2) << sq[7].get_count_chess() << " | "
    << std::setw(2) << sq[6].get_count_chess() << " |    |\n";

    std::cout << "| " << std::setw(2) << sq[11].get_count_chess()
    << " +----+----+----+----+----+ "
    << std::setw(2) << sq[5].get_count_chess() << " |\n";
    std::cout << "|    | "
    << std::setw(2) << sq[0].get_count_chess() << " | "
    << std::setw(2) << sq[1].get_count_chess() << " | "
    << std::setw(2) << sq[2].get_count_chess() << " | "
    << std::setw(2) << sq[3].get_count_chess() << " | "
    << std::setw(2) << sq[4].get_count_chess() << " |    |\n";

    std::cout << "+----+----+----+----+----+----+----+\n";
    std::cout << "       ";
    for (int i = 0; i <= 4; ++i) std::cout << std::setw(2) << i <<"   ";
    std::cout <<"\n";
    std::cout << "       ---> DIEM P1: " << std::setw(2) << scoreP1 << " ---> \n\n";
    std::cout.fill(fill_char);
}

bool board::is_game_over() {
    if (sq[5].get_count_chess() == 0 && sq[11].get_count_chess() == 0) return true;
    return false;
}

void board::move(int start_idx, int direction) {
    int sum_score = 0;
    int idx = start_idx;
    int cnt = sq[idx].choose_all_chess();

    while (true) {

        while (cnt > 0) {
            idx = (idx + direction + 12) % 12;
            sq[idx].add_chess();
            cnt--;
        }

        int next_idx = (idx + direction + 12) % 12;

        if (next_idx == 5 || next_idx == 11) {
            break;
        }
        else if (sq[next_idx].get_count_chess() > 0) {
            cnt = sq[next_idx].choose_all_chess();
            idx = next_idx;
        }
        else {
            int next_next_idx = (next_idx + direction + 12) % 12;

            if (sq[next_next_idx].get_count_chess() > 0) {
                int empty_idx = next_idx;
                int eat_idx = next_next_idx;

                while (sq[empty_idx].get_count_chess() == 0 && sq[eat_idx].get_count_chess() > 0) {
                    int eaten = sq[eat_idx].choose_all_chess();
                    sum_score += eaten;

                    death_count[eat_idx]++;

                    empty_idx = (eat_idx + direction + 12) % 12;
                    eat_idx = (empty_idx + direction + 12) % 12;
                }
                break;
            }
            else {
                break;
            }
        }
    }

    if (turn == 1) {
        scoreP1 += sum_score;
    } else {
        scoreP2 += sum_score;
    }
}

void board::update_board() {
    int s = (turn == 1)? 0 :  6;
    int e = (turn == 1)? 4 : 10;
    if (null_chess() == 0) {
        for (int i = s; i <= e; ++i) sq[i].add_chess();
        if (turn == 1) scoreP1 -=5;
        else scoreP2 -= 5;
    }
}

int board::evaluate_state(int w_score, int w_board, int w_starve) {
    int heuristic_score = 0;
    int score_diff = scoreP2 - scoreP1;
    heuristic_score += (score_diff * w_score);

    int p2_stones = 0;
    for (int i = 6; i <= 10; ++i) {
        p2_stones += sq[i].get_count_chess();
    }

    int p1_stones = 0;
    for (int i = 0; i <= 4; ++i) {
        p1_stones += sq[i].get_count_chess();
    }

    int board_diff = p2_stones - p1_stones;
    heuristic_score += (board_diff * w_board);

    bool is_game_active = (sq[5].get_count_chess() > 0 || sq[11].get_count_chess() > 0);

    if (is_game_active) {
        if (p2_stones == 0) {
            heuristic_score -= w_starve;
        }
        if (p1_stones == 0) {
            heuristic_score += w_starve;
        }
    }
    return heuristic_score;
}