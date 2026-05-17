#ifndef UYEMODEL_H
#define UYEMODEL_H

#include <QAbstractTableModel>
#include "varliklar.h"
#include <vector>

class UyeModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit UyeModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setUyeler(const std::vector<Uye>& yeniUyeler);

private:
    std::vector<Uye> m_uyeler;
};

#endif