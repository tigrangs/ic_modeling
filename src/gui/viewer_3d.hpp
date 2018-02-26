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

class viewer_3d : public QWidget
{
    Q_OBJECT
public:
    explicit viewer_3d(QWidget *parent = nullptr);

private:
    void init();

public:
    void set_draw_mode(QSurface3DSeries::DrawFlag);
    void fill_data(core::layer *l);
    void set_gradient(QLinearGradient gr);

private:
    Q3DSurface* m_surface = 0;
    QSurface3DSeries* m_series = 0;
    core::layer* m_layer = 0;
    QLabel* m_label = 0;
    QSurface3DSeries::DrawFlag m_draw_mode = QSurface3DSeries::DrawSurfaceAndWireframe;
};

}

#endif // THERMAL_WINDOW_HPP
