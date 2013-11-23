#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include <QShortcut>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	query(0)
{
	contructionInProgress = true;
	QCoreApplication::setOrganizationName("SoftDent");
	QCoreApplication::setOrganizationDomain("softdent.lt");
	QCoreApplication::setApplicationName("Repleisiukas");

	fileOperations = new FileLoadSave(this);
	queryExecution = new QueryExecution(fileOperations, this);
	ui->setupUi(this);
//	highlighter = new JSHighlighter(ui->query->document());

//	ui->query->addAction(ui->actionExecute);
//	ui->query->setPlainText(fileOperations->GetLastQuery());
	UpdateLastUsedMenu();

	QSettings settings;
	bool autoUpdate = settings.value("/settings/autoUpdate").toBool();
	ui->actionAuto_update->setChecked(autoUpdate);

	bool debugger = settings.value("/settings/debugger").toBool();
	ui->actionDebugger->setChecked(debugger);

	ui->tabs->createNewTab();
	tabDocument = ui->tabs->getCurrentDocument();

	connect(ui->tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
	ui->tabs->setTrashButton(ui->trashButton);

	query = new QsciScintilla;
	ui->splitter->addWidget(query);

	QsciLexerJavaScript* lexer = new QsciLexerJavaScript();
	lexer->setHighlightTripleQuotedStrings(true);
	query->setLexer(lexer);
	query->setAutoIndent(true);
	query->setBraceMatching(QsciScintilla::SloppyBraceMatch);
	query->setFolding(QsciScintilla::BoxedTreeFoldStyle);
	query->setWrapMode(QsciScintilla::WrapWord);
	query->setUtf8(true);

	QShortcut* shortcut = new QShortcut(QKeySequence(tr("Ctrl+Return")), query);
	connect(shortcut, SIGNAL(activated()), ui->actionExecute, SLOT(trigger()));

	shortcut = new QShortcut(QKeySequence(tr("Ctrl+Enter")), query);
	connect(shortcut, SIGNAL(activated()), ui->actionExecute, SLOT(trigger()));

	shortcut = new QShortcut(QKeySequence(tr("Ctrl+Shift+z")), query);
	connect(shortcut, SIGNAL(activated()), query, SLOT(redo()));

	LoadSession();

	connect(query, SIGNAL(textChanged()), this, SLOT(onQueryChanged()));
	connect(fileOperations, SIGNAL(UpdateLastUsed()), this, SLOT(UpdateLastUsedMenu()));
	connect(ui->stringIn, SIGNAL(updateRequest()), this, SLOT(on_pushButton_Go_clicked()));

	contructionInProgress = false;
}

MainWindow::~MainWindow()
{
//	delete highlighter;
	delete ui;
}


void MainWindow::LoadSession()
{
	QList<QTabDocument*> documents = fileOperations->getSavedSession();

	ui->tabs->loadDocuments(documents);
}

void MainWindow::tabChanged(int)
{
	setCurretTabDocument(ui->tabs->getCurrentDocument());
}

void MainWindow::setCurretTabDocument(QTabDocument * doc){
	if(tabDocument != 0)
	{
		int line = 0, index = 0;
		query->getCursorPosition(&line, &index);
		tabDocument->setQueryCursor(QPoint(line, index));

		if(query->verticalScrollBar())
			tabDocument->setQueryScrollPos(query->verticalScrollBar()->value());

		tabDocument->setInput(ui->stringIn->toPlainText());
		tabDocument->setQuery(query->text());
		tabDocument->setOutput(ui->stringOut->toPlainText());
	}

	tabDocument = doc;
	if(tabDocument != 0)
	{
		ui->stringIn->setText(tabDocument->input());
		query->setText(tabDocument->query());
		ui->stringOut->setPlainText(tabDocument->output());

		query->setCursorPosition(tabDocument->queryCursor().x(), tabDocument->queryCursor().y());
		if(query->verticalScrollBar())
			query->verticalScrollBar()->setValue(tabDocument->queryScrollPos());
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

	if(senderAction)
	{
		QString file = senderAction->data().toString();
		LoadQueryToGUI(fileOperations->LoadFromFile(file));

		if(tabDocument != 0)
		{
			tabDocument->setFileName(file);
			tabDocument->setQuery(query->text());
			tabDocument->setIsModified(false);
		}
	}
}

void MainWindow::LoadQueryToGUI(QString query)
{
	if(!query.isNull())
	{
		this->query->setText(query);

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
	if(!contructionInProgress)
	{
		fileOperations->SetLastQuery(query->text());
			QString result = queryExecution->Execute(query->text(), ui->stringIn->text());
			ui->stringOut->setPlainText(result);
	}
}

void MainWindow::on_actionSave_triggered()
{
	if(tabDocument != 0)
	{
		if(!tabDocument->fileName().isEmpty())
		{
			fileOperations->SaveToFile(tabDocument->fileName(), query->text());
			tabDocument->setQuery(query->text());
			tabDocument->setIsModified(false);
		}else
		{
			on_actionSave_As_triggered();
		}
	}
}

void MainWindow::on_actionSave_As_triggered()
{
	QString fileName = fileOperations->SaveToFile(query->text());
	if(!fileName.isEmpty())
	{
		if(tabDocument != 0)
		{
			tabDocument->setFileName(fileName);
			tabDocument->setQuery(query->text());
			tabDocument->setIsModified(false);
		}
	}
}

void MainWindow::on_actionLoad_triggered()
{
	QString fileName = fileOperations->GetLoadFromFileFilename();
	if(!fileName.isEmpty())
	{
		if(tabDocument != 0)
		{
			tabDocument->setFileName(fileName);
			tabDocument->setIsModified(false);
		}

		LoadQueryToGUI(fileOperations->LoadFromFile(fileName));
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
	query->zoomIn();
}

void MainWindow::on_actionZoom_Out_triggered()
{
	query->zoomOut();
}

void MainWindow::onClosing()
{
	setCurretTabDocument(tabDocument);

	QList<QTabDocument*> documents;

	for(int i = 0; i < ui->tabs->count(); i++){
		documents.append(ui->tabs->getTabDocument(i));
	}

	fileOperations->saveCurrentSession(documents);
}

void MainWindow::onQueryChanged()
{
	if(!contructionInProgress && tabDocument != 0)
	{
		tabDocument->setIsModified(query->text() != tabDocument->query());
	}
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
	onClosing();

	QMainWindow::closeEvent(ev);
}

	void MainWindow::on_actionDebugger_triggered()
{
	QSettings settings;
	settings.setValue("/settings/debugger", QVariant(ui->actionDebugger->isChecked()));
}
