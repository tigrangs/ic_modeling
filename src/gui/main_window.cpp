#include "main_window.hpp"
#include "layers_gallery.hpp"
#include "power_gallery.hpp"
#include "thermal_gallery.hpp"

#include <core/ic.hpp>
#include <core/layer.hpp>
#include <parser/parser.hpp>

#include <QAction>
#include <QErrorMessage>
#include <QFileDialog>
#include <QFormLayout>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
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
    setWindowTitle("Thermal Modeling and Improvement Compiler");
}

void main_window::init_toolbar()
{
    m_tools = new QToolBar;
    addToolBar(Qt::TopToolBarArea, m_tools);
}

void main_window::init_actions()
{
    QMenuBar* mb = menuBar();
    assert(mb != 0);
    QMenu* file = new QMenu("File");
    mb->addMenu(file);
    QMenu* edit = new QMenu("Edit");
    mb->addMenu(edit);
    QMenu* create = new QMenu("Create");
    mb->addMenu(create);
    QMenu* generate = new QMenu("Generate");
    mb->addMenu(generate);
    QMenu* options = new QMenu("Options");
    mb->addMenu(options);

    // Open
    QAction* load_ic = new QAction(QIcon(":icons/open.png"), "Open...");
    bool b = connect(load_ic, SIGNAL(triggered(bool)), this, SLOT(load_ic()));
    assert(b);
    m_tools->addAction(load_ic);
    file->addAction(load_ic);

    // Save
    QAction* save = new QAction(QIcon(":icons/save.png"), "Generate Netlist...");
    b = connect(save, SIGNAL(triggered(bool)), this, SLOT(save_netlist()));
    assert(b);
    m_tools->addAction(save);
    generate->addAction(save);

    // Load netlist
    QAction* load_netlist = new QAction(QIcon(":icons/load.png"), "Load simulation results...");
    b = connect(load_netlist, SIGNAL(triggered(bool)), this, SLOT(load_netlist()));
    assert(b);
    m_tools->addAction(load_netlist);
    file->addAction(load_netlist);
    file->addSeparator();
    file->addAction("Exit");

    // Show grid
    QAction* show_grid = new QAction(QIcon(":icons/grid.png"), "Grid");
    show_grid->setCheckable(true);
    b = connect(show_grid, SIGNAL(toggled(bool)), this, SLOT(show_grid(bool)));
    assert(b);
    m_tools->addAction(show_grid);
    edit->addAction(show_grid);
    edit->addAction("Move");
    edit->addAction("Resize");
    edit->addAction("Zoom In");
    edit->addAction("Zoom Out");
    create->addAction("Cell");
    options->addAction("General...", this, &main_window::show_general_options);

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

    // Show power widget
    QAction* show_powers = new QAction(QIcon(":icons/power.png"), "Generate Powers Map");
    b = connect(show_powers, SIGNAL(triggered(bool)), this, SLOT(show_power_gallery()));
    assert(b);
    m_tools->addAction(show_powers);
    generate->addAction(show_powers);
    generate->addAction("Generate Thermal Map");
    generate->addAction("Optimize placement");
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
    assert(m_gallery != 0);
    QRectF bRect = m_gallery->get_rect();
    int itStep = m_gallery->get_grid_size();
    thermal_gallery* tg = new thermal_gallery(ic, bRect, itStep);
    tg->show();
}

void main_window::show_power_gallery()
{
    core::ic* ic = m_gallery->get_ic();
    assert(ic != 0);
    power_gallery* pg = new power_gallery(ic);
    pg->show();
}

void main_window::show_general_options()
{
    QDialog d;
    d.setWindowTitle("General Options");
    QFormLayout* fl = new QFormLayout();
    fl->setLabelAlignment(Qt::AlignRight);
    QSpinBox* sb = new QSpinBox();
    sb->setMinimum(1);
    sb->setMaximum(1000);
    bool b = connect(sb, SIGNAL(valueChanged(int)), this, SLOT(grid_size_changed(int)));
    assert(b);
    QHBoxLayout* hbl = new QHBoxLayout;
    hbl->addWidget(sb);
    hbl->addWidget(new QLabel("um"));
    fl->addRow("Grid step:", hbl);
    hbl = new QHBoxLayout;
    hbl->addWidget(new QLineEdit("/remote/u/tigrangs/hspice/saed32nm.lib"));
    hbl->addWidget(new QPushButton("Browse..."));
    fl->addRow("Standard cells library path:", hbl);
    d.setLayout(fl);
    d.exec();
}

}
