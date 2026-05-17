#ifndef VARLIKLAR_H
#define VARLIKLAR_H

#include <string>
#include <optional>
#include <vector>

enum class OduncDurum { Oduncte, IadeEdildi, Gecikmis };
std::string durumYazisi(OduncDurum durum);

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
    OduncDurum durum;
};

struct Emanet {
    int emanet_no;
    int uye_no;
    std::string isbn;
    std::string alma_tarihi;
    std::string iade_tarihi;
    bool iade_edildi_mi;
};

std::string durumYazisi(OduncDurum durum);

#endif