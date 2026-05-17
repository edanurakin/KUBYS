
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kitapekledialog.h"
#include "uyeekledialog.h"
#include "oduncverdialog.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    kitapModel = new KitapModel(this);
    ui->viewKitaplar->setModel(kitapModel);
    uyeModel = new UyeModel(this);
    ui->viewUyeler->setModel(uyeModel);
    oduncModel = new OduncModel(this);
    ui->viewEmanetler->setModel(oduncModel);

    kitap_deposu.yukle("kitaplar.dat");
    uye_deposu.yukle("uyeler.dat");
    odunc_deposu.yukle("oduncler.dat");

    auto tumKitaplarMap = kitap_deposu.tumunu_al();
    std::vector<Kitap> kitapListesi;
    for(auto const& [key, val] : tumKitaplarMap) {
        kitapListesi.push_back(val);
    }
    kitapModel->setKitaplar(kitapListesi);

    auto tumUyelerMap = uye_deposu.tumunu_al();
    std::vector<Uye> uyeListesi;
    for(auto const& [key, val] : tumUyelerMap) {
        uyeListesi.push_back(val);
    }
    uyeModel->setUyeler(uyeListesi);

    auto tumKayitlarMap = odunc_deposu.tumunu_al();
    std::vector<OduncKaydi> liste;
    for(auto const& [id, veri] : tumKayitlarMap) {
        liste.push_back(veri);
    }
    oduncModel->setKayitlar(liste);
}

MainWindow::~MainWindow() {
    kitap_deposu.kaydet("kitaplar.dat");
    uye_deposu.kaydet("uyeler.dat");
    odunc_deposu.kaydet("oduncler.dat");
    delete ui;
}

void MainWindow::on_btnUyeEkle_clicked()
{
    UyeEkleDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        Uye yeniUye = dialog.getUye();

        uye_deposu.ekle(yeniUye.uye_no, yeniUye);

        auto tumUyelerMap = uye_deposu.tumunu_al();
        std::vector<Uye> uyeListesi;
        for(auto const& [key, val] : tumUyelerMap) {
            uyeListesi.push_back(val);
        }

        uyeModel->setUyeler(uyeListesi);
    }
}

void MainWindow::on_btnKitapEkle_2_clicked()
{
    KitapEkleDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Kitap yeniKitap = dialog.getKitap();

        kitap_deposu.ekle(yeniKitap.isbn, yeniKitap);

        auto tumKitaplarMap = kitap_deposu.tumunu_al();
        std::vector<Kitap> kitapListesi;
        for(auto const& [key, val] : tumKitaplarMap) {
            kitapListesi.push_back(val);
        }

        kitapModel->setKitaplar(kitapListesi);
    }
}

void MainWindow::on_btnOduncVer_clicked()
{
    OduncVerDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        OduncKaydi yeniKayit = dialog.getOduncKaydi();

        auto uyeKontrol = uye_deposu.bul(yeniKayit.uye_no);
        if (!uyeKontrol.has_value()) {
            QMessageBox::critical(this, "Hata", "Girilen uye numarasina ait bir uye bulunamadi!");
            return;
        }

        auto kitapKontrol = kitap_deposu.bul(yeniKayit.isbn);
        if (!kitapKontrol.has_value()) {
            QMessageBox::critical(this, "Hata", "Girilen ISBN numarasina ait bir kitap bulunamadi!");
            return;
        }

        Kitap guncellenecekKitap = kitapKontrol.value();
        if (guncellenecekKitap.kopya_sayisi <= 0) {
            QMessageBox::warning(this, "Hata", "Bu kitaptan kutuphanede kalmamistir!");
            return;
        }

        guncellenecekKitap.kopya_sayisi--;
        kitap_deposu.ekle(guncellenecekKitap.isbn, guncellenecekKitap);

        auto tumKitaplarMap = kitap_deposu.tumunu_al();
        std::vector<Kitap> kitapListesi;
        for(auto const& [key, val] : tumKitaplarMap) {
            kitapListesi.push_back(val);
        }
        kitapModel->setKitaplar(kitapListesi);

        odunc_deposu.ekle(yeniKayit.kayit_id, yeniKayit);

        auto tumKayitlarMap = odunc_deposu.tumunu_al();
        std::vector<OduncKaydi> liste;
        for(auto const& [id, veri] : tumKayitlarMap) {
            liste.push_back(veri);
        }
        oduncModel->setKayitlar(liste);
    }
}