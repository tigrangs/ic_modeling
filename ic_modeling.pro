QT += core gui widgets
QT += datavisualization

INCLUDEPATH += src

SOURCES += \
    src/main.cpp \
    src/core/cell.cpp \
    src/core/ic.cpp \
    src/core/layer.cpp \
    src/gui/thermal_window.cpp \
    src/parser/parser.cpp \
    src/parser/power_cell.cpp \
    src/gui/cells_window.cpp

HEADERS += \
    src/core/cell.hpp \
    src/core/ic.hpp \
    src/core/layer.hpp \
    src/gui/thermal_window.hpp \
    src/parser/parser.hpp \
    src/parser/power_cell.hpp \
    src/gui/cells_window.hpp
