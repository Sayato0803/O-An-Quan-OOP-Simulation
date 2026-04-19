#include <iostream>
#include <cstdlib> // Thư viện cho rand() và srand()
#include <thread>
using namespace std;

//CHUA CO DI 2 CHIEU
//CHUA CO STOP

int board[12] = {10, 5, 5, 5, 5, 5, 10, 5, 5, 5, 5, 5}; 

void veBanCo() {
    system("cls"); // Xóa màn hình cũ
    cout << "========================================\n";
    cout << "        TRO CHOI O AN QUAN (Sim)        \n";
    cout << "========================================\n\n";

    // Vẽ hàng trên của P2 (từ ô 11 về ô 7)
    cout << "      ";
    for(int i = 11; i >= 7; i--) cout << "[" << board[i] << "] ";
    cout << endl;

    // Vẽ 2 ô quan ở 2 đầu
    cout << " (" << board[0] << ")                     (" << board[6] << ")\n";

    // Vẽ hàng dưới của P1 (từ ô 1 đến ô 5)
    cout << "      ";
    for(int i = 1; i <= 5; i++) cout << "[" << board[i] << "] ";
    cout << "\n\n";
    cout << "========================================\n";
}


class Board{
    public:
    int a[12];
    Board(){
        for (int i = 0 ; i < 12 ; i++){
            if (i == 0){
            this->setValue(i,1);
            continue;
        }
        if (i == 6){
            this->setValue(i,1);
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

class Pit{
    public:
    enum PitType{MANDARIN,CITIZEN};
    enum Owner{None,P1,P2};

    PitType type[12];
    Owner owner[12];
    
    Pit(){
        for(int i = 0 ; i < 12 ; i++){
            if(i==0 || i==6) type[i] = MANDARIN;
            else type[i] = CITIZEN;

            if(i>=1 && i<=5) owner[i] = P1;
            else if(i>=7 && i<=11) owner[i] = P2;
            else owner[i] = None;
        }
    }

    bool isLegalMove(int PId, int vitri, Board& b){
        if(PId == 1 && owner[vitri] != P1) return false;
        if(PId == 2 && owner[vitri] != P2) return false;
        if(b.a[vitri] == 0) return false;
        return true;
    }

    bool CheckType(int vitri){
        if (type[vitri] == MANDARIN) return 0;
        else return 1;
    }

    int isOwe(int player,Board b){
        int total = 0;
        if(player == 1){
            for(int i = 1 ; i< 6 ; i++){
                total += b.getValue(i);
            }
            if(total == 0) return -5;
        }

        if(player == 2){
            for(int i = 7 ; i < 12 ; i++){
                total += b.getValue(i);
            }
            if(total == 0) return -5;
        }
        return 0;
    }

};

class Play{
    public:
    int scoreP1 = 0;
    int OweP1 = 0;
    int scoreP2 = 0;
    int OweP2 = 0;
    int dem = 0;

    void Vay_Rai(int player,Board& b){
        int CPoints = (player == 1) ? scoreP1 : scoreP2;
        int Owe = 0;
        while(CPoints < 0){
            Owe++;
            CPoints++;
        }
        (player == 1) ? OweP1 += Owe, scoreP1 = 0, scoreP2 -= Owe : OweP2 += Owe, scoreP2 = 0, scoreP1 -= Owe;
        if(player == 1){
            for(int i = 1 ; i < 6 ; i++){
                b.setValue(i,1);
            }
            return;
        }else{
            for(int i = 7 ; i < 11 ; i++){
                b.setValue(i,1);
            }
            return;
        }
    }

    void PayOwe(int player,Board b, Pit& rules){
        int Owe = rules.isOwe(player,b);
        (player == 1) ? scoreP1 += Owe : scoreP2 += Owe;
        if(scoreP1 < 0 || scoreP2 < 0) return Vay_Rai(player,b);

    }

    void SCORE(int player,int vitri,Board& b){
        int score = b.getValue(vitri+1);
        b.setValue(vitri+1,0);
        if(player == 1) scoreP1 += score;
        else scoreP2 += score;
    }

    bool Move(int player, int vitri, Board& b, Pit& p){
        //Check
        if(p.CheckType(vitri) == 0) return false;
        if(!(p.isLegalMove(player,vitri,b))) return false;
        //Move
        int Pos= vitri;
        while(true){
            int stones = b.getValue(Pos);
            if(stones == 0) break;
            b.setValue(Pos,0);
            while(stones>0){
                Pos = (Pos+1)%12;
                b.setValue(Pos,b.getValue(Pos)+1);
                stones--;
            }
            int nextPos = (Pos+1)%12;
            if(b.getValue(nextPos) > 0 && nextPos != 0 && nextPos != 6){
                Pos = nextPos;
                if(p.CheckType(Pos) == 0) return true;
                else if(b.getValue(Pos) == 0 && b.getValue(Pos+1) == 0){
                    return true;
                }
                else{
                    int ComboCdt = Pos+dem;
                    while(b.getValue(ComboCdt) == 0 && b.getValue(ComboCdt+1) > 0){
                        SCORE(player,ComboCdt,b);
                        dem++;
                    }
                }
            }else{
                break;
            }
        }
        return true;
    }

    void FinalPoints(Board& b){
        int Points1 = 0;
        int Points2 = 0;
        for(int i = 1 ; i < 6 ; i++){
            Points1 += b.getValue(i);
            b.setValue(i,0);
        }
        for(int i = 7 ; i < 11 ; i++){
            Points2 += b.getValue(i);
            b.setValue(i,0);
        }
        scoreP1 += Points1 - OweP1;
        scoreP2 += Points2 - OweP2;
    }

};


int main(){
    Board myData;
    Play control;
    Pit Rules;
    
    int CurrentPlayer = 1;
    bool Run = true;
    while(Run){
        veBanCo();
        myData.printBoard();
        cout<<"Luot cua Player "<<CurrentPlayer;
        if(Rules.isOwe(CurrentPlayer,myData) < 0){
            control.PayOwe(CurrentPlayer,myData,Rules);
        }
        int vitri;
        cout<<"\nPick a slot to play: ";
        cin>>vitri;
        bool thanhcong = control.Move(CurrentPlayer,vitri,myData,Rules);
        if(thanhcong){
            cout<<"DI CHUYEN THANH CONG!"<<endl;
            CurrentPlayer = (CurrentPlayer == 1) ? 2 : 1;
        }else{
            cout<<"PICK LAI DI KU"<<endl;
        }

        if (myData.getValue(0) == 0 && myData.getValue(6) == 0) {
            Run = false;
            cout << "TRO CHOI KET THUC!";
            control.FinalPoints(myData);
            cout<<"\nDiem so sau cung: "<<endl;
            cout<<"Diem cua Player1: "<<control.scoreP1<<"\nDiem cua Player2: "<<control.scoreP2;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
