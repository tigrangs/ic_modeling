#include "main_window.hpp"
#include "layers_gallery.hpp"
#include "thermal_window.hpp"

#include <core/ic.hpp>
#include <parser/parser.hpp>

#include <QAction>
#include <QErrorMessage>
#include <QFileDialog>
#include <QLabel>
#include <QLayout>
#include <QSpinBox>
#include <QToolBar>
#include <QWidgetAction>

#include <cassert>

namespace gui
{

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
{
    init();
}

void main_window::init()
{
    setMinimumSize(800, 800);
    m_gallery = new layers_gallery(this);
    setCentralWidget(m_gallery);
    init_toolbar();
    init_actions();
}

void main_window::init_toolbar()
{
    m_tools = new QToolBar;
    addToolBar(Qt::TopToolBarArea, m_tools);
}

void main_window::init_actions()
{
    // Open
    QAction* load_ic = new QAction(QIcon(":icons/open.png"), "Open...");
    bool b = connect(load_ic, SIGNAL(triggered(bool)), this, SLOT(load_ic()));
    assert(b);
    m_tools->addAction(load_ic);

    // Save
    QAction* save = new QAction(QIcon(":icons/save.png"), "Save Netlist");
    b = connect(save, SIGNAL(triggered(bool)), this, SLOT(save_netlist()));
    assert(b);
    m_tools->addAction(save);

    // Load netlist
    QAction* load_netlist = new QAction("Load");
    b = connect(load_netlist, SIGNAL(triggered(bool)), this, SLOT(load_netlist()));
    assert(b);
    m_tools->addAction(load_netlist);

    // Show grid
    QAction* show_grid = new QAction(QIcon(":icons/grid.png"), "Grid");
    show_grid->setCheckable(true);
    b = connect(show_grid, SIGNAL(toggled(bool)), this, SLOT(show_grid(bool)));
    assert(b);
    m_tools->addAction(show_grid);

    // Grid step
    QWidget* gsw = new QWidget;
    QHBoxLayout* l = new QHBoxLayout;
    gsw->setLayout(l);
    l->addWidget(new QLabel("Grid Step:"));
    QSpinBox* sb = new QSpinBox();
    sb->setMinimum(1);
    sb->setMaximum(1000);
    l->addWidget(sb);
    b = connect(sb, SIGNAL(valueChanged(int)), this, SLOT(grid_size_changed(int)));
    QWidgetAction* gs = new QWidgetAction(m_tools);
    gs->setDefaultWidget(gsw);
    m_tools->addAction(gs);
}

void main_window::load_ic()
{
    QString f = QFileDialog::getOpenFileName(this);
    if (f.isEmpty()) {
        return;
    }
    files_parser::parser* p = files_parser::parser::get_instance();
    assert(p != 0);
    assert(m_gallery != 0);
    try {
        files_parser::parser::power_cells cells = p->get_cells(f.toStdString());
        // Adding cells to gallery
        m_gallery->fill_layers(cells);
//        layout()->setSizeConstraint(QLayout::SetFixedSize);
    } catch (const files_parser::parser::exception& e) {
        QErrorMessage em;
        em.showMessage(QString::fromStdString(e.what()));
        em.exec();
    }
}

void main_window::save_netlist()
{
    QString f = QFileDialog::getExistingDirectory(this);
    std::string content = m_gallery->dump_netlist();
    QFile nf(f + "/netlist.sp");
    if (!nf.open(QFile::WriteOnly)) {
        QErrorMessage em;
        em.showMessage("Don't have write access");
        em.exec();
    }
    nf.write(QByteArray::fromStdString(content));
}

void main_window::show_grid(bool s)
{
    assert(m_gallery != 0);
    m_gallery->show_grid(s);
}

void main_window::grid_size_changed(int s)
{
    assert(m_gallery != 0);
    m_gallery->set_grid_size(s);
}

void main_window::load_netlist()
{
    QString f = QFileDialog::getOpenFileName(this, "Open Dialog", "", "*.ic0");
    if (f.isEmpty()) {
        return;
    }
    files_parser::parser* p = files_parser::parser::get_instance();
    assert(p != 0);
    core::ic* ic = p->get_ic(f.toStdString());
    if (ic == 0) {
        return;
    }
    thermal_window* tw = new thermal_window;
    tw->fill_data(ic->get_layer(0));
    tw->show();
}

}
