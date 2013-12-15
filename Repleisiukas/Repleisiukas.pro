#-------------------------------------------------
#
# Project created by QtCreator 2010-09-01T11:27:55
#
#-------------------------------------------------

QT       += core gui script scripttools
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = Repleisiukas
TEMPLATE = app


SOURCES += main.cpp\
        ui/mainwindow.cpp \
        ui/qadvancedinputpanel.cpp \
        ui/qadvancedtabbar.cpp \
        presentation/fileloadsave.cpp \
	presentation/queryexecution.cpp \
	model/qtabdocument.cpp \
	model/qfilesystemaccess.cpp \
	model/filesystem/filesystemfactory.cpp \
	model/filesystem/permission.cpp \
	model/filesystem/local/localfolderobject.cpp \
	model/filesystem/filesystemobject.cpp \
	model/filesystem/local/localfilesystemhandler.cpp \
	model/irepleisiukasscriptobject.cpp \
	model/filesystem/local/localfileobject.cpp \
	model/filesystem/openmodes.cpp \
        model/ihelpfull.cpp

HEADERS  += \
        ui/mainwindow.h \
        ui/qadvancedinputpanel.h \
        ui/qadvancedtabbar.h \
        presentation/fileloadsave.h \
	presentation/queryexecution.h \
	model/qtabdocument.h \
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
	model/filesystem/local/localfileobject.h \
        model/ihelpfull.h

FORMS    += \
        ui/mainwindow.ui \
        ui/qadvancedinputpanel.ui

OTHER_FILES += \
	other/extensions.js

RESOURCES += \
	other/resources.qrc

LIBS      += -lqscintilla2
