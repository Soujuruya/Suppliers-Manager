#include "mytablemodel.h"


MyTableModel::MyTableModel(QObject *parent):
    QAbstractTableModel{parent},
    columnsNum(8)
{
    isModified = false;
}

int MyTableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : supplier.count();
}

int MyTableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : columnsNum;
}

QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
    if(!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant();

    int row = index.row();
    int col = index.column();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (col == 0)
            return supplier.at(row).m_id;
        if (col == 1)
            return supplier.at(row).m_nameCompany;
        if (col == 2)
            return supplier.at(row).m_address;
        if (col == 3)
            return supplier.at(row).m_phoneNumber;
        if (col == 4)
            return supplier.at(row).m_eMail;
        if (col == 5)
            return supplier.at(row).m_product;
        if (col == 6)
            return supplier.at(row).m_deliveryTime;
        if (col == 7)
            return supplier.at(row).m_minimumOrder;
    }
    return QVariant();
}

bool MyTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return false;

    int col = index.column();
    int row = index.row();

    if (role == Qt::EditRole) {
        Suppliers &sup = supplier[row];

        bool valueChanged = false;

        switch (col) {
        case 0:
            if (sup.m_id != value.toInt()) {
                sup.m_id = value.toInt();
                valueChanged = true;
            }
            break;
        case 1:
            if (sup.m_nameCompany != value.toString()) {
                sup.m_nameCompany = value.toString();
                valueChanged = true;
            }
            break;
        case 2:
            if (sup.m_address != value.toString()) {
                sup.m_address = value.toString();
                valueChanged = true;
            }
            break;
        case 3:
            if (sup.m_phoneNumber != value.toString()) {
                sup.m_phoneNumber = value.toString();
                valueChanged = true;
            }
            break;
        case 4:
            if (sup.m_eMail != value.toString()) {
                sup.m_eMail = value.toString();
                valueChanged = true;
            }
            break;
        case 5:
            if (sup.m_product != value.toString()) {
                sup.m_product = value.toString();
                valueChanged = true;
            }
            break;
        case 6:
            if (sup.m_deliveryTime != value.toString()) {
                sup.m_deliveryTime = value.toString();
                valueChanged = true;
            }
            break;
        case 7:
            if (sup.m_minimumOrder != value.toString()) {
                sup.m_minimumOrder = value.toString();
                valueChanged = true;
            }
            break;
        }

        if (valueChanged) {
            emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
            emit dataModified();

            isModified = true;
            return true;
        }
    }

    return false;
}


QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        switch (section) {
        case 0:
            return QString("ID");
        case 1:
            return QString(tr("Supplier name"));
        case 2:
            return QString(tr("Address"));
        case 3:
            return QString(tr("Phone number"));
        case 4:
            return QString(tr("Email"));
        case 5:
            return QString(tr("Product"));
        case 6:
            return QString(tr("Delivery time(days)"));
        case 7:
            return QString(tr("Minimum order(units)"));
        }
    }

    if(orientation == Qt::Vertical && role == Qt::DisplayRole){
        return QString::number(section+1);
    }
    return QVariant();
}

Qt::ItemFlags MyTableModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
        return Qt::ItemIsEnabled |
               Qt::ItemIsSelectable |
               Qt::ItemIsEditable;
    else
        return Qt::NoItemFlags;
}

bool MyTableModel::insertRow(int position, const Suppliers &value)
{
    beginInsertRows(QModelIndex(),position,position);

    supplier.insert(position,value);

    endInsertRows();

    emit dataModified();

    return true;
}

bool MyTableModel::removeRow(int position)
{
    if (supplier.isEmpty())
        return false;

    beginRemoveRows(QModelIndex(),position,position);

    supplier.removeAt(position);

    endRemoveRows();

    emit dataModified();

    return true;
}

void MyTableModel::sort(int column, Qt::SortOrder order)
{
    emit layoutAboutToBeChanged();

    switch(column){
    case 0:
        if(order == Qt::AscendingOrder)
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                         const Suppliers &sup2){
                return sup1.m_id < sup2.m_id;
            });
        else
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                           const Suppliers &sup2){
                return sup1.m_id > sup2.m_id;
            });
        break;
    case 1:
        if(order == Qt::AscendingOrder)
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                           const Suppliers &sup2){
                return sup1.m_nameCompany < sup2.m_nameCompany;
            });
        else
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                           const Suppliers &sup2){
                return sup1.m_nameCompany > sup2.m_nameCompany;
            });
        break;
    case 2:
        if(order == Qt::AscendingOrder)
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                           const Suppliers &sup2){
                return sup1.m_address < sup2.m_address;
            });
        else
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                           const Suppliers &sup2){
                return sup1.m_address > sup2.m_address;
            });
        break;
    case 3:
        if(order == Qt::AscendingOrder)
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                           const Suppliers &sup2){
                return sup1.m_phoneNumber < sup2.m_phoneNumber;
            });
        else
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                           const Suppliers &sup2){
                return sup1.m_phoneNumber > sup2.m_phoneNumber;
            });
        break;
    case 4:
        if(order == Qt::AscendingOrder)
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                           const Suppliers &sup2){
                return sup1.m_eMail < sup2.m_eMail;
            });
        else
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                           const Suppliers &sup2){
                return sup1.m_eMail > sup2.m_eMail;
            });
        break;
    case 5:
        if(order == Qt::AscendingOrder)
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                           const Suppliers &sup2){
                return sup1.m_product < sup2.m_product;
            });
        else
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                           const Suppliers &sup2){
                return sup1.m_product > sup2.m_product;
            });
        break;
    case 6:
        if(order == Qt::AscendingOrder)
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                           const Suppliers &sup2){
                return sup1.m_deliveryTime < sup2.m_deliveryTime;
            });
        else
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                           const Suppliers &sup2){
                return sup1.m_deliveryTime > sup2.m_deliveryTime;
            });
        break;
    case 7:
        if(order == Qt::AscendingOrder)
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                           const Suppliers &sup2){
                return sup1.m_minimumOrder < sup2.m_minimumOrder;
            });
        else
            std::sort(supplier.begin(),supplier.end(),[](const Suppliers &sup1,
                                                           const Suppliers &sup2){
                return sup1.m_minimumOrder > sup2.m_minimumOrder;
            });

        break;
    }

    emit layoutChanged();
}

const QList<MyTableModel::Suppliers> &MyTableModel::getData() const
{
    return supplier;
}
