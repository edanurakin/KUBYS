#include "kitapmodel.h"
#include "varliklar.h"

KitapModel::KitapModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

int KitapModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(m_kitaplar.size());
}

int KitapModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return 6;
}

QVariant KitapModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();

    const auto& kitap = m_kitaplar[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return QString::fromStdString(kitap.isbn);
        case 1: return QString::fromStdString(kitap.baslik);
        case 2: return QString::fromStdString(kitap.yazar);
        case 3: return kitap.yayin_yili;
        case 4: return QString::fromStdString(kitap.kategori);
        case 5: return kitap.kopya_sayisi;
        }
    }
    return QVariant();
}

QVariant KitapModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "ISBN";
        case 1: return "Kitap Başlığı";
        case 2: return "Yazar";
        case 3: return "Yayın Yılı";
        case 4: return "Kategori";
        case 5: return "Kopya Sayısı";
        }
    }
    return QVariant();
}

void KitapModel::setKitaplar(const std::vector<Kitap>& yeniKitaplar) {
    beginResetModel();
    m_kitaplar = yeniKitaplar;
    endResetModel();
}