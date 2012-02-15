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
    model/test/testobject.cpp \
    model/test/testprototype.cpp \
    model/filesystem/local/localfolderobjectprototype.cpp \
	model/filesystem/filesystemobjectprototype.cpp \
    model/filesystem/filesystemobject.cpp

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
    model/test/testobject.h \
    model/test/testprototype.h \
    model/filesystem/local/localfolderobjectprototype.h \
	model/filesystem/filesystemobjectprototype.h \
    model/filesystem/filesystemobject.h

FORMS    += mainwindow.ui \
    qadvancedinputpanel.ui

OTHER_FILES += \
	other/extensions.js

RESOURCES += \
	other/resources.qrc











