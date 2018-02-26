#ifndef THERMAL_WINDOW_HPP
#define THERMAL_WINDOW_HPP

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>

#include <QWidget>

using namespace QtDataVisualization;

class QLabel;

namespace core
{

class layer;

}

namespace gui
{

class thermal_window : public QWidget
{
    Q_OBJECT
public:
    explicit thermal_window(QWidget *parent = nullptr);

private:
    void init();

public:
    void fill_data(core::layer *l);

private:
    Q3DSurface* m_surface = 0;
    QSurface3DSeries* m_series = 0;
    core::layer* m_layer = 0;
    QLabel* m_label = 0;

signals:

public slots:
};

}

#endif // THERMAL_WINDOW_HPP
