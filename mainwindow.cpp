
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kitapekledialog.h"
#include "uyeekledialog.h"
#include "oduncverdialog.h"
#include <QMessageBox>
#include <fstream>
#include <chrono>
#include <iomanip>

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
        logYaz("YENI UYE EKLENDI - Uye No: " + std::to_string(yeniUye.uye_no) + ", Isim: " + yeniUye.isim + " " + yeniUye.soyisim);
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
        logYaz("YENI KITAP EKLENDI - ISBN: " + yeniKitap.isbn + ", Baslik: " + yeniKitap.baslik);
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
        logYaz("KITAP ODUNC VERILDI - Kayit ID: " + std::to_string(yeniKayit.kayit_id) + ", Uye No: " + std::to_string(yeniKayit.uye_no) + ", ISBN: " + yeniKayit.isbn);
    }
}
void MainWindow::on_btnIadeAl_clicked()
{
    QModelIndex index = ui->viewEmanetler->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen iade alınacak ödünç kaydını tablodan seçiniz!");
        return;
    }

    int secilenSatir = index.row();

    auto tumKayitlarMap = odunc_deposu.tumunu_al();
    std::vector<OduncKaydi> liste;
    for(auto const& [id, veri] : tumKayitlarMap) {
        liste.push_back(veri);
    }

    OduncKaydi secilenKayit = liste[secilenSatir];

    auto kitapKontrol = kitap_deposu.bul(secilenKayit.isbn);
    if (kitapKontrol.has_value()) {
        Kitap guncellenecekKitap = kitapKontrol.value();
        guncellenecekKitap.kopya_sayisi++;
        kitap_deposu.ekle(guncellenecekKitap.isbn, guncellenecekKitap);
    }

    odunc_deposu.sil(secilenKayit.kayit_id);

    auto guncelKayitlarMap = odunc_deposu.tumunu_al();
    std::vector<OduncKaydi> guncelListe;
    for(auto const& [id, veri] : guncelKayitlarMap) {
        guncelListe.push_back(veri);
    }
    oduncModel->setKayitlar(guncelListe);

    auto guncelKitaplarMap = kitap_deposu.tumunu_al();
    std::vector<Kitap> guncelKitapListesi;
    for(auto const& [key, val] : guncelKitaplarMap) {
        guncelKitapListesi.push_back(val);
    }
    kitapModel->setKitaplar(guncelKitapListesi);

    QMessageBox::information(this, "Başarılı", "Kitap başarıyla iade alındı ve stok güncellendi!");
    logYaz("KITAP IADE ALINDI - Kayit ID: " + std::to_string(secilenKayit.kayit_id) + ", ISBN: " + secilenKayit.isbn);
}


void MainWindow::on_txtKitapAra_textChanged(const QString &arg1)
{
    std::string arananMetin = arg1.toLower().toStdString();
    auto tumKitaplarMap = kitap_deposu.tumunu_al();
    std::vector<Kitap> filtrelenmisListe;

    for(auto const& [key, val] : tumKitaplarMap) {
        std::string kitapAdi = val.baslik;
        std::string yazarAdi = val.yazar;

        std::transform(kitapAdi.begin(), kitapAdi.end(), kitapAdi.begin(), ::tolower);
        std::transform(yazarAdi.begin(), yazarAdi.end(), yazarAdi.begin(), ::tolower);

        if (kitapAdi.find(arananMetin) != std::string::npos ||
            yazarAdi.find(arananMetin) != std::string::npos)
        {
            filtrelenmisListe.push_back(val);
        }
    }

    kitapModel->setKitaplar(filtrelenmisListe);
}


void MainWindow::on_txtUyeAra_textChanged(const QString &arg1)
{
    std::string arananMetin = arg1.toLower().toStdString();
    auto tumUyelerMap = uye_deposu.tumunu_al();
    std::vector<Uye> filtrelenmisListe;

    for(auto const& [key, val] : tumUyelerMap) {
        std::string isim = val.isim;
        std::string soyisim = val.soyisim;

        std::transform(isim.begin(), isim.end(), isim.begin(), ::tolower);
        std::transform(soyisim.begin(), soyisim.end(), soyisim.begin(), ::tolower);

        if (isim.find(arananMetin) != std::string::npos ||
            soyisim.find(arananMetin) != std::string::npos)
        {
            filtrelenmisListe.push_back(val);
        }
    }

    uyeModel->setUyeler(filtrelenmisListe);
}

void MainWindow::logYaz(const std::string &mesaj)
{
    std::ofstream logDosyasi("islem_gunlugu.txt", std::ios::app);
    if (logDosyasi.is_open()) {
        auto simdi = std::chrono::system_clock::now();
        auto zaman_t = std::chrono::system_clock::to_time_t(simdi);
        std::time_t t = std::chrono::system_clock::to_time_t(simdi);

        logDosyasi << std::put_time(std::localtime(&t), "[%Y-%m-%d %H:%M:%S] ")
                   << mesaj << std::endl;
        logDosyasi.close();
    }
}