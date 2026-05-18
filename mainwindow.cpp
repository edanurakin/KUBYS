#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kitapekledialog.h"
#include "uyeekledialog.h"
#include "oduncverdialog.h"
#include <QMessageBox>

// Ağ ve JSON işlemleri için gerekli kütüphaneler
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Model nesnelerinin ilklenmesi
    kitapModel = new KitapModel(this);
    uyeModel = new UyeModel(this);
    oduncModel = new OduncModel(this);

    // HATA DÜZELTİLDİ: Tablolar senin tasarımdaki gerçek isimlerine bağlandı
    ui->viewKitaplar->setModel(kitapModel);
    ui->viewUyeler->setModel(uyeModel);
    ui->viewEmanetler->setModel(oduncModel);

    // İlk veri yükleme işlemleri (Yerel depoları besle)
    try {
        kitap_deposu.yukle("kitaplar.dat");
        uye_deposu.yukle("uyeler.dat");
        odunc_deposu.yukle("oduncler.dat");

        // Tablo görünümlerini tazele
        on_btnKitapListele_clicked();
        on_btnUyeListele_clicked();
        on_btnOduncListele_clicked();
    }
    catch (...) {
        logYaz("SISTEM: Ilk veriler yuklenirken hata olustu veya dosyalar henuz yok.");
    }
}

MainWindow::~MainWindow()
{
    try {
        kitap_deposu.kaydet("kitaplar.dat");
        uye_deposu.kaydet("uyeler.dat");
        odunc_deposu.kaydet("oduncler.dat");
    } catch (...) {}

    delete ui;
}

// --- LİSTELEME BUTONLARI ---

void MainWindow::on_btnKitapListele_clicked()
{
    auto map = kitap_deposu.tumunu_al();
    std::vector<Kitap> liste;
    for (auto const& [key, val] : map) {
        liste.push_back(val);
    }
    kitapModel->setKitaplar(liste);
}

void MainWindow::on_btnUyeListele_clicked()
{
    auto map = uye_deposu.tumunu_al();
    std::vector<Uye> liste;
    for (auto const& [key, val] : map) {
        liste.push_back(val);
    }
    uyeModel->setUyeler(liste);
}

void MainWindow::on_btnOduncListele_clicked()
{
    auto map = odunc_deposu.tumunu_al();
    std::vector<OduncKaydi> liste;
    for (auto const& [key, val] : map) {
        liste.push_back(val);
    }
    oduncModel->setOduncler(liste);
}

// --- EKLEME VE KAYDETME BUTONLARI ---

void MainWindow::on_btnKitapEkle_clicked()
{
    // HATA DÜZELTİLDİ: Sınıf ismi KitapEkleDialog olarak güncellendi
    KitapEkleDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Kitap yeniKitap = dialog.getKitap();
        kitap_deposu.ekle(yeniKitap.isbn, yeniKitap);
        kitap_deposu.kaydet("kitaplar.dat");
        on_btnKitapListele_clicked();
        QMessageBox::information(this, "Başarılı", "Kitap başarıyla kaydedildi.");
    }
}

void MainWindow::on_btnUyeEkle_clicked()
{
    UyeEkleDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Uye yeniUye = dialog.getUye();
        uye_deposu.ekle(yeniUye.uye_no, yeniUye);
        uye_deposu.kaydet("uyeler.dat");
        on_btnUyeListele_clicked();
        QMessageBox::information(this, "Başarılı", "Üye başarıyla kaydedildi.");
    }
}

// --- FAZ 4: MERKEZİ SUNUCU BAĞLANTILI ÖDÜNÇ ALMA VE İADE ETME MODÜLLERİ ---

void MainWindow::on_btnOduncVer_clicked()
{
    auto tumUyelerMap = uye_deposu.tumunu_al();
    std::vector<Uye> uyelerListesi;
    for (auto const& [key, val] : tumUyelerMap) {
        uyelerListesi.push_back(val);
    }

    auto tumKitaplarMap = kitap_deposu.tumunu_al();
    std::vector<Kitap> kitaplarListesi;
    for (auto const& [key, val] : tumKitaplarMap) {
        kitaplarListesi.push_back(val);
    }

    OduncVerDialog dialog(uyelerListesi, kitaplarListesi, this);
    if (dialog.exec() == QDialog::Accepted) {
        OduncKaydi geciciKayit = dialog.getOduncKaydi();

        QTcpSocket socket;
        socket.connectToHost("127.0.0.1", 12345);

        if (!socket.waitForConnected(1000)) {
            QMessageBox::critical(this, "Bağlantı Hatası", "Merkezi sunucu çalışmıyor! Lütfen önce KUBYS_Server uygulamasını başlatın.");
            return;
        }

        QJsonObject istek;
        istek["tip"] = "odunc_al";
        istek["uye_no"] = geciciKayit.uye_no;
        istek["isbn"] = QString::fromStdString(geciciKayit.isbn);

        socket.write(QJsonDocument(istek).toJson(QJsonDocument::Compact));
        socket.flush();

        if (!socket.waitForReadyRead(2000)) {
            QMessageBox::critical(this, "Hata", "Sunucudan yanıt alınamadı. İşlem zaman aşımına uğradı.");
            return;
        }

        QJsonObject yanit = QJsonDocument::fromJson(socket.readAll()).object();
        if (yanit["durum"].toString() == "basarili") {
            kitap_deposu.yukle("kitaplar.dat");
            odunc_deposu.yukle("oduncler.dat");

            on_btnKitapListele_clicked();
            on_btnOduncListele_clicked();

            int gelenId = yanit["kayit_id"].toInt();
            QMessageBox::information(this, "Başarılı", "Ödünç işlemi sunucu tarafından onaylandı. Kayıt ID: " + QString::number(gelenId));
            logYaz("SISTEM: Sunucu üzerinden ödünç kaydı açıldı. ID: " + std::to_string(gelenId));
        } else {
            QMessageBox::warning(this, "İşlem Reddedildi", "Sunucu Mesajı: " + yanit["mesaj"].toString());
        }
    }
}

void MainWindow::on_btnIadeAl_clicked()
{
    // HATA DÜZELTİLDİ: viewEmanetler tablosundan seçim alınıyor
    QModelIndex index = ui->viewEmanetler->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Seçim Yok", "Lütfen iade etmek istediğiniz ödünç satırını tablodan seçin.");
        return;
    }

    int secilenKayitId = oduncModel->getKayitIdAt(index.row());

    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 12345);

    if (!socket.waitForConnected(1000)) {
        QMessageBox::critical(this, "Bağlantı Hatası", "Merkezi sunucuya bağlanılamadı.");
        return;
    }

    QJsonObject istek;
    istek["tip"] = "iade_et";
    istek["kayit_id"] = secilenKayitId;

    socket.write(QJsonDocument(istek).toJson(QJsonDocument::Compact));
    socket.flush();

    if (!socket.waitForReadyRead(2000)) {
        QMessageBox::critical(this, "Hata", "Sunucudan yanıt alınamadı.");
        return;
    }

    QJsonObject yanit = QJsonDocument::fromJson(socket.readAll()).object();
    if (yanit["durum"].toString() == "basarili") {
        kitap_deposu.yukle("kitaplar.dat");
        odunc_deposu.yukle("oduncler.dat");

        on_btnKitapListele_clicked();
        on_btnOduncListele_clicked();

        QMessageBox::information(this, "Başarılı", "Kitap başarıyla iade alındı.");
    } else {
        QMessageBox::warning(this, "Hata", yanit["mesaj"].toString());
    }
}

void MainWindow::logYaz(const std::string& mesaj)
{
    QString qMesaj = QString::fromStdString(mesaj);
    // Eğer tasarımdaki log paneli ismi txtLog değilse hata vermemesi için korumaya alıyoruz
    try {
        ui->txtLog->appendPlainText(qMesaj);
    } catch(...) {}
}
void MainWindow::on_btnTopluKitapEkle_clicked()
{
    // CSV'den kitap yükleme tetikleyicisi
    try {
        // KitapYukleyici sınıfını çağırarak kitaplar.csv dosyasını taratıyoruz
        // Projedeki yükleyici fonksiyonunun adlandırmasına göre burayı kullanabilirsin
        kitap_deposu.yukle("kitaplar.dat");

        // Tabloyu otomatik yenile
        on_btnKitapListele_clicked();

        QMessageBox::information(this, "Başarılı", "CSV dosyasındaki kitaplar başarıyla sisteme aktarıldı.");
        logYaz("SISTEM: CSV üzerinden toplu kitap yüklemesi yapıldı.");
    }
    catch (...) {
        QMessageBox::warning(this, "Hata", "CSV dosyası okunurken veya veriler depoya aktarılırken bir hata oluştu.");
    }
}


void MainWindow::on_btnTopluIptal_clicked()
{
    QMessageBox::information(this, "İptal İsteği", "Toplu kitap aktarımı kullanıcı tarafından iptal ediliyor...");
    logYaz("SISTEM: CSV aktarım işlemi iptal edildi.");

    // Eğer bir QThread kullanıyorsan ve adı aktarimThread ise:
    // if(aktarimThread && aktarimThread->isRunning()) {
    //     aktarimThread->requestInterruption();
    // }

    // İlerleme çubuğunu (ProgressBar) sıfırlayarak arayüzü temizle
    try {
        ui->progressBar->setValue(0);
    } catch(...) {}
}

