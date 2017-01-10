#-------------------------------------------------
#
# Project created by QtCreator 2017-01-10T14:53:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gantt-demo
TEMPLATE = app

DESTDIR = $$OUT_PWD/../bin

unix: CONFIG += debug_and_release debug_and_release_target

# выведем сообщение компилятора в каком режиме собираем (для проверки)
release:message(Building in release mode.)
debug:message(Building in debug mode.)


# подключить библиотеки и *.h файлы
CONFIG += depend_includepath

message(PWD: $$PWD)
INCLUDEPATH += "$$PWD/../gantt-lib"
INCLUDEPATH += "$$PWD/../gantt-lib/gantt"

unix {
  DEP_PATH_LIBS = /usr/lib64
  DEP_PATH_HEADERS = /usr/include
} else:win32 {
  DEP_PATH_LIBS = C:/lib64
  DEP_PATH_HEADERS = C:/include
}
INCLUDEPATH += $${DEP_PATH_HEADERS}/extensions
INCLUDEPATH += $${DEP_PATH_HEADERS}/extensions-gui
INCLUDEPATH += $${DEP_PATH_HEADERS}


message(DESTDIR: $$DESTDIR)
LIBS += -L"$${DESTDIR}"
CONFIG(release, debug|release) {
  LIBS += -lgantt-lib
} else:CONFIG(debug, debug|release) {
  LIBS += -lgantt-lib
}

# пути к зависимым библиотекам
unix {
  DEP_PATH_LIBS = /usr/lib64
  DEP_PATH_HEADERS = /usr/include
} else:win32 {
  DEP_PATH_LIBS = C:/lib64
  DEP_PATH_HEADERS = C:/include
}

LIBS += -L$${DEP_PATH_LIBS}
CONFIG(release, debug|release) {
    LIBS += -lextensions -lextensions-gui
} else:CONFIG(debug, debug|release) {
    LIBS += -lextensionsd -lextensions-guid
}






SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
