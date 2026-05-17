#include "uyeekledialog.h"
#include "ui_uyeekledialog.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QDate>

UyeEkleDialog::UyeEkleDialog(QWidget *parent) : QDialog(parent), ui(new Ui::UyeEkleDialog) {
    ui->setupUi(this);

    QRegularExpression telRegex("^[0-9]{10}$");
    QRegularExpressionValidator *telValidator = new QRegularExpressionValidator(telRegex, this);
    ui->txtTelefon->setValidator(telValidator);
}

UyeEkleDialog::~UyeEkleDialog() {
    delete ui;
}

void UyeEkleDialog::on_btnKaydet_clicked() {
    int uyeNo = ui->txtUyeNo->text().trimmed().toInt();
    QString isim = ui->txtIsim->text().trimmed();
    QString soyisim = ui->txtSoyisim->text().trimmed();
    QString telefon = ui->txtTelefon->text().trimmed();

    if (isim.isEmpty() || soyisim.isEmpty() || telefon.length() != 10) {
        QMessageBox::warning(this, "Eksik Bilgi", "Lutfen isim, soyisim, 10 haneli telefon numarasini eksiksiz girin!");
        return;
    }

    QString otomatikTarih = QDate::currentDate().toString("yyyy-MM-dd");

    yeniUye.uye_no = uyeNo;
    yeniUye.isim = isim.toStdString();
    yeniUye.soyisim = soyisim.toStdString();
    yeniUye.telefon = telefon.toStdString();
    yeniUye.kayit_tarihi = otomatikTarih.toStdString();

    accept();
}

Uye UyeEkleDialog::getUye() const {
    return yeniUye;
}