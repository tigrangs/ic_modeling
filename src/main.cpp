#include <core/ic.hpp>
#include <core/layer.hpp>
#include <gui/window.hpp>

#include <QApplication>
#include <iostream>

int main(int argc, char** argv)
{
    core::ic c(2, 2, 2);
    std::string s;
    c.dump(s);
    std::cout<<s<<std::endl;
    QApplication app(argc, argv);
    gui::thermal_window w;
//    w.fill_data(&l);
//    w.show();
    return app.exec();
}
