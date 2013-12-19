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
	_ui(new Ui::MainWindow),
	_queryEditor(0)
{
	_isContructionInProgress = true;
	QCoreApplication::setOrganizationName("SoftDent");
	QCoreApplication::setOrganizationDomain("softdent.lt");
	QCoreApplication::setApplicationName("Repleisiukas");

	_fileOperations = new FileLoadSave(this);
	_queryExecution = new QueryExecution(_fileOperations, this);
	_ui->setupUi(this);
	updateLastUsedMenu();

	QSettings settings;
	bool autoUpdate = settings.value("/settings/autoUpdate").toBool();
	_ui->actionAuto_update->setChecked(autoUpdate);

	bool debugger = settings.value("/settings/debugger").toBool();
	_ui->actionDebugger->setChecked(debugger);

	_ui->tabs->createNewTab();
	_tabDocument = _ui->tabs->getCurrentDocument();

	connect(_ui->tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
	_ui->tabs->setTrashButton(_ui->trashButton);

	_queryEditor = new QsciScintilla;
	_ui->splitter->addWidget(_queryEditor);

	QsciLexerJavaScript* lexer = new QsciLexerJavaScript();
	lexer->setHighlightTripleQuotedStrings(true);
	_queryEditor->setLexer(lexer);
	_queryEditor->setAutoIndent(true);
	_queryEditor->setBraceMatching(QsciScintilla::SloppyBraceMatch);
	_queryEditor->setFolding(QsciScintilla::BoxedTreeFoldStyle);
	_queryEditor->setWrapMode(QsciScintilla::WrapWord);
	_queryEditor->setUtf8(true);

	QShortcut* shortcut = new QShortcut(QKeySequence(tr("Ctrl+Return")), _queryEditor);
	connect(shortcut, SIGNAL(activated()), _ui->actionExecute, SLOT(trigger()));

	shortcut = new QShortcut(QKeySequence(tr("Ctrl+Enter")), _queryEditor);
	connect(shortcut, SIGNAL(activated()), _ui->actionExecute, SLOT(trigger()));

	shortcut = new QShortcut(QKeySequence(tr("Ctrl+Shift+z")), _queryEditor);
	connect(shortcut, SIGNAL(activated()), _queryEditor, SLOT(redo()));

	loadSession();

	connect(_queryEditor, SIGNAL(textChanged()), this, SLOT(onQueryChanged()));
	connect(_fileOperations, SIGNAL(lastUsedUpdated()), this, SLOT(updateLastUsedMenu()));
	connect(_ui->stringIn, SIGNAL(updateRequest()), this, SLOT(on_pushButton_Go_clicked()));

	_isContructionInProgress = false;
}

MainWindow::~MainWindow()
{
	delete _ui;
}

void MainWindow::loadSession()
{
	QList<QTabDocument*> documents = _fileOperations->getSavedSession();

	_ui->tabs->loadDocuments(documents);
}

void MainWindow::tabChanged(int)
{
	setCurretTabDocument(_ui->tabs->getCurrentDocument());
}

void MainWindow::setCurretTabDocument(QTabDocument * doc){
	if(_tabDocument != 0)
	{
		int line = 0, index = 0;
		_queryEditor->getCursorPosition(&line, &index);
		_tabDocument->setQueryCursor(QPoint(line, index));

		if(_queryEditor->verticalScrollBar())
			_tabDocument->setQueryScrollPos(_queryEditor->verticalScrollBar()->value());

		_tabDocument->setInput(_ui->stringIn->toPlainText());
		_tabDocument->setQuery(_queryEditor->text());
		_tabDocument->setOutput(_ui->stringOut->toPlainText());
	}

	_tabDocument = doc;
	if(_tabDocument != 0)
	{
		_ui->stringIn->setText(_tabDocument->input());
		_queryEditor->setText(_tabDocument->query());
		_ui->stringOut->setPlainText(_tabDocument->output());

		_queryEditor->setCursorPosition(_tabDocument->queryCursor().x(), _tabDocument->queryCursor().y());
		if(_queryEditor->verticalScrollBar())
			_queryEditor->verticalScrollBar()->setValue(_tabDocument->queryScrollPos());
	}
}

void MainWindow::updateLastUsedMenu()
{
	_ui->menuLastUsed->clear();
	QStringList items = _fileOperations->getLastUsedMenuEntries();

	foreach (QString item, items)
	{
		QString name;
		name = item.section('/', -1);

		_ui->menuLastUsed->addAction(name, this, SLOT(onOpenLastUsed()))
				->setData(QVariant(item));
	}
}

void MainWindow::loadFileToGUI(QString fileName){
	if(!fileName.isEmpty())
	{
		loadQueryToGUI(_fileOperations->loadFromFile(fileName));

		if(_tabDocument != 0)
		{
			_tabDocument->setFileName(fileName);
			_tabDocument->setIsModified(false);
		}
	}
}

void MainWindow::loadQueryToGUI(QString _query)
{
	if(!_query.isNull())
	{
		this->_queryEditor->setText(_query);

		QString autoload = _fileOperations->getAutoLoadText(_query);
		if(!autoload.isNull())
		{
			_ui->stringIn->setText(autoload);
			_ui->stringIn->dialogize();
		}
	}
}

void MainWindow::onOpenLastUsed()
{
	QAction *senderAction = dynamic_cast<QAction*>(sender());

	if(senderAction)
	{
		QString file = senderAction->data().toString();
		loadFileToGUI(file);
	}
}

void MainWindow::on_pushButton_Go_clicked()
{
	if(!_isContructionInProgress)
	{
		_fileOperations->setLastQuery(_queryEditor->text());
			QString result = _queryExecution->execute(_queryEditor->text(), _ui->stringIn->text());
			_ui->stringOut->setPlainText(result);
	}
}

void MainWindow::on_actionSave_triggered()
{
	if(_tabDocument != 0)
	{
		if(!_tabDocument->fileName().isEmpty())
		{
			_fileOperations->saveToFile(_tabDocument->fileName(), _queryEditor->text());
			_tabDocument->setQuery(_queryEditor->text());
			_tabDocument->setIsModified(false);
		}else
		{
			on_actionSave_As_triggered();
		}
	}
}

void MainWindow::on_actionSave_As_triggered()
{
	QString fileName = _fileOperations->saveToFile(_queryEditor->text());
	if(!fileName.isEmpty())
	{
		if(_tabDocument != 0)
		{
			_tabDocument->setFileName(fileName);
			_tabDocument->setQuery(_queryEditor->text());
			_tabDocument->setIsModified(false);
		}
	}
}

void MainWindow::on_actionLoad_triggered()
{
	QString fileName = _fileOperations->getLoadFromFileFilename();
	loadFileToGUI(fileName);
}

void MainWindow::on_pushButton_clicked()
{
	_ui->stringIn->clear();
	_ui->stringIn->paste();
	on_pushButton_Go_clicked();
	_ui->stringOut->selectAll();
	_ui->stringOut->copy();
}

void MainWindow::on_actionAuto_update_triggered()
{
	QSettings settings;
	settings.setValue("/settings/autoUpdate", QVariant(_ui->actionAuto_update->isChecked()));
}

void MainWindow::on_actionExecute_triggered()
{
	on_pushButton_Go_clicked();
}

void MainWindow::on_actionZoom_In_triggered()
{
	_queryEditor->zoomIn();
}

void MainWindow::on_actionZoom_Out_triggered()
{
	_queryEditor->zoomOut();
}

void MainWindow::on_actionDebugger_triggered()
{
	QSettings settings;
	settings.setValue("/settings/debugger", QVariant(_ui->actionDebugger->isChecked()));
}

void MainWindow::onClosing()
{
	setCurretTabDocument(_tabDocument);

	QList<QTabDocument*> documents;

	for(int i = 0; i < _ui->tabs->count(); i++){
		documents.append(_ui->tabs->getTabDocument(i));
	}

	_fileOperations->saveCurrentSession(documents);
}

void MainWindow::onQueryChanged()
{
	if(!_isContructionInProgress && _tabDocument != 0)
	{
		_tabDocument->setIsModified(_queryEditor->text() != _tabDocument->query());
	}
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
	onClosing();

	QMainWindow::closeEvent(ev);
}

void MainWindow::on_actionRestore_last_query_triggered()
{
	QTabDocument *last = new QTabDocument(_ui->tabs);
	last->setName("Restored");
	last->setIsModified(true);
	last->setQuery(_fileOperations->lastQuery());

	_ui->tabs->openDocument(last);
}
