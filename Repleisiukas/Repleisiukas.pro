#-------------------------------------------------
#
# Project created by QtCreator 2010-09-01T11:27:55
#
#-------------------------------------------------

QT       += core gui script scripttools

TARGET = Repleisiukas
TEMPLATE = app


SOURCES += main.cpp\
		mainwindow.cpp \
	ui/jshighlighter.cpp \
	qadvancedinputpanel.cpp \
	presentation/fileloadsave.cpp \
	presentation/queryexecution.cpp \
	model/qtabdocument.cpp \
	qadvancedtabbar.cpp \
	model/qfilesystemaccess.cpp \
	model/filesystem/filesystemfactory.cpp \
	model/filesystem/permission.cpp \
	model/filesystem/local/localfolderobject.cpp \
	model/filesystem/filesystemobject.cpp \
	model/filesystem/local/localfilesystemhandler.cpp \
	model/irepleisiukasscriptobject.cpp \
	model/filesystem/local/localfileobject.cpp \
	model/filesystem/openmodes.cpp

HEADERS  += mainwindow.h \
	ui/jshighlighter.h \
	qadvancedinputpanel.h \
	presentation/fileloadsave.h \
	presentation/queryexecution.h \
	model/qtabdocument.h \
	qadvancedtabbar.h \
	model/qfilesystemaccess.h \
	model/filesystem/filesystemfactory.h \
	model/filesystem/ifilesystemhandler.h \
	model/filesystem/ifilesystemobject.h \
	model/filesystem/whence.h \
	model/filesystem/permission.h \
	model/filesystem/local/localfolderobject.h \
	model/filesystem/filesystemobject.h \
	model/filesystem/local/localfilesystemhandler.h \
	model/filesystem/openmodes.h \
	model/irepleisiukasscriptobject.h \
	model/scriptvalueconverttemplate.h \
	model/filesystem/local/localfileobject.h

FORMS    += mainwindow.ui \
	qadvancedinputpanel.ui

OTHER_FILES += \
	other/extensions.js

RESOURCES += \
	other/resources.qrc

LIBS         += -lqscintilla2
