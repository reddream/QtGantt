#-------------------------------------------------
#
# Project created by QtCreator 2017-01-10T14:53:35
#
#-------------------------------------------------

TARGET = gantt-lib
TEMPLATE = lib

DEFINES += GANTTLIB_LIBRARY

include(../gantt-lib-include.pri)

SOURCES += \
    gantt/info/ganttinfoitem.cpp \
    gantt/info/ganttinfoleaf.cpp \
    gantt/info/ganttinfonode.cpp \
    gantt/info/ganttinfotree.cpp \
    gantt/player/ganttplayercontrol.cpp \
    gantt/player/ganttplayersettings.cpp \
    gantt/player/playercontrolbutton.cpp \
    gantt/plot/ganttscene.cpp \
    gantt/plot/scene_objects/ganttcalcgraphicsobject.cpp \
    gantt/plot/scene_objects/ganttcurrentdtslider.cpp \
    gantt/plot/scene_objects/ganttdtcrossobject.cpp \
    gantt/plot/scene_objects/ganttgraphicsobject.cpp \
    gantt/plot/scene_objects/gantthovergraphicsobject.cpp \
    gantt/plot/scene_objects/ganttintervalgraphicsobject.cpp \
    gantt/plot/hfitscene.cpp \
    gantt/plot/ganttgraphicsview.cpp \
    extensions/dtline.cpp \
    extensions/appeventfilter.cpp \
    extensions/intervalslider.cpp \
    extensions/timespan_extension.cpp \
    extensions/idtinterval.cpp \
    ganttwidget.cpp \
    gantt/ganttintervalslider.cpp \
    gantt/interfaces/iganttmodel.cpp \
    gantt/plot/ganttdtline.cpp

HEADERS +=\
        gantt-lib_global.h \
    gantt/info/ganttinfonode.h \
    gantt/info/ganttinfoitem.h \
    gantt/info/ganttinfoleaf.h \
    gantt/info/ganttinfotree.h \
    gantt/player/ganttplayercontrol.h \
    gantt/player/ganttplayersettings.h \
    gantt/player/playercontrolbutton.h \
    gantt/plot/ganttscene.h \
    gantt/plot/scene_objects/ganttcalcgraphicsobject.h \
    gantt/plot/scene_objects/ganttcurrentdtslider.h \
    gantt/plot/scene_objects/ganttdtcrossobject.h \
    gantt/plot/scene_objects/ganttgraphicsobject.h \
    gantt/plot/scene_objects/gantthovergraphicsobject.h \
    gantt/plot/scene_objects/ganttintervalgraphicsobject.h \
    gantt/plot/hfitscene.h \
    gantt/plot/ganttgraphicsview.h \
    extensions/appeventfilter.h \
    extensions/gantt-lib_global_values.h \
    extensions/intervalslider.h \
    extensions/timespan_extension.h \
    extensions/idtinterval.h \
    extensions/dtline.h \
    ganttwidget.h \
    gantt/ganttintervalslider.h \
    gantt/plot/ganttdtline.h \
    gantt/interfaces/iganttmodel.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    icons.qrc

FORMS += \
    gantt/player/ganttplayercontrol.ui \
    ganttwidget.ui

DISTFILES += \
    ../gantt-lib-include.pri \
    gantt_uml.qmodel
