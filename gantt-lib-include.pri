
VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_PATCH = 0
VERSION = $$VERSION_MAJOR'.'$$VERSION_MINOR'.'$$VERSION_PATCH
message(VERSION2: $$VERSION)

DESTDIR = $$OUT_PWD/../bin

LIB_DIR = $$PWD/gantt-lib


# подключить библиотеки и *.h файлы
CONFIG += depend_includepath
unix: CONFIG += debug_and_release debug_and_release_target

# выведем сообщение компилятора в каком режиме собираем (для проверки)
release:message(Building in release mode.)
debug:message(Building in debug mode.)

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
message(Library directory: $$LIB_DIR)

#INCLUDEPATH += $$LIB_DIR
#INCLUDEPATH += $$LIB_DIR/extensions
#INCLUDEPATH += $$LIB_DIR/gantt
#INCLUDEPATH += $$LIB_DIR/gantt/interfaces
#INCLUDEPATH += $$LIB_DIR/gantt/info
#INCLUDEPATH += $$LIB_DIR/gantt/player
#INCLUDEPATH += $$LIB_DIR/gantt/plot
#INCLUDEPATH += $$LIB_DIR/gantt/plot/scene_objects


INCLUDEPATH += $${DEP_PATH_HEADERS}/extensions
INCLUDEPATH += $${DEP_PATH_HEADERS}/extensions-gui
