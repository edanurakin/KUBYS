#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QStandardItemModel>
#include <QMainWindow>
#include "depo.h"
#include "varliklar.h"
#include "kitapmodel.h"
#include "uyemodel.h"
#include "oduncmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    //void on_btnKitapEkle_clicked();

    void on_btnUyeEkle_clicked();

    void on_btnKitapEkle_2_clicked();

    void on_btnOduncVer_clicked();

private:
    Ui::MainWindow *ui;

    KitapModel *kitapModel;
    UyeModel *uyeModel;
    OduncModel *oduncModel;

    Depo<int, Uye> uye_deposu;
    Depo<int, OduncKaydi> odunc_deposu;
    Depo<std::string, Kitap> kitap_deposu;
};
#endif