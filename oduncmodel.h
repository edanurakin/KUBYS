#ifndef ODUNCMODEL_H
#define ODUNCMODEL_H

#include <QAbstractTableModel>
#include "varliklar.h"
#include <vector>

class OduncModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit OduncModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setKayitlar(const std::vector<OduncKaydi>& yeniKayitlar);
    void setOduncler(const std::vector<OduncKaydi>& kayitlar);

    // HATA ÇÖZÜMÜ: Seçili satırdaki kaydın ID'sini döndüren fonksiyon eklendi
    int getKayitIdAt(int row) const {
        if (row >= 0 && row < static_cast<int>(m_kayitlar.size())) {
            return m_kayitlar[row].kayit_id;
        }
        return -1;
    }

private:
    std::vector<OduncKaydi> m_kayitlar;
};

#endif // ODUNCMODEL_H