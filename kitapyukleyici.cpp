#include "kitapyukleyici.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <thread>
#include <chrono>

KitapYukleyici::KitapYukleyici(const std::string& dosyaYolu, QObject *parent)
    : QObject{parent}, m_dosyaYolu(dosyaYolu)
{
}

void KitapYukleyici::iptalEt()
{
    m_iptalEdildi = true;
}

void KitapYukleyici::calis()
{
    std::vector<Kitap> yuklenenKitaplar;
    QFile dosya(QString::fromStdString(m_dosyaYolu));

    if (!dosya.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit tamamlandi();
        return;
    }

    QTextStream stream(&dosya);
    stream.setEncoding(QStringConverter::Utf8);

    QStringList satirlar;
    while (!stream.atEnd()) {
        QString satir = stream.readLine().trimmed();
        if (!satir.isEmpty()) {
            satirlar.append(satir);
        }
    }

    int toplamSatir = satirlar.size();
    int mevcutSatir = 0;

    for (const QString& satir : satirlar) {
        if (m_iptalEdildi) {
            break;
        }

        mevcutSatir++;
        QStringList parcalar = satir.split(',');

        if (parcalar.size() >= 6) {
            try {
                Kitap k;
                k.isbn = parcalar[0].trimmed().toStdString();
                k.baslik = parcalar[1].trimmed().toStdString();
                k.yazar = parcalar[2].trimmed().toStdString();
                k.yayin_yili = parcalar[3].trimmed().toInt();
                k.kategori = parcalar[4].trimmed().toStdString();
                k.kopya_sayisi = parcalar[5].trimmed().toInt();

                yuklenenKitaplar.push_back(k);
            } catch (...) {
                continue;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        if (toplamSatir > 0) {
            int yuzde = (mevcutSatir * 100) / toplamSatir;
            emit ilerlemeGuncellendi(yuzde);
        }
    }

    dosya.close();

    if (!m_iptalEdildi) {
        emit kitaplarHazir(yuklenenKitaplar);
    }

    emit tamamlandi();
}