#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui/jshighlighter.h"
#include "presentation/fileloadsave.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    JSHighlighter *highlighter;

    FileLoadSave* fileOperations;

private:

    QString preProxessQuey(QString query);
	QString LoadExtensions();
	void LoadQueryToGUI(QString query);

    QString GetAutoLoadText(QString script);

private slots:
    void on_pushButton_2_clicked();
    void UpdateLastUsedMenu();
    void on_actionAuto_update_triggered();
    void on_openFile_clicked();
    void on_pushButton_clicked();
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void on_pushButton_Go_clicked();
    void on_actionExecute_triggered();
	void on_actionZoom_In_triggered();
	void on_actionZoom_Out_triggered();
};

#endif // MAINWINDOW_H
