#include "qadvancedtabbar.h"
#include <QVariant>
#include <QDebug>

static unsigned int tab_document_count = 0;

QAdvancedTabBar::QAdvancedTabBar(QWidget *parent) :
	QTabBar(parent), trashButton(0)
{
	trashMenu = new QMenu(this);
	lastAction = trashMenu->addSeparator();
	trashMenu->addAction("Valyti", this, SLOT(emptyTrash()));

	setExpanding(false);
	setDocumentMode(true);
	setTabsClosable(true);
	setMovable(true);

	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

QTabDocument* QAdvancedTabBar::getCurrentDocument()
{
	QTabDocument* doc = 0;

	QString currentDocumentUID = tabData(currentIndex()).toString();

	if(documents.contains(currentDocumentUID))
		doc = documents[currentDocumentUID];

	return doc;
}

QTabDocument* QAdvancedTabBar::createDocument()
{
	QTabDocument* doc = new QTabDocument(this);
	doc->setName(QString("New %1").arg(tab_document_count++));

	documents[doc->uid()] = doc;

	return doc;
}

void QAdvancedTabBar::openDocument(QTabDocument* document)
{
	if(document != 0)
	{
		if(documents.contains(document->uid()))
			documents[document->uid()] = document;

		int index = -1;
		for(int i = 0; i < count(); i++)
		{
			if(tabData(i) == document->uid())
			{
				index = i;
				break;
			}
		}

		if(index == -1)
		{
			index = addTab(document->name());
			setTabData(index, QVariant(document->uid()));
		}

		setCurrentIndex(index);
	}
}

void QAdvancedTabBar::createNewTab()
{
	QTabDocument* doc = createDocument();
	int index = addTab(doc->name());
	setTabData(index, QVariant(doc->uid()));
	setCurrentIndex(index);
}

void QAdvancedTabBar::mouseDoubleClickEvent(QMouseEvent * evt){
	int index = tabAt(evt->pos());

	if(index == -1)
	{
		createNewTab();
	}
}

void QAdvancedTabBar::setTrashButton(QPushButton *button)
{
	trashButton = button;

	if(trashButton != 0)
	{
		trashButton->setMenu(trashMenu);
	}
}

void QAdvancedTabBar::emptyTrash()
{
	if(trash.isEmpty())
		return;

	QAction* action = 0;
	while(!trash.isEmpty())
	{
		action = trash.takeLast();
		QString uid = action->data().toString();
		trashMenu->removeAction(action);

		QTabDocument* doc = 0;

		if(documents.contains(uid))
			doc = documents.take(uid);

		if(doc != 0)
			delete doc;

		delete action;
		action = 0;
	}

	lastAction = trashMenu->actions().first();
}

void QAdvancedTabBar::closeTab(int index)
{
	if(index != -1)
	{
		QTabDocument* doc = 0;
		QString currentDocumentUID = tabData(index).toString();

		if(documents.contains(currentDocumentUID))
			doc = documents[currentDocumentUID];

		removeTab(index);

		if(doc != 0)
		{
			QAction *action = new QAction(doc->name(), trashMenu);
			action->setData(QVariant(doc->uid()));
			connect(action, SIGNAL(triggered()), this, SLOT(restoreTab()));

			trash.append(action);
			trashMenu->insertAction(lastAction, action);
			lastAction = action;
		}
	}
}

void QAdvancedTabBar::restoreTab()
{
	QAction *action = dynamic_cast<QAction*>(sender());
	if(action != 0)
	{
		QString uid = action->data().toString();
		QTabDocument * doc = 0;

		if(documents.contains(uid))
			doc = documents[uid];

		if(doc != 0)
		{
			openDocument(doc);
		}

		if(trash.contains(action))
			trash.removeAll(action);

		trashMenu->removeAction(action);
		lastAction = trashMenu->actions().first();

		action->deleteLater();
	}
}
