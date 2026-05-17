#ifndef KITAPMODEL_H
#define KITAPMODEL_H

#include <QAbstractTableModel>
#include "varliklar.h"
#include <vector>

class KitapModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit KitapModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setKitaplar(const std::vector<Kitap>& yeniKitaplar);

private:
    std::vector<Kitap> m_kitaplar;
};

#endif