#include "main_window.hpp"
#include "layers_gallery.hpp"

#include <parser/parser.hpp>

#include <QAction>
#include <QErrorMessage>
#include <QFileDialog>
#include <QToolBar>

#include <cassert>

namespace gui
{

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
    , m_tools(0)
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
    QAction* load_ic = new QAction(QIcon(":icons/open.png"), "Open...");
    m_tools->addAction(load_ic);
    bool b = connect(load_ic, SIGNAL(triggered(bool)), this, SLOT(load_ic()));
    assert(b);
}

void main_window::load_ic()
{
    QString f = QFileDialog::getOpenFileName(this);
    if (f.isEmpty()) {
        return;
    }
    files_parser::parser* p = files_parser::parser::get_instance();
    assert(p != 0);
    try {
        files_parser::parser::power_cells cells = p->get_cells(f.toStdString());
        // Adding cells to gallery
        m_gallery->fill_layers(cells);
    } catch (const files_parser::parser::exception& e) {
        QErrorMessage em;
        em.showMessage(QString::fromStdString(e.what()));
        em.exec();
    }
}

}
