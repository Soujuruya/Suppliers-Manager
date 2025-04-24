#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addnewsuplier.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QLibraryInfo>
#include <QHeaderView>
#include <QCloseEvent>
#include <QSettings>
#include <QSize>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tableModel(new MyTableModel(this))
    , languageActionGroup(nullptr)
{
    ui->setupUi(this);

    ui->tableView->setModel(tableModel);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(100);
    ui->tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    ui->AddSupplier->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->delRow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    ui->AddSupplier->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->delRow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);


    int buttonHeight = 40;
    ui->AddSupplier->setMinimumHeight(buttonHeight);
    ui->delRow->setMinimumHeight(buttonHeight);

    connect(tableModel, &MyTableModel::dataModified, this, [=]() {
        isModified = true;
        setWindowTitleWithPath();
    });

    connect(ui->actionAbout_QT, &QAction::triggered, qApp, &QApplication::aboutQt);

    qApp->installTranslator(&appTranslator);
    qApp->installTranslator(&qtTranslator);
    qmPath = qApp->applicationDirPath() + "/tr";

    createLanguageMenu();
    setWindowTitleWithPath();
    readSettings();
}

MainWindow::~MainWindow()
{
    delete languageActionGroup;
    delete ui;
    writeSettings();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

bool MainWindow::maybeSave()
{
    if (!isModified)
        return true;

    const auto ret = QMessageBox::warning(this, tr("Unsaved Changes"),
                                          tr("The document has been modified.\nDo you want to save your changes?"),
                                          QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    switch (ret) {
    case QMessageBox::Save:
        on_actionSave_triggered();
        return !isModified;
    case QMessageBox::Cancel:
        return false;
    case QMessageBox::Discard:
        return true;
    default:
        return false;
    }
}

void MainWindow::on_actionNew_triggered()
{
    if (maybeSave()) {
        tableModel->resetModel();
        currentFilePath.clear();
        isModified = false;
        setWindowTitleWithPath();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if (!maybeSave())
        return;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                                                    tr("Text Files (*.csv)"));
    if (fileName.isEmpty())
        return;

    loadFile(fileName);
}


void MainWindow::on_actionSave_triggered()
{
    if (currentFilePath.isEmpty()) {
        on_actionSave_as_triggered();
    } else {
        saveFile(currentFilePath);
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty())
        saveFile(fileName);
}

void MainWindow::on_actionClose_triggered()
{
    close();
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot read file:\n%1").arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    tableModel->resetModel();

    int row = 0;
    while (!in.atEnd()) {
        QStringList columns = in.readLine().split(";");
        if (columns.size() >= 8) {
            bool ok;
            int id = columns[0].trimmed().toInt(&ok);
            if (!ok) id = 0;

            MyTableModel::Suppliers sup(id,
                                        columns[1].trimmed(), columns[2].trimmed(), columns[3].trimmed(),
                                        columns[4].trimmed(), columns[5].trimmed(), columns[6].trimmed(),
                                        columns[7].trimmed());

            tableModel->addSupplier(sup, row, row);
            row++;
        }
    }

    QApplication::restoreOverrideCursor();
    file.close();
    currentFilePath = fileName;
    isModified = false;
    setWindowTitleWithPath();
}

void MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot write file:\n%1").arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    int rowCount = tableModel->rowCount();
    int colCount = tableModel->columnCount();

    for (int row = 0; row < rowCount; ++row) {
        QStringList rowData;
        for (int col = 0; col < colCount; ++col) {
            QModelIndex index = tableModel->index(row, col);
            rowData << tableModel->data(index).toString();
        }
        out << rowData.join(";") << "\n";
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::restoreOverrideCursor();

    file.close();
    currentFilePath = fileName;
    isModified = false;
    QMessageBox::information(this, tr("Success"), tr("File saved successfully."));
    setWindowTitleWithPath();
}

void MainWindow::setWindowTitleWithPath()
{
    QString title = "Supplier Manager";
    if (!currentFilePath.isEmpty())
        title += " - " + QFileInfo(currentFilePath).fileName();
    if (isModified)
        title += " *";
    setWindowTitle(title);
}

void MainWindow::switchLanguage(QAction *action)
{
    QString locale = action->data().toString();
    qApp->removeTranslator(&appTranslator);
    qApp->removeTranslator(&qtTranslator);

    if (appTranslator.load("MyKursovaya_" + locale + ".qm", qmPath))
        qApp->installTranslator(&appTranslator);
    if (qtTranslator.load("qt_" + locale + ".qm", QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
        qApp->installTranslator(&qtTranslator);

    ui->retranslateUi(this);
}

void MainWindow::createLanguageMenu()
{
    languageActionGroup = new QActionGroup(this);
    connect(languageActionGroup, &QActionGroup::triggered, this, &MainWindow::switchLanguage);

    QDir dir(qmPath);
    QStringList fileNames = dir.entryList(QStringList("MyKursovaya_*.qm"));

    for (int i = 0; i < fileNames.size(); ++i) {
        QString locale = fileNames[i];
        locale.remove(0, locale.indexOf('_') + 1);
        locale.truncate(locale.lastIndexOf('.'));

        QTranslator translator;
        translator.load(fileNames[i], qmPath);

        QString language = translator.translate("MainWindow", "English");

        QAction *action = new QAction(tr("&%1 %2").arg(i + 1).arg(language));
        action->setCheckable(true);
        action->setData(locale);

        ui->menuLanguage->addAction(action);
        languageActionGroup->addAction(action);

        if (language == "English")
            action->setChecked(true);
    }
}

void MainWindow::on_action_5_triggered()
{
    QMessageBox msgBox;
    msgBox.setStyleSheet("QLabel{min-width: 200px;}");
    msgBox.setIconPixmap(QPixmap("image/icon8-reference-96.png"));
    msgBox.setWindowTitle(tr("Information about the creator"));
    msgBox.setText(tr("Astapenko Nikita 2-102 ICMTS"));
    msgBox.exec();
}

void MainWindow::on_AddSupplier_clicked()
{
    int currentRow = ui->tableView->currentIndex().row();
    AddNewSuplier dialog(currentRow, tableModel, this);
    dialog.exec();
}

void MainWindow::on_actionAdd_triggered()
{
    on_AddSupplier_clicked();
}

void MainWindow::on_delRow_clicked()
{
    if (tableModel) {
        int row = ui->tableView->currentIndex().row();
        if (!tableModel->removeRow(row))
            QMessageBox::information(this, tr("Deletion Error"), tr("The table is empty."));
    }
}

void MainWindow::readSettings()
{
    QSettings settings("MySo", "Suppliers Manager");

    settings.beginGroup("MainWindowGeometry");
    resize(settings.value("size", QSize(800, 600)).toSize());
    move(settings.value("position", QPoint(200, 200)).toPoint());
    if (settings.value("FullScreen", false).toBool())
        showMaximized();
    settings.endGroup();
}

void MainWindow::writeSettings()
{

    QSettings settings("MySo", "Suppliers Manager");

    settings.beginGroup("MainWindowGeometry");
    settings.setValue("size", size());
    settings.setValue("position", pos());
    settings.setValue("FullScreen", isMaximized());
    settings.endGroup();

}
