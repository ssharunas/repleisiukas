#include "fileloadsave.h"
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QDebug>
#include <QStandardPaths>

const unsigned int DEFAULT_MENU_ITEMS_COUNT = 10;

FileLoadSave::FileLoadSave(QWidget *parent) :
	QObject(parent)
{
	_strem = 0;

	QSettings settings;
	QString tempFilename = settings.value("/files/tempFile", QVariant(QString())).toString() ;

	if(tempFilename.isEmpty() || !QFile::exists(tempFilename)){
		QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
		QDir().mkpath(path);

		tempFilename = path + "/lastQuery";
		settings.setValue("/files/tempFile", tempFilename);
	}

	_last = new QFile(tempFilename);
	if (_last->open(QIODevice::ReadWrite | QIODevice::Text))
	{
		_strem = new QTextStream(_last);
		_strem->setCodec("UTF-8");
	}
	else
	{
		delete _last;
		_last = 0;
	}
}

FileLoadSave::~FileLoadSave()
{
	if(_last != 0){
		if(_strem != 0)
			delete _strem;

		_last->close();
		delete _last;
	}
}

QWidget* FileLoadSave::parent() const
{
	return dynamic_cast<QWidget*>(QObject::parent());
}

QString FileLoadSave::lastQuery()
{
	QString result;

	if(_strem != 0)
	{
		_strem->seek(0);
		result = _strem->readAll();
	}

	return result;
}

void FileLoadSave::setLastQuery(QString query)
{
	if(_last != 0 && _strem != 0)
	{
		_last->resize(0);

		*_strem << query;
		_strem->flush();
	}
}

QString FileLoadSave::getLoadFromFileFilename()
{
	QSettings settings;
	QString path =  settings.value("/files/lastSaveDir").toString();
	qDebug() << path;
	QString fileName = QFileDialog::getOpenFileName(parent(), QString(), path);

	if(!fileName.isEmpty())
	{
		path = fileName.section('/', 0, -2);
		settings.setValue("/files/lastSaveDir", QVariant(path));

		return fileName;
	}

	return QString();
}

QString FileLoadSave::loadFromFile(QString filename)
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
			updateLastUsedOrder(filename);
		}
	}

	return result;
}

unsigned int FileLoadSave::menuRecentItemCount()
{
	QSettings settings;
	return settings.value("/files/recentCount", QVariant(DEFAULT_MENU_ITEMS_COUNT)).toInt();
}

void FileLoadSave::updateLastUsedOrder(QString newName)
{
	QSettings settings;
	int count = menuRecentItemCount();
	QStringList settingsToAdd;
	settingsToAdd.reserve(count);

	settingsToAdd.append(newName);

	for(int i = 0; i < count - 1; i++)
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

	emit lastUsedUpdated();
}

QStringList FileLoadSave::getLastUsedMenuEntries()
{
	QStringList result;
	QSettings settings;

	for(unsigned int i = 0; i < menuRecentItemCount(); i++)
	{
		QString item = settings.value(QString("/files/load/%1").arg(i)).toString();

		if(item.isEmpty())
			break;

		result.append(item);
	}

	return result;
}


QString FileLoadSave::saveToFile(QString query)
{
	QSettings settings;
	QString path =  settings.value("/files/lasLoadDir").toString();
	QString fileName = QFileDialog::getSaveFileName(parent(), QString(), path);
	QString savedTo;

	if(!fileName.isEmpty())
	{
		path = fileName.section('/', 0, -2);
		settings.setValue("/files/lasLoadDir", QVariant(path));

		savedTo = saveToFile(fileName, query);
	}

	return savedTo;
}

QString FileLoadSave::saveToFile(QString fileName, QString query)
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
		updateLastUsedOrder(fileName);
	}

	return fileName;
}

QString FileLoadSave::loadResource(QString path){
	QFile resourceFile(path);
	resourceFile.open(QIODevice::ReadOnly);
	QTextStream stream(&resourceFile);
	stream.setCodec("UTF-8");
	return stream.readAll();
}

QString FileLoadSave::getAutoLoadText(QString script){
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
