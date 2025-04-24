#ifndef ADDNEWSUPLIER_H
#define ADDNEWSUPLIER_H

#include <QDialog>
#include "mytablemodel.h"

namespace Ui {
class AddNewSuplier;
}

class AddNewSuplier : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewSuplier(int currentRow,MyTableModel* model,QWidget *parent = nullptr);
    ~AddNewSuplier();


private slots:
    void on_AddNewRow_clicked();

private:
    Ui::AddNewSuplier *ui;
    int m_currentRow;
    MyTableModel* tableModel;
};

#endif // ADDNEWSUPLIER_H
