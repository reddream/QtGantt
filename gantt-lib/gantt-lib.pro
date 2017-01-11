#-------------------------------------------------
#
# Project created by QtCreator 2017-01-10T14:53:35
#
#-------------------------------------------------

TARGET = gantt-lib
TEMPLATE = lib

DEFINES += GANTTLIB_LIBRARY

DESTDIR = $$OUT_PWD/../bin

#DLLDESTDIR = $$DESTDIR

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
# подключить библиотеки и *.h файлы
CONFIG += depend_includepath

INCLUDEPATH += $$PWD/gantt
INCLUDEPATH += $${DEP_PATH_HEADERS}/extensions
INCLUDEPATH += $${DEP_PATH_HEADERS}/extensions-gui



SOURCES += ganttlib.cpp \
    appeventfilter.cpp \
    intervalslider.cpp \
    gantt/dtline.cpp \
    timespan_extension.cpp \
    gantt/default_model/ganttinfoitem.cpp \
    gantt/default_model/ganttinfoleaf.cpp \
    gantt/default_model/ganttinfonode.cpp \
    gantt/default_model/gantttreemodel.cpp \
    gantt/default_model/iganttmodel.cpp \
    gantt/default_model/gantttreeview.cpp

HEADERS += ganttlib.h\
        gantt-lib_global.h \
    appeventfilter.h \
    intervalslider.h \
    gantt/dtline.h \
    timespan_extension.h \
    gantt-lib_global_values.h \
    gantt/default_model/ganttinfonode.h \
    gantt/default_model/ganttinfoitem.h \
    gantt/default_model/ganttinfoleaf.h \
    gantt/default_model/gantttreemodel.h \
    gantt/default_model/iganttmodel.h \
    gantt/default_model/gantttreeview.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    icons.qrc
