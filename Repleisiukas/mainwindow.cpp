#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include <QShortcut>
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	QCoreApplication::setOrganizationName("SoftDent");
	QCoreApplication::setOrganizationDomain("softdent.lt");
	QCoreApplication::setApplicationName("Repleisiukas");

	fileOperations = new FileLoadSave(this);
	queryExecution = new QueryExecution(fileOperations, this);
	ui->setupUi(this);
	highlighter = new JSHighlighter(ui->query->document());

	ui->query->addAction(ui->actionExecute);
	ui->query->setPlainText(fileOperations->GetLastQuery());
	UpdateLastUsedMenu();

	QSettings settings;
	bool autoUpdate = settings.value("/settings/autoUpdate").toBool();
	ui->actionAuto_update->setChecked(autoUpdate);

        QShortcut* shortcut = new QShortcut(QKeySequence(tr("Ctrl+Return")), ui->query);
        connect(shortcut, SIGNAL(activated()), ui->actionExecute, SLOT(trigger()));

	connect(fileOperations, SIGNAL(UpdateLastUsed()), this, SLOT(UpdateLastUsedMenu()));
	connect(ui->stringIn, SIGNAL(updateRequest()), this, SLOT(on_pushButton_Go_clicked()));

	ui->tabs->createNewTab();
	tabDocument = ui->tabs->getCurrentDocument();

	connect(ui->tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
	ui->tabs->setTrashButton(ui->trashButton);

	fileOperations->getSavedSession();
}

MainWindow::~MainWindow()
{
	delete highlighter;
	delete ui;
}

void MainWindow::tabChanged(int)
{
	setCurretTabDocument(ui->tabs->getCurrentDocument());
}

void MainWindow::setCurretTabDocument(QTabDocument * doc){
	if(tabDocument != 0)
	{
		//tabDocument->setFileName();
//		tabDocument->setName();
		tabDocument->setInput(ui->stringIn->toPlainText());
		tabDocument->setQuery(ui->query->toPlainText());
		tabDocument->setOutput(ui->stringOut->toPlainText());
	}

	tabDocument = doc;
	if(tabDocument != 0)
	{
		ui->stringIn->setText(tabDocument->input());
		ui->query->setText(tabDocument->query());
		ui->stringOut->setPlainText(tabDocument->output());
	}
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

		LoadQueryToGUI(fileOperations->LoadFromFile(file));
	}
}

void MainWindow::LoadQueryToGUI(QString query)
{
	if(!query.isNull())
	{
		ui->query->setPlainText(query);

		QString autoload = fileOperations->GetAutoLoadText(query);
		if(!autoload.isNull())
		{
			ui->stringIn->setText(autoload);
			ui->stringIn->Dialogize();
		}
	}
}

void MainWindow::on_pushButton_Go_clicked()
{
	fileOperations->SetLastQuery(ui->query->toPlainText());
        QString result = queryExecution->Execute(ui->query->toPlainText(), ui->stringIn->text());
        ui->stringOut->setPlainText(result);
}

void MainWindow::on_actionSave_triggered()
{
	fileOperations->SaveToFile(ui->query->toPlainText());
}

void MainWindow::on_actionLoad_triggered()
{
	LoadQueryToGUI(fileOperations->LoadFromFile());
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
//	QString dir = QFileDialog::getExistingDirectory(this);

//	if(!dir.isEmpty())
//	{
//		if(!(dir.endsWith('/') || dir.endsWith('\\')))
//			dir.append("/");
//		dir.append("*");

//		ui->inFiles->addItem(dir);
//	}
}

void MainWindow::on_actionZoom_In_triggered()
{
	ui->query->zoomIn();
}

void MainWindow::on_actionZoom_Out_triggered()
{
	ui->query->zoomOut();
}

void MainWindow::onClosing()
{
	//TODO: save current document
	QList<QTabDocument*> documents;
	for(int i = 0; i < ui->tabs->count(); i++){
		documents.append(ui->tabs->getTabDocument(i));
	}

	fileOperations->saveCurrentSession(documents);
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
	onClosing();

	QMainWindow::closeEvent(ev);
}
