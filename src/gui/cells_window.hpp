#ifndef CELLS_WINDOW_HPP
#define CELLS_WINDOW_HPP

#include <parser/parser.hpp>

#include <QWidget>

class QGraphicsView;
class QGraphicsScene;

namespace gui
{

class cells_window : public QWidget
{
public:
    cells_window(QWidget* p = nullptr);

private:
    void init();

private:
    void fill_data(const files_parser::parser::power_cells&);

private:
    QGraphicsView* m_view = 0;
    QGraphicsScene* m_scene = 0;
};

}

#endif // CELLS_WINDOW_HPP
