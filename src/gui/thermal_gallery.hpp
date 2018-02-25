#ifndef THERMAL_GALLERY_HPP
#define THERMAL_GALLERY_HPP

#include <QWidget>

namespace core
{
class ic;
}

namespace gui
{

class thermal_window;

class thermal_gallery : public QWidget
{
    Q_OBJECT
public:
    explicit thermal_gallery(core::ic*, QRectF, int, QWidget *parent = nullptr);

private:
    using layers = QList<thermal_window*>;

private:
    layers m_layers = {};
};

}

#endif // THERMAL_GALLERY_HPP
