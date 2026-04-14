#pragma once
class OCo {
private:
    int SoQuan;
    bool LaOQuan;

public:
    OCo(int SoQuanBanDau, bool CoPhaiOQuan);

    int LaySoQuan() const;
    bool KiemTraOQuan() const;

    void ThemQuan(int sl);
    int LayHetQuan();
};
