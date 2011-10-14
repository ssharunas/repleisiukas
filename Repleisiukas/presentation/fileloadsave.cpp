#include "fileloadsave.h"
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QDebug>

FileLoadSave::FileLoadSave(QWidget *parent) :
	QObject(parent)
{
	DEFAULT_MENU_ITEMS_COUNT = 10;
	strem = 0;

	QSettings settings;
	QString tempFilename;

#ifdef Q_OS_UNIX
	tempFilename = settings.value("/files/tempFile", QVariant("lastQuery")).toString();
#else
	tempFilename = settings.value("/files/tempFile", QVariant("d:\\last.txt")).toString();
#endif

	last = new QFile(tempFilename);
	if (last->open(QIODevice::ReadWrite | QIODevice::Text))
	{
		strem = new QTextStream(last);
		strem->setCodec("UTF-8");
	}
	else
	{
		last = 0;
	}
}

FileLoadSave::~FileLoadSave()
{
	if(last != 0){
		last->close();
		if(strem != 0)
			delete strem;
		delete last;
	}
}

QWidget* FileLoadSave::parent() const
{
	return dynamic_cast<QWidget*>(QObject::parent());
}

QString FileLoadSave::GetLastQuery()
{
	QString result;

	if(strem != 0)
	{
		result = strem->readAll();
	}


	return result;
}

void FileLoadSave::SetLastQuery(QString query)
{
	if(last != 0 && strem != 0)
	{
		last->resize(0);

		*strem << query;
		strem->flush();
	}
}

QString FileLoadSave::LoadFromFile()
{
	QSettings settings;
	QString path =  settings.value("/files/lastSaveDir").toString();
	qDebug() << path;
	QString fileName = QFileDialog::getOpenFileName(parent(), QString(), path);

	if(!fileName.isEmpty())
	{
		path = fileName.section('/', 0, -2);
		settings.setValue("/files/lastSaveDir", QVariant(path));
		qDebug() << "saved" << path << fileName;
		return LoadFromFile(fileName);
	}

	return QString();
}

QString FileLoadSave::LoadFromFile(QString filename)
{
	QString result = QString();

	if(!filename.isEmpty())
	{
		QFile file(filename);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::warning(parent(),
								 "Could not open file", "could not open file for reading.");
		}else
		{
			QTextStream stream(&file);
			stream.setCodec("UTF-8");
			result = stream.readAll();
			UpdateLastUsedOrder(filename);
		}
	}

	return result;
}

unsigned int FileLoadSave::MenuItemCount()
{
	QSettings settings;
	return settings.value("/files/recentCount", QVariant(DEFAULT_MENU_ITEMS_COUNT)).toInt();
}

void FileLoadSave::UpdateLastUsedOrder(QString newName)
{
	QSettings settings;
	int menuItemCount = MenuItemCount();
	QStringList settingsToAdd;
	settingsToAdd.reserve(menuItemCount);

	settingsToAdd.append(newName);

	for(int i = 0; i < menuItemCount - 1; i++)
	{
		QString item = QString("/files/load/%1").arg(i);
		QString shitValue = settings.value(item).toString();

		if(!settingsToAdd.contains(shitValue))
			settingsToAdd.append(shitValue);
	}

	for(int i = 0; i < settingsToAdd.size(); i++)
	{
		QString item = QString("/files/load/%1").arg(i);
		settings.setValue(item, settingsToAdd[i]);
	}

	emit UpdateLastUsed();
}

QStringList FileLoadSave::GetLastUsedMenuEntries()
{
	QStringList result;
	QSettings settings;

	for(unsigned int i = 0; i < MenuItemCount(); i++)
	{
		QString item = settings.value(QString("/files/load/%1").arg(i)).toString();

		if(item.isEmpty())
			break;

		result.append(item);
	}

	return result;
}


void FileLoadSave::SaveToFile(QString query)
{
	QSettings settings;
	QString path =  settings.value("/files/lasLoadDir").toString();
	QString fileName = QFileDialog::getSaveFileName(parent(), QString(), path);

	if(!fileName.isEmpty())
	{
		path = fileName.section('/', 0, -2);
		settings.setValue("/files/lasLoadDir", QVariant(path));

		SaveToFile(fileName, query);
	}
}

void FileLoadSave::SaveToFile(QString fileName, QString query)
{
	if(!fileName.isEmpty())
	{
		QFile file(fileName);

		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QMessageBox::warning(parent(),
								 "Could not open file", "Could not open file for writing.");
		}

		QTextStream stream(&file);
		stream.setCodec("UTF-8");
		stream << query;
		UpdateLastUsedOrder(fileName);
	}
}

QString FileLoadSave::LoadResource(QString path){
	QFile resourceFile(path);
	resourceFile.open(QIODevice::ReadOnly);
	QTextStream stream(&resourceFile);
	stream.setCodec("UTF-8");
	return stream.readAll();
}

QString FileLoadSave::GetAutoLoadText(QString script){
	QString result;
	const char * START_TAG = "--<autoload>--";
	const char * END_TAG = "--</autoload>--";

	int start = script.indexOf(START_TAG);
	if(start != -1)
	{
		start += strlen(START_TAG);
		int end = script.indexOf(END_TAG, start);

		if(end != -1 && end > start)
		{
			result = script.mid(start, end - start);
		}
	}

	return result;
}

void FileLoadSave::saveCurrentSession(QList<QTabDocument *> &sessionDocuments)
{
	QList<QList<QString> > data;

	foreach (QTabDocument* doc, sessionDocuments)
	{
		if(doc != 0){
			QList<QString> item;
			item << doc->input()
				 << doc->query()
				 << doc->output()
				 << doc->name()
				 << doc->fileName();

			data.append(item);
		}
		else
		{
			qDebug("NULL document while saving...");
		}
	}

	QSettings settings;
	QString fileName = settings.value("/files/sessionfile", QVariant("session.dat")).toString();

	QFile file(fileName);
	if(file.open(QIODevice::WriteOnly))
	{
		QDataStream out(&file);

		out << (qint32)1; //version
		out << data;
	}else{
		qDebug() << "Failed to write to file: " << fileName;
	}
}

QList<QTabDocument *> FileLoadSave::getSavedSession()
{
	QList<QTabDocument *> result;
	QList<QList<QString> > data;
	qint32 version = -1;

	QSettings settings;
	QString fileName = settings.value("/files/sessionfile", QVariant("session.dat")).toString();

	QFile file(fileName);
	if(file.open(QIODevice::ReadOnly))
	{
		QDataStream in(&file);
		in >> version;

		if(version == 1){
			in >> data;

			foreach (QList<QString> item, data)
			{
				QTabDocument *doc = new QTabDocument();

				doc->setInput(item[0]);
				doc->setQuery(item[1]);
				doc->setOutput(item[2]);
				doc->setName(item[3]);
				doc->setFileName(item[4]);

				result.append(doc);
			}
		}

		qDebug() << version << data;
	}else{
		qDebug() << "Failed to open session file: " << fileName;
	}

	return result;
}
