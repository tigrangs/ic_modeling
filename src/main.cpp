#include <core/ic.hpp>
#include <core/layer.hpp>
#include <gui/viewer_3d.hpp>
#include <gui/cells_window.hpp>
#include <gui/main_window.hpp>

#include <QApplication>
#include <iostream>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    gui::main_window mw;
    mw.show();
    return app.exec();
}
