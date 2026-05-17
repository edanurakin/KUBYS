#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDate>
#include <QDebug>
#include "depo.h"
#include "varliklar.h"

Depo<std::string, Kitap> kitap_deposu;
Depo<int, OduncKaydi> odunc_deposu;

void istemciIsteginiIsle(QTcpSocket* socket) {
    QByteArray hamVeri = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(hamVeri);
    if (doc.isNull() || !doc.isObject()) {
        return;
    }

    QJsonObject istek = doc.object();
    QString tip = istek["tip"].toString();
    QJsonObject yanit;

    if (tip == "odunc_al") {
        int uyeNo = istek["uye_no"].toInt();
        std::string isbn = istek["isbn"].toString().toStdString();

        auto kitapKontrol = kitap_deposu.bul(isbn);
        if (kitapKontrol.has_value()) {
            Kitap guncelKitap = kitapKontrol.value();
            if (guncelKitap.kopya_sayisi > 0) {
                guncelKitap.kopya_sayisi--;
                kitap_deposu.ekle(isbn, guncelKitap);
                kitap_deposu.kaydet("kitaplar.dat");

                OduncKaydi yeniKayit;
                yeniKayit.kayit_id = QDate::currentDate().toString("hhmmss").toInt();
                yeniKayit.uye_no = uyeNo;
                yeniKayit.isbn = isbn;
                yeniKayit.odunc_tarihi = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
                yeniKayit.durum = OduncDurum::Oduncte;

                odunc_deposu.ekle(yeniKayit.kayit_id, yeniKayit);
                odunc_deposu.kaydet("oduncler.dat");

                yanit["durum"] = "basarili";
                yanit["mesaj"] = "Kitap odunc verildi";
                yanit["kayit_id"] = yeniKayit.kayit_id;
            } else {
                yanit["durum"] = "hata";
                yanit["mesaj"] = "Kutuphanede bu kitaptan kalmadi";
            }
        } else {
            yanit["durum"] = "hata";
            yanit["mesaj"] = "Kitap bulunamadi";
        }
    }

    QJsonDocument yanitDoc(yanit);
    socket->write(yanitDoc.toJson(QJsonDocument::Compact));
    socket->flush();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try {
        kitap_deposu.yukle("kitaplar.dat");
        odunc_deposu.yukle("oduncler.dat");
    } catch (...) {}

    QTcpServer server;
    if (!server.listen(QHostAddress::Any, 12345)) {
        return -1;
    }

    QObject::connect(&server, &QTcpServer::newConnection, [&]() {
        QTcpSocket* clientSocket = server.nextPendingConnection();
        QObject::connect(clientSocket, &QTcpSocket::readyRead, [clientSocket]() {
            istemciIsteginiIsle(clientSocket);
        });
        QObject::connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    });

    return a.exec();
}