#ifndef VARLIKLAR_H
#define VARLIKLAR_H

#include <string>
#include <optional>

enum class OduncDurum {
    Oduncte,
    IadeEdildi,
    Gecikmis
};

struct Kitap {
    std::string isbn;
    std::string baslik;
    std::string yazar;
    int yayin_yili{0};
    std::string kategori;
    int kopya_sayisi{0};
};

struct Uye {
    int uye_no;
    std::string isim;
    std::string soyisim;
    std::string telefon;
    std::string kayit_tarihi;
};

struct OduncKaydi {
    int kayit_id;
    int uye_no;
    std::string isbn;
    std::string odunc_tarihi;
    std::optional<std::string> iade_tarihi;
    OduncDurum durum{OduncDurum::Oduncte};
};

#endif