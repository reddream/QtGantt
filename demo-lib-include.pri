
message(PWD: $$PWD)
INCLUDEPATH += "$$PWD/../gantt-lib"
INCLUDEPATH += "$$PWD/../gantt-lib/gantt"
LIBS += -L"$${DESTDIR}"
CONFIG(release, debug|release) {
  LIBS += -lgantt-lib
} else:CONFIG(debug, debug|release) {
  LIBS += -lgantt-lib
}
