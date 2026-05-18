#include "kitapekledialog.h"
#include "ui_kitapekledialog.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>

KitapEkleDialog::KitapEkleDialog(QWidget *parent) : QDialog(parent), ui(new Ui::KitapEkleDialog) {
    ui->setupUi(this);

    // 13 haneli ISBN doğrulaması (Sadece sayılar)
    QRegularExpression isbnRegex("^[0-9]{13}$");
    QRegularExpressionValidator *isbnValidator = new QRegularExpressionValidator(isbnRegex, this);
    ui->txtIsbn->setValidator(isbnValidator);
}

KitapEkleDialog::~KitapEkleDialog() {
    delete ui;
}

// Hem ButtonBox hem de kendi butonun için ortak doğrulama ve kaydetme fonksiyonu
void KitapEkleDialog::on_buttonBox_accepted() {
    // Kod tekrarı olmaması için doğrudan buton tıklama fonksiyonunu çağırıyoruz
    on_btnKaydet_clicked();
}

Kitap KitapEkleDialog::getKitap() const {
    return yeniKitap;
}

// Tasarımdaki "Yeni Kitap Kaydet" butonun tıklandığında burası çalışacak
void KitapEkleDialog::on_btnKaydet_clicked()
{
    QString isbn = ui->txtIsbn->text().trimmed();
    QString baslik = ui->txtKitapAdi->text().trimmed();
    QString yazar = ui->txtYazar->text().trimmed();
    QString kategori = ui->txtKategori->text().trimmed();
    int yil = ui->spinYil->value();

    // Kopya sayısı için eğer QSpinBox kullandıysan ui->txtKopyaSayisi->value() yapmalısın.
    // LineEdit kullandığını varsayarak mevcut kodunu koruyoruz:
    int kopya = ui->txtKopyaSayisi->text().trimmed().toInt();

    // Girdi Kontrolleri (Validation)
    if (isbn.length() != 13 || baslik.isEmpty() || yazar.isEmpty() || kategori.isEmpty()) {
        QMessageBox::warning(this, "Eksik Bilgi", "Lütfen 13 haneli ISBN numarasını ve tüm zorunlu alanları doldurun!");
        return;
    }

    if (kopya <= 0) {
        QMessageBox::warning(this, "Geçersiz Veri", "Kopya sayısı 0'dan büyük olmalıdır!");
        return;
    }

    // Verileri lokal nesneye aktarıyoruz
    yeniKitap.isbn = isbn.toStdString();
    yeniKitap.baslik = baslik.toStdString();
    yeniKitap.yazar = yazar.toStdString();
    yeniKitap.kategori = kategori.toStdString();
    yeniKitap.yayin_yili = yil;
    yeniKitap.kopya_sayisi = kopya;

    // Dialog penceresini kapatır ve MainWindow'a QDialog::Accepted (Onay) sinyali gönderir
    accept();
}
