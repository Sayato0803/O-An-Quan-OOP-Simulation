// 

#include <iostream>
#include <cstdlib> // Thư viện cho rand() và srand()
#include <thread> // Display on CMD
using namespace std;

class Board{
    public:
    int a[12];
    Board(){
        for (int i = 0 ; i < 12 ; i++){
            if (i == 0){
            this->setValue(i,10);
            continue;
        }
        if (i == 6){
            this->setValue(i,10);
            continue;
        }
            this->setValue(i,5);
        }
    }
    void setValue(int vitri, int da){
        if(vitri >= 0 && vitri < 12 && da >= 0){
            a[vitri] = da;
        }
        else{
            cout<<"\n\nERROR!!!!";
            return;
        }
    }
    int getValue(int vitri){
        if(vitri >= 0 && vitri < 12){
            return a[vitri];
        }
        return -1;
    }
    void printBoard(){
        for(int i = 0 ; i < 12 ; i++){
            if (i == 7) cout<<"\n  ";
            cout<<a[i]<<" ";
        }
    }
};

void veBanCo(Board& b, int p1Score, int p2Score) {
    system("cls"); // XÓA MÀN HÌNH CŨ
    cout << "========================================\n";
    cout << "        TRO CHOI O AN QUAN (Sim)        \n";
    cout << "========================================\n\n";

    cout << "      ";
    for (int i = 11; i >= 7; i--) printf("[%2d] ", b.a[i]);
    cout << "\n\n";

    // 2 Ô quan
    printf(" (%2d)                     (%2d)\n\n", b.a[0], b.a[6]);

    // Hàng dưới của P1 (ô 1 đến ô 5)
    cout << "      ";
    for (int i = 1; i <= 5; i++) printf("[%2d] ", b.a[i]);
    
    cout << "\n\n========================================\n";
    cout << " SCORE:  P1: " << p1Score << "  |  P2: " << p2Score << "\n";
    cout << "========================================\n";
}

class Pit{
    public:
    enum Owner{None,P1,P2};
    Owner owner[12];
    Pit(){
        for(int i = 0 ; i < 12 ; i++){
            if(i>=1 && i<=5) owner[i] = P1;
            else if(i>=7 && i<=11) owner[i] = P2;
            else owner[i] = None;
        }
    }

    bool isLegal(int player, int pos, Board& b) {
        if (player == 1 && (pos < 1 || pos > 5)) return false;
        if (player == 2 && (pos < 7 || pos > 11)) return false;
        if (b.a[pos] == 0) return false;
        return true;
    }
};

class Play{
    public:
    int scoreP1 = 0;
    int OweP1 = 0;
    int scoreP2 = 0;
    int OweP2 = 0;

    void CheckandRefill(int player,Board& b){
        int Total = 0;
        int start = (player == 1) ? 1 : 7;
        int end = (player == 1) ? 5 : 11;

        for(int i = start ; i <= end ; i++) Total += b.a[i];
        
        if(Total == 0){
            cout << "--- San nha trong! Dang rai quan... ---\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            int needed = 5;
            if(player == 1){
                if(scoreP1 < needed){
                    OweP1 += (needed - scoreP1);
                    scoreP1 = 0;
                }else{
                    scoreP1 -= needed;
                }
            }else {
                if (scoreP2 < needed) {
                    OweP2 += (needed - scoreP2); // Mượn của P1
                    scoreP2 = 0;
                } else scoreP2 -= needed;
            }
            for (int i = start; i <= end; i++) b.a[i] = 1;
        }
    }

    void Move(int player, int vitri, Board& b, Pit& p){
        int pos = vitri;
        int stones = b.a[pos];
        b.a[pos] = 0;
        while(1){
            while(stones > 0){
                pos = (pos + 1)%12;
                b.a[pos]++;
                stones--;
                veBanCo(b,scoreP1,scoreP2);
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }

            int nextPos = (pos+1) % 12;
            if(b.a[nextPos] > 0 && nextPos != 0 && nextPos != 6){
                stones = b.a[nextPos];
                b.a[nextPos] = 0;
                pos = nextPos;
                cout << "-> Tiep tuc boc quan tai o " << pos << "...\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            else if(b.a[nextPos] == 0 && b.a[(nextPos+1)%12] > 0){
                int vitriAN = (nextPos+1)%12;
                int scores = b.a[vitriAN];
                b.a[vitriAN] = 0;
                if(player == 1) scoreP1 += scores;
                else scoreP2 += scores;
                cout << "!!! DA AN DUOC " << scores << " DIEM !!!\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                break;
            }
            else{
                break;
            }
        }
    }
};


int main(){
    Board myData;
    Play control;
    Pit Rules;
    
    int CurrentPlayer = 1;
    while(1){
        control.CheckandRefill(CurrentPlayer,myData);
        veBanCo(myData,control.scoreP1,control.scoreP2);

        int pos;
        cout<<"DIEN VI TRI: ";
        cin>>pos;

        if(Rules.isLegal(CurrentPlayer,pos,myData)){
            control.Move(CurrentPlayer,pos,myData,Rules);
            CurrentPlayer = (CurrentPlayer == 1) ? 2 : 1;
        }else{
            cout<<"PICK LAI DI KU: ";
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
        }

        if (myData.a[0] == 0 && myData.a[6] == 0) break;
    }

    cout << "\nTRO CHOI KET THUC!\n";
    return 0;
}
