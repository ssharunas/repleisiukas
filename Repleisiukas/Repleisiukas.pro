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
    qadvancedtabbar.cpp

HEADERS  += mainwindow.h \
	ui/jshighlighter.h \
    qadvancedinputpanel.h \
	presentation/fileloadsave.h \
    presentation/queryexecution.h \
    model/qtabdocument.h \
    qadvancedtabbar.h

FORMS    += mainwindow.ui \
    qadvancedinputpanel.ui

OTHER_FILES += \
	other/extensions.js

RESOURCES += \
	other/resources.qrc

LIBS         += -lqscintilla2
