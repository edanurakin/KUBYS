#ifndef ODUNCVERDIALOG_H
#define ODUNCVERDIALOG_H

#include <QDialog>
#include <vector>
#include "varliklar.h"

namespace Ui {
class OduncVerDialog;
}

class OduncVerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OduncVerDialog(const std::vector<Uye>& uyeler, const std::vector<Kitap>& kitaplar, QWidget *parent = nullptr);
    ~OduncVerDialog();
    OduncKaydi getOduncKaydi() const;

private slots:
    void on_btnKaydet_clicked();

private:
    Ui::OduncVerDialog *ui;
    OduncKaydi yeniKayit;
};

#endif