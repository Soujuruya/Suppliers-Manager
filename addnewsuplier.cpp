#include "addnewsuplier.h"
#include "ui_addnewsuplier.h"
#include "mytablemodel.h"

#include <QMessageBox>
#include <QString>
#include <QRegularExpressionValidator>

AddNewSuplier::AddNewSuplier(int currentRow,MyTableModel* model,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddNewSuplier),
      m_currentRow(currentRow),
      tableModel(model)
{
    ui->setupUi(this);


    //SIZE POLITIC

    QList<QLineEdit*> fields = {
        ui->lineID,
        ui->lineSuplierName,
        ui->lineAddress,
        ui->linePhoneNumber,
        ui->lineEmail,
        ui->lineProduct,
        ui->lineDeliveryTime,
        ui->lineMinimumOrder
    };

    for (QLineEdit* field : fields) {
        field->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }


    ui->AddNewRow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->pushButton_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //SIZE POLITIC

    ui->lineID->setInputMask("000000");
    ui->linePhoneNumber->setInputMask("+7 (000) 000-00-00; ");

    QRegularExpression emailRegex(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    ui->lineEmail->setValidator(new QRegularExpressionValidator(emailRegex, this));

    ui->lineDeliveryTime->setInputMask("00");
    ui->lineMinimumOrder->setInputMask("0000");




}

AddNewSuplier::~AddNewSuplier()
{
    delete ui;
}

void AddNewSuplier::on_AddNewRow_clicked()
{
    bool checker;
    int id = ui -> lineID -> text().toInt(&checker);

    if(id == 0){
        QMessageBox::warning(this,tr("Error"),tr("The ID field is empty."));
        return;
    }

    if(!checker) {
        QMessageBox::warning(this,tr("Error"),tr("ID must be a number."));
        return;
    }


    QString nameCompany = ui -> lineSuplierName -> text();
    QString address = ui -> lineAddress -> text();
    QString phoneNumber = ui -> linePhoneNumber -> text();
    QString eMail = ui -> lineEmail -> text();
    QString product = ui -> lineProduct -> text();
    QString deliveryTime = ui -> lineDeliveryTime -> text();
    QString minimumOrder = ui -> lineMinimumOrder -> text();


    for (const auto& supplier : tableModel->getData()) {
        if (supplier.m_id == id) {
            QMessageBox::warning(this, tr("Duplicate Name"),
                                 QString(tr("The ID %1 already exists.")).arg(id));
            return;
        }
    }

    if (tableModel != nullptr){
        MyTableModel::Suppliers sup(id,nameCompany,address,phoneNumber,eMail,product,deliveryTime,minimumOrder);
        int row = m_currentRow;
        if(row > 0)
            tableModel -> insertRow(row + 1 ,sup);
        else
            tableModel -> insertRow(tableModel -> rowCount(),sup);
    }
    else
        QMessageBox::information(this,tr("Duplicate Name"),
                                 QString(tr("The ID %1 already exists.")).arg(id));

    this->accept();
}

