#include "kitapekledialog.h"
#include "ui_kitapekledialog.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>

KitapEkleDialog::KitapEkleDialog(QWidget *parent) : QDialog(parent), ui(new Ui::KitapEkleDialog) {
    ui->setupUi(this);

    QRegularExpression isbnRegex("^[0-8][0-9]{12}$");
    QRegularExpressionValidator *isbnValidator = new QRegularExpressionValidator(isbnRegex, this);
    ui->txtIsbn->setValidator(isbnValidator);
}

KitapEkleDialog::~KitapEkleDialog() {
    delete ui;
}

void KitapEkleDialog::on_buttonBox_accepted() {
    QString isbn = ui->txtIsbn->text().trimmed();
    QString baslik = ui->txtKitapAdi->text().trimmed();
    QString yazar = ui->txtYazar->text().trimmed();
    QString kategori = ui->txtKategori->text().trimmed();
    int yil = ui->spinYil->value();
    int kopya = ui->txtKopyaSayisi->text().trimmed().toInt();

    if (isbn.length() != 13 || baslik.isEmpty() || yazar.isEmpty() || kategori.isEmpty()) {
        QMessageBox::warning(this, "Eksik Bilgi", "Lutfen 13 haneli ISBN numarasini ve tum zorunlu alanlari doldurun!");
        return;
    }

    yeniKitap.isbn = isbn.toStdString();
    yeniKitap.baslik = baslik.toStdString();
    yeniKitap.yazar = yazar.toStdString();
    yeniKitap.kategori = kategori.toStdString();
    yeniKitap.yayin_yili = yil;
    yeniKitap.kopya_sayisi = kopya;

    accept();
}

Kitap KitapEkleDialog::getKitap() const {
    return yeniKitap;
}
