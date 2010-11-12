#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include <QFile>
#include "jshighlighter.h"

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
    QTextStream* strem;
    QFile *last;
    JSHighlighter *highlighter;

private slots:
    void on_pushButton_clicked();
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void on_pushButton_Go_clicked();
};

#endif // MAINWINDOW_H
