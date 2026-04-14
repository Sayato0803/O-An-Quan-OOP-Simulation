#include <iostream>
#include "BanCo.h"
#include <windows.h>
using namespace std;
void FixConsoleWindow() {
    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_EXSTYLE);
    style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
    SetWindowLong(consoleWindow, GWL_EXSTYLE, style);
}

int main() {
    BanCo banCo;
    int diemP1 = 0; // Điểm người chơi 1 (Hàng dưới)
    int diemP2 = 0; // Điểm người chơi 2 (Hàng trên)
    int luotChoi = 1; // 1 là lượt P1, 2 là lượt P2
    FixConsoleWindow();
    cout << "========================================\n";
    cout << "    CHAO MUNG DEN VOI O AN QUAN         \n";
    cout << "========================================\n";
    while (!banCo.KetThuc()) {
        banCo.InBanCo();
        cout << ">> TONG DIEM - P1: " << diemP1 << " | P2: " << diemP2 << " <<\n";
        cout << "----------------------------------------\n";

        int viTri, huong;

        if (luotChoi == 1) {
            if (banCo.KiemTraRong(1)) {
                diemP1 -= 5;
                banCo.VaiQuan(1);
                banCo.InBanCo();
            }
            cout << "[LUOT NGUOI CHOI 1] - Chon o (0 -> 4): ";
            cin >> viTri;
            while (viTri < 0 || viTri > 4 || banCo.XemSoQuan(viTri) == 0) {
                cout << "Vi tri cua P1 chi tu 0 den 4 hoac O ban chon dang khong co quan. Chon lai: ";
                cin >> viTri;
            }
        } else {
            if (banCo.KiemTraRong(2)) {
                diemP2 -= 5;
                banCo.VaiQuan(2);
                banCo.InBanCo();
            }
            cout << "[LUOT NGUOI CHOI 2] - Chon o (6 -> 10): ";
            cin >> viTri;
            while (viTri < 6 || viTri > 10 || banCo.XemSoQuan(viTri) == 0) {
                cout << "Vi tri cua P2 chi tu 6 den 10 hoac o ban chon da het quan. Chon lai: ";
                cin >> viTri;
            }
        }

        cout << "Chon huong di (1: Phai, -1: Trai): ";
        cin >> huong;
        while (huong != 1 && huong != -1) {
            cout << "Chi duoc nhap 1 hoac -1. Chon lai: ";
            cin >> huong;
        }

        cout << "\n----------------------------------------\n";
        int diemThuDuoc = banCo.DiQuan(viTri, huong);
        if (diemThuDuoc != -1) {
            if (luotChoi == 1) {
                diemP1 += diemThuDuoc;
                luotChoi = 2;
            } else {
                diemP2 += diemThuDuoc;
                luotChoi = 1;
            }
        } else {
            cout << "Nuoc di loi, ban van duoc giu luot.\n";
        }
    }
    cout << "\n========================================\n";
    cout << "           TRO CHOI KET THUC!           \n";
    cout << "========================================\n";
    banCo.InBanCo();
    cout << "DIEM CHUNG CUOC:\n";
    cout << "- Nguoi choi 1: " << diemP1 << " diem.\n";
    cout << "- Nguoi choi 2: " << diemP2 << " diem.\n";
    if (diemP1 > diemP2) cout << "=> NGUOI CHOI 1 CHIEN THANG!\n";
    else if (diemP2 > diemP1) cout << "=> NGUOI CHOI 2 CHIEN THANG!\n";
    else cout << "=> TRAN DAU HOA NHAU!\n";

    return 0;
}
