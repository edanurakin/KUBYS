#ifndef KITAPEKLEDIALOG_H
#define KITAPEKLEDIALOG_H

#include <QDialog>
#include "varliklar.h"

namespace Ui {
class KitapEkleDialog;
}

class KitapEkleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KitapEkleDialog(QWidget *parent = nullptr);
    ~KitapEkleDialog();
    Kitap getKitap() const;

private slots:
    void on_buttonBox_accepted();

    void on_btnKaydet_clicked();

private:
    Ui::KitapEkleDialog *ui;
    Kitap yeniKitap;
};

#endif