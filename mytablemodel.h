
#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>

class MyTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    struct Suppliers
    {
        int m_id;
        QString m_nameCompany;
        QString m_address;
        QString m_phoneNumber;
        QString m_eMail;
        QString m_product;
        QString m_deliveryTime;
        QString m_minimumOrder;
        Suppliers(const int id,
                  const QString nameCompany ,
                  const QString address ,
                  const QString phoneNumber,
                  const QString eMail ,
                  const QString product ,
                  const QString deliveryTime ,
                  const QString minimumOrder) : m_id(id) ,
                                              m_nameCompany(nameCompany),
                                              m_address(address),
                                              m_phoneNumber(phoneNumber),
                                              m_eMail(eMail),
                                              m_product(product),
                                              m_deliveryTime(deliveryTime),
                                              m_minimumOrder(minimumOrder){}
        Suppliers(const Suppliers &other) : m_id(other.m_id),
                                            m_nameCompany(other.m_nameCompany),
                                            m_address(other.m_address),
                                            m_phoneNumber(other.m_phoneNumber),
                                            m_eMail(other.m_eMail),
                                            m_product(other.m_product),
                                            m_deliveryTime(other.m_deliveryTime),
                                            m_minimumOrder(other.m_minimumOrder){}
    };

signals:
    void dataModified();

private:
    QList<Suppliers> supplier;
    const int columnsNum;
    bool isModified;


public:
    explicit MyTableModel(QObject *parent = nullptr);
    void resetModel() {
        beginResetModel();
        supplier.clear();
        endResetModel();
    }
    void addRow(){
         endInsertRows();
    }

    void addSupplier(const Suppliers& supplier,int row1,int row2) {
        beginInsertRows(QModelIndex(), row1, row2);
        this->supplier.append(supplier);
        endInsertRows();
    }



public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRow(int position,const Suppliers &value);
    bool removeRow(int position);

    const QList<Suppliers> &getData() const;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
};

#endif // MYTABLEMODEL_H
