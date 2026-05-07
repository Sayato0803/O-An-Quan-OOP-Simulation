#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <string>
#include <algorithm>
#include "board.h"
#include "ai.h"
#include <windows.h>

using namespace std;

struct OpeningResult {
    int start_idx;
    int direction;
    int wins;
    long long deaths_of_this_square;
    long long total_deaths_in_match;
};

long long death_count[12] = {0};

struct Weights {
    int score, board, starve;
};

Weights generate_random_weight() {
    return { rand() % 101, rand() % 101, rand() % 101 };
}

int duel(Weights w1, Weights w2, int num_games) {
    int w1_wins = 0;
    int depth = 4;
    for (int i = 1; i <= num_games; ++i) {
        board game;
        bool w1_is_P1 = (i % 2 != 0);
        int turn_count = 0;
        while (!game.is_game_over() && turn_count < 200) {
            turn_count++;
            game.update_board();
            int s = -1, d = 0;
            if (game.get_turn() == 1) {
                if (w1_is_P1) AI::find_best_move_P1(game, s, d, depth, w1.score, w1.board, w1.starve);
                else AI::find_best_move_P1(game, s, d, depth, w2.score, w2.board, w2.starve);
            } else {
                if (!w1_is_P1) AI::find_best_move_P2(game, s, d, depth, w1.score, w1.board, w1.starve);
                else AI::find_best_move_P2(game, s, d, depth, w2.score, w2.board, w2.starve);
            }
            if (s != -1) { game.move(s, d); game.change_turn(); } else break;
        }
        if (game.get_scoreP1() > game.get_scoreP2() && w1_is_P1) w1_wins++;
        else if (game.get_scoreP2() > game.get_scoreP1() && !w1_is_P1) w1_wins++;
    }
    return w1_wins;
}

int main() {
    srand(time(0));
    int choice;
    cout << "CHON CHE DO DAU (1: NGUOI VS BOT | 2: NGUOI VS NGUOI): ";
    cin >> choice;

    if (choice == -1) {
        int cycle_count = 0;
        while (true) {
            cycle_count++;
            int score_bins[5] = {0}, board_bins[5] = {0}, starve_bins[5] = {0};
            int total_matches = 100; // so van dau
            int winners_count = 0;

            for (int i = 0; i < total_matches; i++) {
                Weights w1 = generate_random_weight();
                Weights w2 = generate_random_weight();
                int w1_v_w2 = duel(w1, w2, 10);

                Weights winner = (w1_v_w2 >= 5) ? w1 : w2;
                winners_count++;
                score_bins[min(winner.score / 20, 4)]++;
                board_bins[min(winner.board / 20, 4)]++;
                starve_bins[min(winner.starve / 20, 4)]++;
            }
            string labels[5] = {"0-20", "21-40", "41-60", "61-80", "81-100"};
            int max_s = 0, max_b = 0, max_st = 0;
            for(int i=1; i<5; i++) {
                if(score_bins[i] > score_bins[max_s]) max_s = i;
                if(board_bins[i] > board_bins[max_b]) max_b = i;
                if(starve_bins[i] > starve_bins[max_st]) max_st = i;
            }
            string total_result = "{[" + labels[max_s] + "], [" + labels[max_b] + "], [" + labels[max_st] + "]}";

            ofstream file_weights("BaoCao_TrongSo_TiLeThang.csv", ios::app);
            if (file_weights.is_open()) {
                file_weights.seekp(0, ios::end);
                if (file_weights.tellp() == 0) {
                    file_weights << "Khoang,Ti le w_score,Ti le w_board,Ti le w_starve,Total\n";
                }
                for (int i = 0; i < 5; i++) {
                    file_weights << labels[i] << ","
                                 << (float)score_bins[i]*100/winners_count << "%,"
                                 << (float)board_bins[i]*100/winners_count << "%,"
                                 << (float)starve_bins[i]*100/winners_count << "%,";
                    if (i == 0) file_weights << total_result;
                    file_weights << "\n";
                }
                file_weights << ",,,,\n";
                file_weights.close();
                cout << "-> Da cap nhat Excel: " << total_result << endl;
            } else {
                cout << "![LOI] Khong the mo file Excel!" << endl;
            }
        }
    }
    else if (choice == -2) {
        int b_score = 90, b_board = 10, b_starve = 15;
        int total_test_per_move = 500;
        int total_cycles = 100000;

        for (int cycle = 1; cycle <= total_cycles; ++cycle) {
            vector<OpeningResult> opening_stats;

            for (int start_idx = 0; start_idx <= 4; ++start_idx) {
                for (int dir : {-1, 1}) {
                    int wins = 0;
                    for(int i = 0; i < 12; ++i) death_count[i] = 0;

                    for (int t = 0; t < total_test_per_move; ++t) {
                        board game;
                        game.move(start_idx, dir);
                        game.change_turn();
                        int turn_count = 0;

                        while (!game.is_game_over() && turn_count < 200) {
                            turn_count++;
                            game.update_board();
                            int s = -1, d = 0;
                            if (game.get_turn() == 1) {
                                AI::find_best_move_P1(game, s, d, 4, b_score, b_board, b_starve);
                            } else {
                                if (rand() % 100 < 15) {
                                    d = (rand() % 2 == 0) ? 1 : -1;
                                    do { s = 6 + (rand() % 5); } while (game.check_square(s) == 0);
                                } else {
                                    AI::find_best_move_P2(game, s, d, 4, b_score, b_board, b_starve);
                                }
                            }
                            if (s != -1) { game.move(s, d); game.change_turn(); } else break;
                        }
                        if (game.get_scoreP1() > game.get_scoreP2()) wins++;
                    }

                    long long current_scenario_total_deaths = 0;
                    for(int i = 0; i < 12; ++i) {
                        if(i != 5 && i != 11) current_scenario_total_deaths += death_count[i];
                    }
                    opening_stats.push_back({start_idx, dir, wins, death_count[start_idx], current_scenario_total_deaths});
                }
            }

            time_t now = time(0);
            tm *ltm = localtime(&now);
            char time_str[20];
            strftime(time_str, sizeof(time_str), "%H:%M:%S %d/%m", ltm);

            bool is_new_file = true;
            ifstream check_file("BaoCao_KhaiCuoc_TuHuyet.csv");
            if (check_file.is_open()) {
                check_file.seekg(0, ios::end);
                if (check_file.tellg() > 0) is_new_file = false;
                check_file.close();
            }

            ofstream file_excel("BaoCao_KhaiCuoc_TuHuyet.csv", ios::app);
            if (!file_excel.is_open()) {
                cout << "\n[LOI] FILE EXCEL DANG BI KHOA!" << endl;
                system("pause");
                cycle--;
                continue;
            }

            if (is_new_file) {
                file_excel << "\xEF\xBB\xBF";
                file_excel << "Thời gian chạy,Ô,Hướng đi,Tỉ lệ ô đó bị ăn (%),Tỉ lệ thắng khi chọn trái phải (%)\n";
            }

            for (auto &res : opening_stats) {
                string huong = (res.direction == 1) ? "Phải" : "Trái";
                double ti_le_thang = (res.wins * 100.0) / total_test_per_move;
                double ti_le_bi_an = 0;
                if (res.total_deaths_in_match > 0) {
                    ti_le_bi_an = (res.deaths_of_this_square * 100.0) / res.total_deaths_in_match;
                }
                file_excel << time_str << "," << res.start_idx << "," << huong << ","
                           << fixed << setprecision(2) << ti_le_bi_an << "%,"
                           << fixed << setprecision(2) << ti_le_thang << "%\n";
            }
            file_excel.close();
            cout << "-> Da ghi luu ket qua vao Excel!" << endl;
        }
    }
    else if (choice == 1) {
        int depth;
        cout << "DA CHON CHE DO NGUOI VA MAY \n";
        cout << "CHON DO KHO(DE: 1, TRUNG BINH: 3, KHO: 7 \n";
        while (cin >> depth) {
            if (depth == 1) { cout <<"DA CHON BOT DE \n"; break; }
            else if (depth == 3) { cout <<"DA CHON BOT TRUNG BINH \n"; break; }
            else if (depth == 7) { cout <<"DA CHON BOT KHO \n"; break; }
            else {
                cout <<"NHAP LAI: ";
            }
        }
        board game;
        int b_score = 90, b_board = 10, b_starve = 15;

        int first_turn;
        cout << "\nCHon luot(1 hoac 2):";
        cin >> first_turn;
        if (first_turn == 2) game.change_turn();

        while (!game.is_game_over()) {
            system("cls");
            game.update_board();
            game.print_board();
            int s = -1, d = 0;
            if (game.get_turn() == 1) {
                cout << "\n[LUOT CUA BAN - P1]" << endl;
                cout << "=> Chon o de boc (0-4): ";
                cin >> s;
                while (s < 0 || s > 4 || game.check_square(s) == 0) {
                    cout << "![Loi] O khong hop le hoac o dang het soi! Chon lai: ";
                    cin >> s;
                }
                cout << "=> Chon huong (1: Nguoc chieu kim dong ho, -1: Cung chieu kim dong ho): ";
                cin >> d;
                while (d != 1 && d != -1) {
                    cout << "![Loi] Huong chi duoc nhap 1 hoac -1! Chon lai: ";
                    cin >> d;
                }
            } else {
                cout << "\n[LUOT CUA MAY - P2] AI dang suy nghi..." << endl;

                AI::find_best_move_P2(game, s, d, depth, b_score, b_board, b_starve);

                string huong_chu = (d == 1) ? "Nguoc chieu kim dong ho" : "Cung chieu kim dong ho";
                cout << "=> MAY DA QUYET DINH BOC O: " << s << " | HUONG: " << huong_chu << endl;
                Sleep(2000);
            }

            if (s != -1) {
                game.move(s, d);
                game.change_turn();
            } else {
                break;
            }
        }
        system("cls");
        cout<<"\n";
        cout << "          TỔNG KẾT TRẬN ĐẤU             \n";
        game.update_board();
        game.print_board();

        cout << "\nDiem cua Ban (P1): " << game.get_scoreP1() << endl;
        cout << "Diem cua May (P2): " << game.get_scoreP2() << endl;

        if (game.get_scoreP1() > game.get_scoreP2()) {
            cout << "\nBAN DA THANG" << endl;
        } else if (game.get_scoreP1() < game.get_scoreP2()) {
            cout << "BAN DA THUA" << endl;
        } else {
            cout << "\nHOA" << endl;
        }
        system("pause");
    }
    else if (choice == 2) {
        cout << "DA CHON CHE DO NGUOI VS NGUOI" << endl;
        Sleep(2000);
        board game;
        while (!game.is_game_over()) {
            system("cls");
            cout << "\n";
            game.update_board();
            game.print_board();

            int s = -1, d = 0;

            if (game.get_turn() == 1) {
                cout << "\n[LUOT CUA NGUOI CHOI 1 - P1]" << endl;
                cout << "=> Chon o de boc (Tu 0 den 4): ";
                cin >> s;
                while (s < 0 || s > 4 || game.check_square(s) == 0) {
                    cout << "![Loi] O khong thuoc san nha hoac dang het soi! Chon lai: ";
                    cin >> s;
                }
            } else {
                cout << "\n[LUOT CUA NGUOI CHOI 2 - P2]" << endl;
                cout << "=> Chon o de boc (Tu 6 den 10): ";
                cin >> s;
                while (s < 6 || s > 10 || game.check_square(s) == 0) {
                    cout << "![Loi] O khong thuoc san nha hoac dang het soi! Chon lai: ";
                    cin >> s;
                }
            }

            cout << "=> Chon huong (1: Nguoc chieu kim dong ho, -1: Cung chieu kim dong ho): ";
            cin >> d;
            while (d != 1 && d != -1) {
                cout << "![Loi] Huong chi duoc nhap 1 hoac -1! Chon lai: ";
                cin >> d;
            }

            if (s != -1) {
                game.move(s, d);
                game.change_turn();
            } else {
                break;
            }
        }
    } else {
        cout << "Lua chon khong hop le. Chuong trinh ket thuc!" << endl;
    }
    system("pause");
    return 0;
}