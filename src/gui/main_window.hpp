#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>

namespace gui
{

class layers_gallery;

class main_window : public QMainWindow
{
    Q_OBJECT
public:
    explicit main_window(QWidget *parent = nullptr);

signals:

private slots:
    void load_ic();
    void show_grid(bool);
    void grid_size_changed(int);

private:
    void init();
    void init_actions();
    void init_toolbar();

private:
    QToolBar* m_tools = 0;
    layers_gallery* m_gallery = 0;
};

}

#endif // MAIN_WINDOW_HPP
