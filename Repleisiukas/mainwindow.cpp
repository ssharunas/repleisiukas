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

    fileOperations = new FileLoadSave(this);
    ui->setupUi(this);
    highlighter = new JSHighlighter(ui->query->document());

    ui->query->addAction(ui->actionExecute);
    ui->query->setPlainText(fileOperations->GetLastQuery());
    UpdateLastUsedMenu();

    QSettings settings;
    bool autoUpdate = settings.value("/settings/autoUpdate").toBool();
    ui->actionAuto_update->setChecked(autoUpdate);

    connect(fileOperations, SIGNAL(UpdateLastUsed()), this, SLOT(UpdateLastUsedMenu()));
    connect(ui->stringIn, SIGNAL(updateRequest()), this, SLOT(on_pushButton_Go_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete highlighter;
}

void MainWindow::UpdateLastUsedMenu()
{
    ui->menuLastUsed->clear();
    QStringList items = fileOperations->GetLastUsedMenuEntries();

    foreach (QString item, items)
    {
        QString name;
        name = item.section('/', -1);

        ui->menuLastUsed->addAction(name, this, SLOT(on_openFile_clicked()))
                ->setData(QVariant(item));
    }
}

void MainWindow::on_openFile_clicked()
{
    QAction *senderAction = dynamic_cast<QAction*>(sender());

    if(senderAction){
        QString file = senderAction->data().toString();
        qDebug() << "trying to open file" << file;

        QString string = fileOperations->LoadFromFile(file);

        if(!string.isNull())
            ui->query->setPlainText(string);
    }
}

QString MainWindow::GetAutoLoadText(QString script){
    QString result;
    int start = script.indexOf("--<autoload>--");
    if(start != -1)
    {
        int end = script.indexOf("--</autoload>--");

        if(end != -1 && end > start)
        {
            result = script.mid(start, end - start);
        }
    }

    return result;
}

QString MainWindow::LoadExtensions(){
    QString result;

    QSettings settings;
    QString extensionsPath = settings.value("/settings/extensionsPath",
                                            QApplication::applicationDirPath() + "/ext").toString();

    qDebug() << "Extensions:" << extensionsPath;
    QDir dir(extensionsPath);
    if(dir.exists())
    {
        QFileInfoList list = dir.entryInfoList(QDir::Files);
        for (int i = 0; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);

            QFile file(fileInfo.filePath());

            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream stream(&file);
                result += stream.readAll();

                result += "\n ; ";
                file.close();
            }
        }
    }

    return result;
}

QString MainWindow::preProxessQuey(QString query)
{
    QString result;
    bool changed = false;
    int lastIndex = 0;
    do
    {
        changed = false;
        int from = query.indexOf("\"\"\"", lastIndex);
        int to = query.indexOf("\"\"\"", from + 1);
        if(from >= 0 && to - from >= 3)
        {
            result += query.mid(lastIndex, from - lastIndex);

            result += "'";
            result += query.mid(from + 3, to - 3 - from).replace('\'', "\\'").replace('\r', "").replace('\n', "\\n' + \n'");
            result += "'";
            lastIndex = to + 3;

            changed = true;
        }
    }while(changed);

    result += query.mid(lastIndex);

    return result;
}

void MainWindow::on_pushButton_Go_clicked()
{
    fileOperations->SetLastQuery(ui->query->toPlainText());

    QScriptEngine engine;
    QString resources = fileOperations->LoadResource(":/scripts/extensions.js");
    QString in = ui->stringIn->text();
    QString extensions = LoadExtensions();

    //qDebug() << "EXTENSIONS" << extensions << "-------------";

    QString uiQuery = preProxessQuey(ui->query->toPlainText());
    qDebug() << uiQuery;
    QString query = extensions + QString(" ; %3 ;\n %1 ; \n%2")
                     .arg(in)
                     .arg(uiQuery)
                     .arg(resources);

    //qDebug() << query;

    QScriptValue value = engine.evaluate(query);

    if(value.isError())
        ui->stringOut->setPlainText("Klaida: " + value.toString());
    else
        ui->stringOut->setPlainText(value.toString());
}

void MainWindow::on_actionSave_triggered()
{
    fileOperations->SaveToFile(ui->query->toPlainText());
}

void MainWindow::on_actionLoad_triggered()
{
    QString string = fileOperations->LoadFromFile();

    if(!string.isNull())
        ui->query->setPlainText(string);

    QString autoload = GetAutoLoadText(string);
    if(!autoload.isNull())
    {
        ui->stringIn->setText(string);
        ui->stringIn->Dialogize();
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

void MainWindow::on_actionAuto_update_triggered()
{
    QSettings settings;
    settings.setValue("/settings/autoUpdate", QVariant(ui->actionAuto_update->isChecked()));
}

void MainWindow::on_actionExecute_triggered()
{
    on_pushButton_Go_clicked();
}

void MainWindow::on_pushButton_2_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this);

    if(!dir.isEmpty())
    {
        if(!(dir.endsWith('/') || dir.endsWith('\\')))
            dir.append("/");
        dir.append("*");

        ui->inFiles->addItem(dir);
    }
}
