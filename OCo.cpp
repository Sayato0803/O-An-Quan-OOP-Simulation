#include "OCo.h"

OCo::OCo(int SoQuanBanDau, bool CoPhaiOQuan) {
    this->SoQuan = SoQuanBanDau;
    this->LaOQuan = CoPhaiOQuan;
}

int OCo::LaySoQuan() const {
    return SoQuan;
}

bool OCo::KiemTraOQuan() const {
    return LaOQuan;
}

void OCo::ThemQuan(int sl) {
    SoQuan += sl;
}

int OCo::LayHetQuan() {
    int QuanBocDuoc = SoQuan;
    SoQuan = 0;
    return QuanBocDuoc;
}
