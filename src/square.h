
#ifndef O_AN_QUAN_SQUARE_H
#define O_AN_QUAN_SQUARE_H

class square {
private:
    int count_chess;
    bool is_king;
public:

    square() {
        count_chess = 5;
        is_king = false;
    }

    void set_king() {
        count_chess = 10;
        is_king = true;
    }

    bool check_king() const {
        return this->is_king;
    }

    int get_count_chess() const {
        return this->count_chess;
    }

    void add_chess() {
        ++this->count_chess;
    }

    int choose_all_chess() {
        int tmp = this->count_chess;
        this->count_chess = 0;
        return tmp;
    }
};
#endif
