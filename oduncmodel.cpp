#include "oduncmodel.h"
#include <QColor>
#include <QBrush>
#include "varliklar.h"

OduncModel::OduncModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

int OduncModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(m_kayitlar.size());
}

int OduncModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return 6;
}

QVariant OduncModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();

    const auto& kayit = m_kayitlar[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return kayit.kayit_id;
        case 1: return kayit.uye_no;
        case 2: return QString::fromStdString(kayit.isbn);
        case 3: return QString::fromStdString(kayit.odunc_tarihi);
        case 4: return QString::fromStdString(kayit.iade_tarihi.value_or("---"));
        case 5: return QString::fromStdString(durumYazisi(kayit.durum));
        }
    }

    if (role == Qt::BackgroundRole) {
        if (kayit.durum == OduncDurum::Gecikmis) {
            return QBrush(QColor(255, 200, 200));
        } else if (kayit.durum == OduncDurum::Oduncte) {
            return QBrush(QColor(200, 230, 255));
        } else if (kayit.durum == OduncDurum::IadeEdildi) {
            return QBrush(QColor(210, 255, 210));
        }
    }

    return QVariant();
}


QVariant OduncModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "Kayıt ID";
        case 1: return "Üye No";
        case 2: return "ISBN";
        case 3: return "Ödünç Tarihi";
        case 4: return "İade Tarihi";
        case 5: return "Durum";
        }
    }
    return QVariant();
}

void OduncModel::setKayitlar(const std::vector<OduncKaydi>& yeniKayitlar) {
    beginResetModel();
    m_kayitlar = yeniKayitlar;
    endResetModel();
}

std::string durumYazisi(OduncDurum durum) {
    switch (durum) {
    case OduncDurum::Oduncte:    return "Ödünçte";
    case OduncDurum::IadeEdildi: return "İade Edildi";
    case OduncDurum::Gecikmis:   return "Gecikmiş";
    default:                     return "Bilinmiyor";
    }
}