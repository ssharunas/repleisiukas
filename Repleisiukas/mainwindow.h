#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "jshighlighter.h"
#include "fileloadsave.h"

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

    void UpdateLastUsedOrder(QString newName);
    void LoadFromFile(QString file);

private slots:
	void UpdateLastUsedMenu();
    void on_actionAuto_update_triggered();
    void on_openFile_clicked();
    void on_pushButton_clicked();
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void on_pushButton_Go_clicked();
};

#endif // MAINWINDOW_H
