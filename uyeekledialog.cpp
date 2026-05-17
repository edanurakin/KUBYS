#include "uyeekledialog.h"
#include "ui_uyeekledialog.h"
#include <QMessageBox>
#include <QDateTime>
UyeEkleDialog::UyeEkleDialog(QWidget *parent) : QDialog(parent), ui(new Ui::UyeEkleDialog) {
    ui->setupUi(this);
    this->setWindowTitle("Yeni Üye Kaydı");
}

UyeEkleDialog::~UyeEkleDialog() {
    delete ui;
}

void UyeEkleDialog::on_btnKaydet_clicked() {
    if(ui->txtUyeNo->text().isEmpty() || ui->txtIsim->text().isEmpty() ||
        ui->txtSoyisim->text().isEmpty() || ui->txtTelefon->text().isEmpty()) {
        QMessageBox::warning(this, "Hata", "Lütfen tüm üye bilgilerini doldurunuz!");
        return;
    }
    accept();
}

void UyeEkleDialog::on_btnIptal_clicked() {
    reject();
}

Uye UyeEkleDialog::getUye() const {
    Uye u;
    u.uye_no = ui->txtUyeNo->text().toInt();
    u.isim = ui->txtIsim->text().toStdString();
    u.soyisim = ui->txtSoyisim->text().toStdString();
    u.telefon = ui->txtTelefon->text().toStdString();
    u.kayit_tarihi = QDateTime::currentDateTime().toString("dd.MM.yyyy").toStdString();
    return u;
}