#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScriptEngine>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    QCoreApplication::setOrganizationName("SoftDent");
    QCoreApplication::setOrganizationDomain("softdent.lt");
    QCoreApplication::setApplicationName("Repleisiukas");

    I_MENU_ITEMS = 10;
    ui->setupUi(this);
    highlighter = new JSHighlighter(ui->query->document());

    last = new QFile("d:\\last.txt");

    if (!last->open(QIODevice::ReadWrite | QIODevice::Text))
        return;
    strem = new QTextStream(last);

    ui->query->setPlainText(strem->readAll());
    UpdateLastUsedMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete highlighter;
    last->close();
    delete strem;
    delete last;
}

void MainWindow::UpdateLastUsedMenu(){
    QStringList items;
    QSettings settings;

    for(unsigned int i = 0; i < I_MENU_ITEMS; i++){
        QString item = settings.value(QString("/files/load/%1").arg(i)).toString();

        if(item.isEmpty())
            break;

        items.append(item);
    }

    foreach (QString item, items) {
        int index = item.lastIndexOf('/');

        if(index == -1)
            index = item.lastIndexOf('\\');

        QString fileName = item;

        if(index != -1)
            fileName = QString("%1 [%2]").arg(item.left(index)).arg(item);

        ui->menuLastUsed->addAction(fileName, this, SLOT(on_openFile_clicked()));
    }
}

void MainWindow::UpdateLastUsedOrder(QString newName){
    QSettings settings;

    for(int i = I_MENU_ITEMS; i > 1; i++){
        QString prevItem = QString("/files/load/%1").arg(i-1);
        QString item = QString("/files/load/%1").arg(i);
        settings.setValue(item, settings.value(prevItem));
    }

    QString item = QString("/files/load/%1").arg(0);

    settings.setValue(item, newName);

    UpdateLastUsedMenu();
}

void MainWindow::LoadFromFile(QString fileName){
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::warning(this, "Could not open file", "could not open file");
            return;
        }
        QTextStream stream(&file);
        ui->query->setPlainText(stream.readAll());
        UpdateLastUsedOrder(fileName);
    }
}

void MainWindow::on_openFile_clicked(){
    QAction *senderAction = dynamic_cast<QAction*>(sender());

    if(senderAction){
        QString file = senderAction->data().toString();
        LoadFromFile(file);
    }
}

void MainWindow::on_pushButton_Go_clicked()
{
    last->resize(0);

    *strem << ui->query->toPlainText();
    strem->flush();

    QScriptEngine engine;
    QString in = ui->stringIn->text();

    QString query =  QString("%1 ; \n%2").arg(in).arg(ui->query->toPlainText());
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
    QSettings settings;
    QString path =  settings.value("/files/load/0").toString();
    int index = path.lastIndexOf("/");
    if(index == -1)
        index = path.lastIndexOf("\\");
    if(index == -1)
        index = path.length();

    path = path.left(index);

    qDebug() << path;

    QString fileName = QFileDialog::getOpenFileName(this, QString(), path);
    LoadFromFile(fileName);
}

void MainWindow::on_pushButton_clicked()
{
    ui->stringIn->clear();
    ui->stringIn->paste();
    on_pushButton_Go_clicked();
    ui->stringOut->selectAll();
    ui->stringOut->copy();
}
