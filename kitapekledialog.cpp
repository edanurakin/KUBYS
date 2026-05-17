#include <QMessageBox>
#include "kitapekledialog.h"
#include "ui_kitapekledialog.h"

KitapEkleDialog::KitapEkleDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::KitapEkleDialog)
{
    ui->setupUi(this);
}

KitapEkleDialog::~KitapEkleDialog()
{
    delete ui;
}

void KitapEkleDialog::on_btnKaydet_clicked()
{
    if(ui->txtIsbn->text().isEmpty() ||
        ui->txtKitapAdi->text().isEmpty() ||
        ui->txtYazar->text().isEmpty() ||
        ui->label_5->text().isEmpty())
    {
        QMessageBox::warning(this, "Eksik Bilgi", "Lütfen boş alanları doldurunuz!");
        return;
    }

    if(ui->txtIsbn->text().length() < 10) {
        QMessageBox::warning(this, "Hatalı Format", "ISBN en az 10 karakter olmalıdır!");
        return;
    }

    accept();
}

Kitap KitapEkleDialog::getKitap() const {
    Kitap k;
    k.isbn = ui->txtIsbn->text().toStdString();
    k.baslik = ui->txtKitapAdi->text().toStdString();
    k.yazar = ui->txtYazar->text().toStdString();
    k.kategori = ui->txtKategori->text().toStdString();
    k.yayin_yili = ui->spinYil->value();
    k.kopya_sayisi = ui->txtKopyaSayisi->text().toInt();
    return k;
}
