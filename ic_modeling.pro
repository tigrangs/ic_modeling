QT += core gui widgets
QT += datavisualization

INCLUDEPATH += src

SOURCES += \
    src/main.cpp \
    src/core/cell.cpp \
    src/core/ic.cpp \
    src/core/layer.cpp \
    src/parser/parser.cpp \
    src/parser/power_cell.cpp \
    src/gui/cells_window.cpp \
    src/gui/main_window.cpp \
    src/gui/layers_gallery.cpp \
    src/gui/thermal_gallery.cpp \
    src/gui/viewer_3d.cpp \
    src/gui/power_gallery.cpp \
    src/controller/matrix_cell.cpp \
    src/controller/matrix_layer.cpp \
    src/core/submatrix.cpp \
    src/controller/genetic_improvement.cpp

HEADERS += \
    src/core/cell.hpp \
    src/core/ic.hpp \
    src/core/layer.hpp \
    src/parser/parser.hpp \
    src/parser/power_cell.hpp \
    src/gui/cells_window.hpp \
    src/gui/main_window.hpp \
    src/gui/layers_gallery.hpp \
    src/gui/thermal_gallery.hpp \
    src/gui/viewer_3d.hpp \
    src/gui/power_gallery.hpp \
    src/controller/matrix_cell.hpp \
    src/controller/matrix_layer.hpp \
    src/core/submatrix.hpp \
    src/controller/genetic_improvement.hpp

DISTFILES +=

RESOURCES += \
    icons/icons.qrc
