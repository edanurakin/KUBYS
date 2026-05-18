#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDate>
#include <QTime> // Saat dönüşümü için eklendi
#include <QDebug>
#include "depo.h"
#include "varliklar.h"

Depo<std::string, Kitap> kitap_deposu;
Depo<int, OduncKaydi> odunc_deposu;

void istemciIsteginiIsle(QTcpSocket* socket) {
    QByteArray hamVeri = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(hamVeri);
    if (doc.isNull() || !doc.isObject()) return;

    QJsonObject istek = doc.object();
    QString tip = istek["tip"].toString();
    QJsonObject yanit;

    // ÖDÜNÇ ALMA MODÜLÜ
    if (tip == "odunc_al") {
        int uyeNo = istek["uye_no"].toInt();
        std::string isbn = istek["isbn"].toString().toStdString();

        try {
            kitap_deposu.yukle("kitaplar.dat");
            odunc_deposu.yukle("oduncler.dat");

            auto kitapKontrol = kitap_deposu.bul(isbn);
            if (kitapKontrol.has_value()) {
                Kitap guncelKitap = kitapKontrol.value();
                if (guncelKitap.kopya_sayisi > 0) {
                    guncelKitap.kopya_sayisi--;
                    kitap_deposu.ekle(isbn, guncelKitap);
                    kitap_deposu.kaydet("kitaplar.dat");

                    OduncKaydi yeniKayit;
                    // HATA DÜZELTİLDİ: QDate yerine QTime kullanılarak benzersiz sayı üretildi
                    yeniKayit.kayit_id = QTime::currentTime().toString("hhmmss").toInt();
                    yeniKayit.uye_no = uyeNo;
                    yeniKayit.isbn = isbn;
                    yeniKayit.odunc_tarihi = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
                    yeniKayit.durum = OduncDurum::Oduncte; // Enum kontrolü (Gerekirse OduncDurum::Oduncte yapabilirsin)

                    odunc_deposu.ekle(yeniKayit.kayit_id, yeniKayit);
                    odunc_deposu.kaydet("oduncler.dat");

                    yanit["durum"] = "basarili";
                    yanit["kayit_id"] = yeniKayit.kayit_id;
                } else {
                    yanit["durum"] = "hata";
                    yanit["mesaj"] = "Stokta bu kitaptan kalmadi!";
                }
            } else {
                yanit["durum"] = "hata";
                yanit["mesaj"] = "Kitap bulunamadi!";
            }
        } catch (...) {
            yanit["durum"] = "hata";
            yanit["mesaj"] = "Veritabani erisim hatasi!";
        }
    }
    // İADE ETME MODÜLÜ
    else if (tip == "iade_et") {
        int kayitId = istek["kayit_id"].toInt();

        try {
            kitap_deposu.yukle("kitaplar.dat");
            odunc_deposu.yukle("oduncler.dat");

            auto oduncKontrol = odunc_deposu.bul(kayitId);
            if (oduncKontrol.has_value()) {
                OduncKaydi kayit = oduncKontrol.value();
                if (kayit.durum == OduncDurum::Oduncte) {
                    kayit.durum = OduncDurum::IadeEdildi; // Enum kontrolü
                    odunc_deposu.ekle(kayitId, kayit);
                    odunc_deposu.kaydet("oduncler.dat");

                    auto kitapKontrol = kitap_deposu.bul(kayit.isbn);
                    if (kitapKontrol.has_value()) {
                        Kitap k = kitapKontrol.value();
                        k.kopya_sayisi++;
                        kitap_deposu.ekle(kayit.isbn, k);
                        kitap_deposu.kaydet("kitaplar.dat");
                    }

                    yanit["durum"] = "basarili";
                } else {
                    yanit["durum"] = "hata";
                    yanit["mesaj"] = "Bu kitap zaten iade edilmis!";
                }
            } else {
                yanit["durum"] = "hata";
                yanit["mesaj"] = "Odunc kaydi bulunamadi!";
            }
        } catch (...) {
            yanit["durum"] = "hata";
            yanit["mesaj"] = "Veritabani erisim hatasi!";
        }
    }

    // Yanıtı gönder ve bağlantıyı güvenli bir şekilde kapat/kes
    QJsonDocument yanitDoc(yanit);
    socket->write(yanitDoc.toJson(QJsonDocument::Compact));
    socket->flush();
    socket->disconnectFromHost();
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QTcpServer server;
    if (!server.listen(QHostAddress::Any, 12345)) {
        qDebug() << "Server baslatilamadi!";
        return -1;
    }
    qDebug() << "Sunucu 12345 portunda dinlemede...";

    QObject::connect(&server, &QTcpServer::newConnection, [&]() {
        QTcpSocket* clientSocket = server.nextPendingConnection();
        QObject::connect(clientSocket, &QTcpSocket::readyRead, [clientSocket]() {
            istemciIsteginiIsle(clientSocket);
        });
        // HATA DÜZELTİLDİ: İşlem bittiğinde soket bellekten temizlenir
        QObject::connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    });
    return a.exec();
}