#ifndef KITAPYUKLEYICI_H
#define KITAPYUKLEYICI_H

#include <QObject>
#include <string>
#include <vector>
#include <atomic>
#include "varliklar.h"

class KitapYukleyici : public QObject
{
    Q_OBJECT
public:
    explicit KitapYukleyici(const std::string& dosyaYolu, QObject *parent = nullptr);

public slots:
    void calis();
    void iptalEt();

signals:
    void ilerlemeGuncellendi(int yuzde);
    void kitaplarHazir(const std::vector<Kitap>& yuklenenKitaplar);
    void tamamlandi();

private:
    std::string m_dosyaYolu;
    std::atomic<bool> m_iptalEdildi{false};
};

#endif // KITAPYUKLEYICI_H