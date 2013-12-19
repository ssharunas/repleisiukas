#ifndef QADVANCEDTABBAR_H
#define QADVANCEDTABBAR_H

#include <QTabBar>
#include <QHash>
#include <QMouseEvent>
#include <QPushButton>
#include <QMenu>

#include "../model/qtabdocument.h"

class QAdvancedTabBar : public QTabBar
{
		Q_OBJECT
	public:
		explicit QAdvancedTabBar(QWidget *parent = 0);

		QTabDocument* getCurrentDocument();
		QTabDocument* getTabDocument(int tab);
		void setTrashButton(QPushButton* button);
		void loadDocuments(QList<QTabDocument*> _documents);
	signals:

	public slots:
		void openDocument(QTabDocument* document);
		void createNewTab();
		void emptyTrash();

	protected:
		virtual void mouseDoubleClickEvent(QMouseEvent *);

	private:
		QPushButton* _trashButton;
		QMenu* _trashMenu;
		QHash<QString, QTabDocument*> _documents;
		QList<QAction*> _trash;

		QAction* _lastAction;

	private:
		QTabDocument* createDocument();

	private slots:
		void closeTab(int index);
		void restoreTab();
		void updateTab();
};

#endif // QADVANCEDTABBAR_H
