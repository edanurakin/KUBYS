#ifndef DEPO_H
#define DEPO_H

#include <fstream>
#include <map>
#include <vector>
#include <functional>
#include <optional>
#include <type_traits>
#include "varliklar.h"

template <typename K, typename V>
class Depo {
private:
    std::map<K, V> veriler;

public:
    void ekle(K anahtar, V deger) {
        veriler[anahtar] = deger;
    }

    std::optional<V> bul(K anahtar) {
        if (veriler.contains(anahtar)) return veriler.at(anahtar);
        return std::nullopt;
    }

    bool sil(K anahtar) {
        return veriler.erase(anahtar) > 0;
    }

    const std::map<K, V>& tumunu_al() const {
        return veriler;
    }

    std::size_t boyut() const {
        return veriler.size();
    }

    void temizle() {
        veriler.clear();
    }

    std::vector<V> filtrele(std::function<bool(const V&)> kosul) const {
        std::vector<V> sonuc;
        for (const auto& [anahtar, deger] : veriler) {
            if (kosul(deger)) {
                sonuc.push_back(deger);
            }
        }
        return sonuc;
    }

    void kaydet(const std::string& dosya_yolu) const {
        std::ofstream dosya(dosya_yolu);
        if (!dosya) return;

        for (const auto& [anahtar, deger] : veriler) {
            if constexpr (std::is_same_v<V, Kitap>) {
                dosya << deger.isbn << "\n" << deger.baslik << "\n" << deger.yazar << "\n"
                      << deger.yayin_yili << "\n" << deger.kategori << "\n" << deger.kopya_sayisi << "\n";
            } else if constexpr (std::is_same_v<V, Uye>) {
                dosya << deger.uye_no << "\n" << deger.isim << "\n" << deger.soyisim << "\n"
                      << deger.telefon << "\n" << deger.kayit_tarihi << "\n";
            } else if constexpr (std::is_same_v<V, OduncKaydi>) {
                dosya << deger.kayit_id << "\n" << deger.uye_no << "\n" << deger.isbn << "\n"
                      << deger.odunc_tarihi << "\n"
                      << (deger.iade_tarihi.has_value() ? deger.iade_tarihi.value() : "NULL") << "\n"
                      << static_cast<int>(deger.durum) << "\n";
            }
        }
        dosya.close();
    }

    void yukle(const std::string& dosya_yolu) {
        std::ifstream dosya(dosya_yolu);
        if (!dosya) return;

        veriler.clear();

        if constexpr (std::is_same_v<V, Kitap>) {
            Kitap k;
            while (std::getline(dosya, k.isbn) && std::getline(dosya, k.baslik) && std::getline(dosya, k.yazar) &&
                   (dosya >> k.yayin_yili) && dosya.ignore() && std::getline(dosya, k.kategori) &&
                   (dosya >> k.kopya_sayisi) && dosya.ignore()) {
                veriler[k.isbn] = k;
            }
        } else if constexpr (std::is_same_v<V, Uye>) {
            Uye u;
            while ((dosya >> u.uye_no) && dosya.ignore() && std::getline(dosya, u.isim) &&
                   std::getline(dosya, u.soyisim) && std::getline(dosya, u.telefon) &&
                   std::getline(dosya, u.kayit_tarihi)) {
                veriler[u.uye_no] = u;
            }
        } else if constexpr (std::is_same_v<V, OduncKaydi>) {
            OduncKaydi ok;
            std::string iadeStr;
            int durumInt;
            while ((dosya >> ok.kayit_id >> ok.uye_no) && dosya.ignore() && std::getline(dosya, ok.isbn) &&
                   std::getline(dosya, ok.odunc_tarihi) && std::getline(dosya, iadeStr) &&
                   (dosya >> durumInt) && dosya.ignore()) {
                if (iadeStr == "NULL") ok.iade_tarihi = std::nullopt;
                else ok.iade_tarihi = iadeStr;
                ok.durum = static_cast<OduncDurum>(durumInt);
                veriler[ok.kayit_id] = ok;
            }
        }
        dosya.close();
    }
};

#endif