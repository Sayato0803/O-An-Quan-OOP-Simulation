#include "BanCo.h"
#include <iostream>
#include <iomanip>

using namespace std;

BanCo::BanCo(int QuanDanBanDau, int QuanChinhBanDau) {
    //Nguoi choi: 0-4, Quan 1: 5
    //Bot: 6 - 10, Quan 2: 11
    for (int i = 0; i < 12; ++i) {
        if (i == 5 || i == 11) {
            DanhSachO.push_back(OCo(QuanChinhBanDau, true));
        } else DanhSachO.push_back(OCo(QuanDanBanDau, false));
    }
}
int BanCo::XemSoQuan(int viTri) {
    return DanhSachO[viTri].LaySoQuan();
}
bool BanCo::KiemTraRong(int NguoiChoi) {
    int bd = (NguoiChoi == 1)? 0 : 6;
    int kt = (NguoiChoi == 1)? 4 : 10;
    for (int i = bd; i <= kt; ++i) {
        if (DanhSachO[i].LaySoQuan() != 0) return false;
    }
    return true;
}

void BanCo::VaiQuan(int NguoiChoi) {
    int bd = (NguoiChoi == 1)? 0 : 6;
    int kt = (NguoiChoi == 1)? 4 : 10;
    for (int i = bd; i <= kt; ++i) {
        DanhSachO[i].ThemQuan(1);
    }
}
void BanCo::InBanCo() {
    cout << "\n================ BAN CO ================\n";
    cout << "      [P2]  10   9   8   7   6\n";
    cout << "        -----------------------\n";

    cout << "Quan11|   |";
    for (int i = 10; i >= 6; i--) cout << setw(3) << DanhSachO[i].LaySoQuan() << " ";
    cout << "|   |Quan5\n";

    cout << setw(5) << DanhSachO[11].LaySoQuan() << " |                       | " << setw(4) << DanhSachO[5].LaySoQuan() << "\n";

    cout << "      |   |";
    for (int i = 0; i <= 4; i++) cout << setw(3) << DanhSachO[i].LaySoQuan() << " ";
    cout << "|   |\n";

    cout << "        -----------------------\n";
    cout << "      [P1]   0   1   2   3   4\n";
    cout << "========================================\n\n";
}

bool BanCo::KetThuc() {
    return (DanhSachO[5].LaySoQuan() == 0 && DanhSachO[11].LaySoQuan() == 0);
}

int BanCo::DiQuan(int ViTriBatDau, int Huong) {
    if (DanhSachO[ViTriBatDau].LaySoQuan() == 0 || DanhSachO[ViTriBatDau].KiemTraOQuan()) {
        cout << "Nuoc di khong hop le, di lai \n";
        return -1;
    }

    int DiemThuHoach = 0;
    int QuanTrenTay = DanhSachO[ViTriBatDau].LayHetQuan();
    int ViTriHienTai = ViTriBatDau;

    while(1) {
        while(QuanTrenTay > 0) {
            ViTriHienTai = (ViTriHienTai + Huong + 12) % 12;
            DanhSachO[ViTriHienTai].ThemQuan(1);
            --QuanTrenTay;
        }
        int OTiepTheo = (ViTriHienTai + Huong + 12) % 12;
        if (DanhSachO[OTiepTheo].LaySoQuan() > 0) {
            if (DanhSachO[OTiepTheo].KiemTraOQuan()){ // Neu la O quan thi ko dc boc de rai
                break;
            } else { // rai, cap nhat vi tri tiep theo
            QuanTrenTay = DanhSachO[OTiepTheo].LayHetQuan();
            ViTriHienTai = OTiepTheo;
            }
        } else {
            int OTiepTheo2 = (OTiepTheo + Huong + 12) % 12; // check o cach 2 vi tri
            if (DanhSachO[OTiepTheo2].LaySoQuan() > 0) { // co dan thi hot, va check lien tuc o trong & o co dan xen ke
                int OTrong = OTiepTheo;
                int OBiAn = OTiepTheo2;
                while(DanhSachO[OTrong].LaySoQuan() == 0 && DanhSachO[OBiAn].LaySoQuan() > 0) {
                    int OAnDuoc = DanhSachO[OBiAn].LayHetQuan();
                    DiemThuHoach += OAnDuoc;
                    cout <<"An duoc" << OAnDuoc << "Quan tai O So" << OBiAn << "\n";
                    OTrong = (OBiAn + Huong + 12) % 12;
                    OBiAn = (OTrong + Huong + 12) % 12;
                }
                break;
            } else {
                break;
            }
        }
    }
    return DiemThuHoach;
}

