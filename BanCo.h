#pragma once
#include <vector>
#include "OCo.h"

class BanCo{
private:
    std::vector<OCo> DanhSachO;
public:
    BanCo(int QuanDanBanDau = 5, int QuanChinhBanDau = 10);
    int XemSoQuan(int viTri);
    void InBanCo();
    bool KetThuc();
    int DiQuan(int ViTriBatDau, int Huong);
    bool KiemTraRong(int NguoiChoi);
    void VaiQuan(int NguoiChoi);
};

