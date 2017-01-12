#-------------------------------------------------
#
# Project created by QtCreator 2017-01-10T14:53:35
#
#-------------------------------------------------

TARGET = gantt-lib
TEMPLATE = lib

DEFINES += GANTTLIB_LIBRARY

include(../gantt-lib-include.pri)

SOURCES += ganttlib.cpp \
    appeventfilter.cpp \
    intervalslider.cpp \
    timespan_extension.cpp \
    gantt/default_model/ganttinfoitem.cpp \
    gantt/default_model/ganttinfoleaf.cpp \
    gantt/default_model/ganttinfonode.cpp \
    gantt/default_model/gantttreemodel.cpp \
    gantt/default_model/iganttmodel.cpp \
    gantt/ganttinfotree.cpp \
    gantt/ganttwidget.cpp \
    gantt/player/ganttplayercontrol.cpp \
    gantt/player/ganttplayersettings.cpp \
    gantt/player/playercontrolbutton.cpp \
    gantt/plot/dtline.cpp \
    gantt/plot/ganttscene.cpp \
    gantt/plot/scene_objects/ganttcalcgraphicsobject.cpp \
    gantt/plot/scene_objects/ganttcurrentdtslider.cpp \
    gantt/plot/scene_objects/ganttdtcrossobject.cpp \
    gantt/plot/scene_objects/ganttgraphicsobject.cpp \
    gantt/plot/scene_objects/gantthovergraphicsobject.cpp \
    gantt/plot/scene_objects/ganttintervalgraphicsobject.cpp

HEADERS += ganttlib.h\
        gantt-lib_global.h \
    appeventfilter.h \
    intervalslider.h \
    timespan_extension.h \
    gantt-lib_global_values.h \
    gantt/default_model/ganttinfonode.h \
    gantt/default_model/ganttinfoitem.h \
    gantt/default_model/ganttinfoleaf.h \
    gantt/default_model/gantttreemodel.h \
    gantt/default_model/iganttmodel.h \
    gantt/ganttinfotree.h \
    gantt/ganttwidget.h \
    gantt/player/ganttplayercontrol.h \
    gantt/player/ganttplayersettings.h \
    gantt/player/playercontrolbutton.h \
    gantt/plot/dtline.h \
    gantt/plot/ganttscene.h \
    gantt/plot/scene_objects/ganttcalcgraphicsobject.h \
    gantt/plot/scene_objects/ganttcurrentdtslider.h \
    gantt/plot/scene_objects/ganttdtcrossobject.h \
    gantt/plot/scene_objects/ganttgraphicsobject.h \
    gantt/plot/scene_objects/gantthovergraphicsobject.h \
    gantt/plot/scene_objects/ganttintervalgraphicsobject.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    icons.qrc

FORMS += \
    gantt/ganttwidget.ui \
    gantt/player/ganttplayercontrol.ui

DISTFILES += \
    ../gantt-lib-include.pri
