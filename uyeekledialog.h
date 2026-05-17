#ifndef UYEEKLEDIALOG_H
#define UYEEKLEDIALOG_H

#include <QDialog>
#include "varliklar.h"

namespace Ui { class UyeEkleDialog; }

class UyeEkleDialog : public QDialog {
    Q_OBJECT
public:
    explicit UyeEkleDialog(QWidget *parent = nullptr);
    ~UyeEkleDialog();

    Uye getUye() const;

private slots:
    void on_btnKaydet_clicked();
    void on_btnIptal_clicked();

private:
    Ui::UyeEkleDialog *ui;
};

#endif
