#ifndef THERMAL_GALLERY_HPP
#define THERMAL_GALLERY_HPP

#include <QWidget>

namespace core
{
class ic;
}

namespace gui
{

class viewer_3d;

class thermal_gallery : public QWidget
{
    Q_OBJECT
public:
    explicit thermal_gallery(core::ic*, QRectF, int, QWidget *parent = nullptr);

private:
    using layers = QList<viewer_3d*>;

private:
    layers m_layers = {};
};

}

#endif // THERMAL_GALLERY_HPP
