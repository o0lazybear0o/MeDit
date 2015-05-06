#-------------------------------------------------
#
# Project created by QtCreator 2015-01-07T21:19:45
#
#-------------------------------------------------

QT       += core gui webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MeDitTabs
TEMPLATE = app

qtHaveModule(printsupport): QT += printsupport

SOURCES += main.cpp\
        mainwindow.cpp \
    editor.cpp \
    parse.cpp \
    autolink.c \
    buffer.c \
    houdini_href_e.c \
    houdini_html_e.c \
    html.c \
    markdown.c \
    stack.c

HEADERS  += mainwindow.h \
    editor.h \
    autolink.h \
    buffer.h \
    houdini.h \
    html.h \
    html_blocks.h \
    markdown.h \
    parse.h \
    stack.h \
    pubconst.h

DISTFILES += \
    CSS/Clearness.css \
    CSS/Clearness Dark.css \
    CSS/GitHub.css \
    CSS/GitHub2.css \
    CSS/Clearness Dark.css \
    CSS/Clearness Dark.css \
    CSS/Clearness Dark.css \
    CSS/ClearnessDark.css

RESOURCES += \
    resource.qrc
