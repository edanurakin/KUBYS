#include "oduncverdialog.h"
#include "ui_oduncverdialog.h"
#include <QMessageBox>
#include <QDate>

OduncVerDialog::OduncVerDialog(const std::vector<Uye>& uyeler, const std::vector<Kitap>& kitaplar, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OduncVerDialog)
{
    ui->setupUi(this);

    ui->dateOdunc->setDate(QDate::currentDate());

    for (const auto& uye : uyeler) {
        QString uyeMetni = QString::number(uye.uye_no) + " - " + QString::fromStdString(uye.isim) + " " + QString::fromStdString(uye.soyisim);
        ui->cmbUye->addItem(uyeMetni, uye.uye_no);
    }

    for (const auto& kitap : kitaplar) {
        if (kitap.kopya_sayisi > 0) {
            QString kitapMetni = QString::fromStdString(kitap.baslik) + " (" + QString::fromStdString(kitap.isbn) + ")";
            ui->cmbKitap->addItem(kitapMetni, QString::fromStdString(kitap.isbn));
        }
    }
}

OduncVerDialog::~OduncVerDialog()
{
    delete ui;
}

void OduncVerDialog::on_btnKaydet_clicked()
{
    if (ui->cmbUye->currentIndex() == -1 || ui->cmbKitap->currentIndex() == -1) {
        QMessageBox::warning(this, "Eksik Secim", "Lutfen uye ve kitap secimini eksiksiz yapin!");
        return;
    }

    int secilenUyeNo = ui->cmbUye->currentData().toInt();
    QString secilenIsbn = ui->cmbKitap->currentData().toString();
    QString oduncTarihi = ui->dateOdunc->date().toString("yyyy-MM-dd");

    yeniKayit.kayit_id = QDate::currentDate().toString("hhmmss").toInt();
    yeniKayit.uye_no = secilenUyeNo;
    yeniKayit.isbn = secilenIsbn.toStdString();
    yeniKayit.odunc_tarihi = oduncTarihi.toStdString();
    yeniKayit.iade_tarihi = std::nullopt;
    yeniKayit.durum = OduncDurum::Oduncte;

    accept();
}

OduncKaydi OduncVerDialog::getOduncKaydi() const
{
    return yeniKayit;
}

