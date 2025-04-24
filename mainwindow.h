#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QActionGroup>
#include <QMdiArea>

#include "mytablemodel.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionClose_triggered();
    void on_action_5_triggered();


    void on_AddSupplier_clicked();
    void on_actionAdd_triggered();
    void on_delRow_clicked();
    void readSettings();
    void writeSettings();

    void switchLanguage(QAction *action);

private:
    Ui::MainWindow *ui;
    MyTableModel *tableModel;
    QString currentFilePath;
    bool isModified = false;

    QMdiArea *mdiArea;

    QTranslator appTranslator;
    QTranslator qtTranslator;
    QString qmPath;
    QActionGroup *languageActionGroup;

    void loadFile(const QString &fileName);
    void saveFile(const QString &fileName);
    bool maybeSave();
    void createLanguageMenu();
    void setWindowTitleWithPath();
    void createNewDocument();
    //MdiChild *activeMdiChild() const;



protected:
    void closeEvent(QCloseEvent *event) override;

};

#endif // MAINWINDOW_H
