message(PWD: $$PWD)
INCLUDEPATH += "$$PWD/gantt-lib"
LIBS += -L"$${DESTDIR}"

CONFIG(release, debug|release) {
  LIBS += -lgantt-lib$$VERSION_MAJOR
} else:CONFIG(debug, debug|release) {
  LIBS += -lgantt-lib$$VERSION_MAJOR
}
