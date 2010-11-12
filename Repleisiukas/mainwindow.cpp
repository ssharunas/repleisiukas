#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScriptEngine>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    highlighter = new JSHighlighter(ui->query->document());

    last = new QFile("d:\\last.txt");

    if (!last->open(QIODevice::ReadWrite | QIODevice::Text))
        return;
    strem = new QTextStream(last);

    ui->query->setPlainText(strem->readAll());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete highlighter;
    last->close();
    delete strem;
    delete last;
}

void MainWindow::on_pushButton_Go_clicked()
{
    //strem->device()->reset();
    last->resize(0);

    *strem << ui->query->toPlainText();
    strem->flush();

    QScriptEngine engine;
    QString in = ui->stringIn->toPlainText();
    in = in.replace('\'', "\\'");
    in = in.replace('\n', "\\n");

    QString query =  QString("wee='%1'; \n%2").arg(in).arg(ui->query->toPlainText());
    qDebug() << query;
    QScriptValue value = engine.evaluate(query);

    if(value.isError())
        ui->stringOut->setPlainText("Klaida: " + value.toString());
    else
        ui->stringOut->setPlainText(value.toString());
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream stream(&file);
        stream << ui->query->toPlainText();
    }
}

void MainWindow::on_actionLoad_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::warning(this, "Could not open file", "could not open file");
            return;
        }
        QTextStream stream(&file);
        ui->query->setPlainText(stream.readAll());
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->stringIn->clear();
    ui->stringIn->paste();
    on_pushButton_Go_clicked();
    ui->stringOut->selectAll();
    ui->stringOut->copy();
}
