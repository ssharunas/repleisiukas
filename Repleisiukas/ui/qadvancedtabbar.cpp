#include "qadvancedtabbar.h"
#include <QVariant>
#include <QDebug>

static unsigned int tab_document_count = 0;

QAdvancedTabBar::QAdvancedTabBar(QWidget *parent) :
	QTabBar(parent), _trashButton(0)
{
	_trashMenu = new QMenu(this);
	_lastAction = _trashMenu->addSeparator();
	_trashMenu->addAction("Valyti", this, SLOT(emptyTrash()));

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

	if(_documents.contains(currentDocumentUID))
		doc = _documents[currentDocumentUID];

	return doc;
}

QTabDocument* QAdvancedTabBar::getTabDocument(int tab)
{
	QTabDocument* doc = 0;

	if(tab >= 0 && tab < count()){
		QString currentDocumentUID = tabData(tab).toString();

		if(_documents.contains(currentDocumentUID))
			doc = _documents[currentDocumentUID];
		else
			qDebug() << "Could not find document with id " << currentDocumentUID;
	}

	return doc;
}

QTabDocument* QAdvancedTabBar::createDocument()
{
	QTabDocument* doc = new QTabDocument(this);
	doc->setName(QString("New %1").arg(tab_document_count++));
	connect(doc, SIGNAL(changed()), this, SLOT(updateTab()));

	_documents[doc->uid()] = doc;

	return doc;
}

void QAdvancedTabBar::openDocument(QTabDocument* document)
{
	if(document != 0)
	{
		if(!_documents.contains(document->uid()))
		{
			_documents[document->uid()] = document;
			connect(document, SIGNAL(changed()), this, SLOT(updateTab()));
		}

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
			index = addTab(document->tabName());
			setTabData(index, QVariant(document->uid()));
		}

		setCurrentIndex(index);
	}
}

void QAdvancedTabBar::createNewTab()
{
	QTabDocument* doc = createDocument();
	int index = addTab(doc->tabName());
	setTabData(index, QVariant(doc->uid()));

	if(currentIndex() != index){
		setCurrentIndex(index);
	}else{
		currentChanged(index);
	}
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
	_trashButton = button;

	if(_trashButton != 0)
	{
		_trashButton->setMenu(_trashMenu);
	}
}

void QAdvancedTabBar::emptyTrash()
{
	if(_trash.isEmpty())
		return;

	QAction* action = 0;
	while(!_trash.isEmpty())
	{
		action = _trash.takeLast();
		QString uid = action->data().toString();
		_trashMenu->removeAction(action);

		QTabDocument* doc = 0;

		if(_documents.contains(uid))
			doc = _documents.take(uid);

		if(doc != 0)
			delete doc;

		delete action;
		action = 0;
	}

	_lastAction = _trashMenu->actions().first();
}

void QAdvancedTabBar::closeTab(int index)
{
	if(index != -1)
	{
		QTabDocument* doc = 0;
		QString currentDocumentUID = tabData(index).toString();

		if(_documents.contains(currentDocumentUID))
			doc = _documents[currentDocumentUID];

		removeTab(index);

		if(doc != 0)
		{
			QAction *action = new QAction(doc->name(), _trashMenu);
			action->setData(QVariant(doc->uid()));
			connect(action, SIGNAL(triggered()), this, SLOT(restoreTab()));

			_trash.append(action);
			_trashMenu->insertAction(_lastAction, action);
			_lastAction = action;
		}
	}

	if(count() <= 0)
		createNewTab();
}

void QAdvancedTabBar::restoreTab()
{
	QAction *action = dynamic_cast<QAction*>(sender());
	if(action != 0)
	{
		QString uid = action->data().toString();
		QTabDocument * doc = 0;

		if(_documents.contains(uid))
			doc = _documents[uid];

		if(doc != 0)
		{
			openDocument(doc);
		}

		if(_trash.contains(action))
			_trash.removeAll(action);

		_trashMenu->removeAction(action);
		_lastAction = _trashMenu->actions().first();

		action->deleteLater();
	}
}

void QAdvancedTabBar::updateTab()
{
	QTabDocument* doc =  qobject_cast<QTabDocument*>(sender());
	if(doc != 0)
	{
		int index = -1;
		for(int i = 0; i < count(); i++)
		{
			if(tabData(i) == doc->uid())
			{
				index = i;
				break;
			}
		}

		if(index != -1)
		{
			setTabText(index, doc->tabName());
		}
	}
}

void QAdvancedTabBar::loadDocuments(QList<QTabDocument*> documents)
{
	if(documents.count() > 0)
	{
		this->_documents.clear();

		while(count())
			removeTab(0);

		for(int i = 0; i < documents.count(); i++)
		{
			openDocument(documents[i]);
		}

		currentChanged(currentIndex());
	}
}
