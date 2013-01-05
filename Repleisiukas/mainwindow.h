#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui/jshighlighter.h"
#include "presentation/fileloadsave.h"
#include "presentation/queryexecution.h"
#include "model/qtabdocument.h"

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerjavascript.h>

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

//    JSHighlighter *highlighter;

    FileLoadSave* fileOperations;
	QueryExecution *queryExecution;
	QTabDocument * tabDocument;
	QsciScintilla* query;
	bool contructionInProgress;

private:

	void LoadQueryToGUI(QString query);
	void setCurretTabDocument(QTabDocument * doc);
	void LoadSession();

protected:
	virtual void closeEvent(QCloseEvent *);

private slots:
	void onClosing();
	void onQueryChanged();
	void tabChanged(int index);
    void on_pushButton_2_clicked();
    void UpdateLastUsedMenu();
    void on_actionAuto_update_triggered();
    void on_openFile_clicked();
    void on_pushButton_clicked();
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void on_pushButton_Go_clicked();
    void on_actionExecute_triggered();
	void on_actionZoom_In_triggered();
	void on_actionZoom_Out_triggered();
	void on_actionDebugger_triggered();
	void on_actionSave_As_triggered();
};

#endif // MAINWINDOW_H
