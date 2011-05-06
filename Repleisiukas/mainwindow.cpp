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

	ui->query->setPlainText(fileOperations->GetLastQuery());
    UpdateLastUsedMenu();

    QSettings settings;
    bool autoUpdate = settings.value("/settings/autoUpdate").toBool();
    ui->actionAuto_update->setChecked(autoUpdate);

	connect(fileOperations, SIGNAL(UpdateLastUsed()), this, SLOT(UpdateLastUsedMenu()));
    //connect(ui->stringIn, SIGNAL(updateRequest()), this, SLOT(on_pushButton_clicked()));
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
#ifdef Q_OS_UNIX
		name = item.section('/', -1);
#else
		name = item.section('\\', -1);
#endif

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

void MainWindow::on_pushButton_Go_clicked()
{
	fileOperations->SetLastQuery(ui->query->toPlainText());

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
	fileOperations->SaveToFile(ui->query->toPlainText());
}

void MainWindow::on_actionLoad_triggered()
{
	QString string = fileOperations->LoadFromFile();

	if(!string.isNull())
		ui->query->setPlainText(string);
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
