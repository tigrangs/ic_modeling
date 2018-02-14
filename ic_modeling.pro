QT += core gui widgets
QT += datavisualization

INCLUDEPATH += src

SOURCES += \
    src/main.cpp \
    src/core/cell.cpp \
    src/core/ic.cpp \
    src/core/layer.cpp \
    src/gui/window.cpp

HEADERS += \
    src/core/cell.hpp \
    src/core/ic.hpp \
    src/core/layer.hpp \
    src/gui/window.hpp
