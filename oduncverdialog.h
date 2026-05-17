#ifndef ODUNCVERDIALOG_H
#define ODUNCVERDIALOG_H

#include <QDialog>
#include "varliklar.h"

namespace Ui {
class OduncVerDialog;
}

class OduncVerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OduncVerDialog(QWidget *parent = nullptr);
    ~OduncVerDialog();

    OduncKaydi getOduncKaydi() const;

private slots:
    void on_btnKaydet_clicked();
    void on_btnIptal_clicked();

private:
    Ui::OduncVerDialog *ui;
};

#endif // ODUNCVERDIALOG_H