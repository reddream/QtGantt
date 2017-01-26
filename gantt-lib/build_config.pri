# Файл настроек проекта динамической библиотеки

# Исходный префикс для абсолютного пути установки библиотеки
unix{
  INSTALL_PREFIX = /usr
  # Собирать сразу две версии (Debug и Release)
  CONFIG += debug_and_release debug_and_release_target

} else:win32 {
  INSTALL_PREFIX = C:/
}

# Полный путь установки библиотек
INSTALL_LIB_PATH = $${INSTALL_PREFIX}/lib64/
# Полный путь установки заголовочных файлов
INSTALL_HEADERS_PATH = $${INSTALL_PREFIX}/include/$${TARGET}

# К отладочной версии дописать постфикс "d"
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,d)

win32:TARGET_EXT=.dll

target.path   = $${INSTALL_LIB_PATH}

INSTALLS += target

# Создать правило установки заголовочных файлов с относительными директориями
CONFIG(debug, debug|release) {
  INSTALL_HEADERS = $$HEADERS
  for(header, INSTALL_HEADERS) {
    dir = $${dirname(header)}
    path = $${INSTALL_HEADERS_PATH}/$${dir}
    eval(headers_$${dir}.files += $$header)
    eval(headers_$${dir}.path = $$path)
    eval(INSTALLS *= headers_$${dir})
  }
}
