#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerjavascript.h>

#include "presentation/fileloadsave.h"
#include "presentation/queryexecution.h"
#include "model/qtabdocument.h"

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
	Ui::MainWindow *_ui;

	FileLoadSave* _fileOperations;
	QueryExecution *_queryExecution;
	QTabDocument * _tabDocument;
	QsciScintilla* _queryEditor;
	bool _isContructionInProgress;

private:

	void loadQueryToGUI(QString _query);
	void loadFileToGUI(QString fileName);
	void setCurretTabDocument(QTabDocument * doc);
	void loadSession();


protected:
	virtual void closeEvent(QCloseEvent *);

private slots:
	void onClosing();
	void onQueryChanged();
	void tabChanged(int index);
	void updateLastUsedMenu();
	void onOpenLastUsed();
	void on_actionAuto_update_triggered();
    void on_pushButton_clicked();
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void on_pushButton_Go_clicked();
    void on_actionExecute_triggered();
	void on_actionZoom_In_triggered();
	void on_actionZoom_Out_triggered();
	void on_actionDebugger_triggered();
	void on_actionSave_As_triggered();
	void on_actionRestore_last_query_triggered();
};

#endif // MAINWINDOW_H
