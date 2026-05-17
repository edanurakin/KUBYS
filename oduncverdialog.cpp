#include "oduncverdialog.h"
#include "ui_oduncverdialog.h"
#include <QMessageBox>

OduncVerDialog::OduncVerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OduncVerDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Yeni Ödünç İşlemi");
}

OduncVerDialog::~OduncVerDialog()
{
    delete ui;
}

void OduncVerDialog::on_btnKaydet_clicked()
{
    if(ui->txtKayitId->text().isEmpty() || ui->txtUyeNo->text().isEmpty() ||
        ui->txtIsbn->text().isEmpty() || ui->txtOduncTarihi->text().isEmpty()) {
        QMessageBox::warning(this, "Hata", "Lütfen tüm alanları doldurunuz!");
        return;
    }
    accept();
}

void OduncVerDialog::on_btnIptal_clicked()
{
    reject();
}

OduncKaydi OduncVerDialog::getOduncKaydi() const
{
    OduncKaydi ok;
    ok.kayit_id = ui->txtKayitId->text().toInt();
    ok.uye_no = ui->txtUyeNo->text().toInt();

    std::string isbnStr = ui->txtIsbn->text().toStdString();
    std::string tarihStr = ui->txtOduncTarihi->text().toStdString();
    ok.isbn = ui->txtIsbn->text().toStdString();
    ok.odunc_tarihi = ui->txtOduncTarihi->text().toStdString();
    ok.iade_tarihi = std::nullopt;
    ok.durum = OduncDurum::Oduncte;

    return ok;
}

