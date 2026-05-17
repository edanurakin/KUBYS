#include "uyemodel.h"

UyeModel::UyeModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

int UyeModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(m_uyeler.size());
}

int UyeModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return 5;
}

QVariant UyeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_uyeler.size()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        const auto &uye = m_uyeler[index.row()];
        switch (index.column()) {
        case 0:
            return uye.uye_no;
        case 1:
            return QString::fromStdString(uye.isim);
        case 2:
            return QString::fromStdString(uye.soyisim);
        case 3:
            return QString::fromStdString(uye.telefon);
        case 4:
            return QString::fromStdString(uye.kayit_tarihi);
        default:
            return QVariant();
        }
    }
    return QVariant();
}
QVariant UyeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "Üye No";
        case 1: return "İsim";
        case 2: return "Soyisim";
        case 3: return "Telefon";
        case 4: return "Kayıt Tarihi";
        }
    }
    return QVariant();
}

void UyeModel::setUyeler(const std::vector<Uye>& yeniUyeler) {
    beginResetModel();
    m_uyeler = yeniUyeler;
    endResetModel();
}
