#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "depo.h"
#include "varliklar.h"
#include "kitapmodel.h"
#include "uyemodel.h"
#include "oduncmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Listeleme Butonlarının Tetikleyicileri (Slots)
    void on_btnKitapListele_clicked();
    void on_btnUyeListele_clicked();
    void on_btnOduncListele_clicked();

    // İşlem ve Ekleme Butonlarının Tetikleyicileri (Slots)
    void on_btnKitapEkle_clicked();
    void on_btnUyeEkle_clicked();
    void on_btnOduncVer_clicked();
    void on_btnIadeAl_clicked();

    void on_btnTopluKitapEkle_clicked();

    void on_btnTopluIptal_clicked();

private:
    Ui::MainWindow *ui;

    // Şablon Sınıfı (Template) Kullanan Veri Depoları
    Depo<std::string, Kitap> kitap_deposu;
    Depo<int, Uye> uye_deposu;
    Depo<int, OduncKaydi> odunc_deposu;

    // Model/View Mimarisi İçin Model Nesneleri
    KitapModel *kitapModel;
    UyeModel *uyeModel;
    OduncModel *oduncModel;

    // Arayüze Durum Raporlamak İçin Yardımcı Fonksiyon
    void logYaz(const std::string& mesaj);
};

#endif // MAINWINDOW_H