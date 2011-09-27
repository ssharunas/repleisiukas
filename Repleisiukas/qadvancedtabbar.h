#ifndef QADVANCEDTABBAR_H
#define QADVANCEDTABBAR_H

#include <QTabBar>
#include <QHash>
#include <QMouseEvent>
#include "model/qtabdocument.h"
#include <QPushButton>
#include <QMenu>

class QAdvancedTabBar : public QTabBar
{
		Q_OBJECT
	public:
		explicit QAdvancedTabBar(QWidget *parent = 0);

		QTabDocument* getCurrentDocument();
		QTabDocument* getTabDocument(int tab);
		void setTrashButton(QPushButton* button);
	signals:

	public slots:
		void openDocument(QTabDocument* document);
		void createNewTab();
		void emptyTrash();

	protected:
		virtual void mouseDoubleClickEvent(QMouseEvent *);

	private:
		QPushButton* trashButton;
		QMenu* trashMenu;
		QHash<QString, QTabDocument*> documents;
		QList<QAction*> trash;

		QAction* lastAction;

	private:
		QTabDocument* createDocument();

	private slots:
		void closeTab(int index);
		void restoreTab();
};

#endif // QADVANCEDTABBAR_H
