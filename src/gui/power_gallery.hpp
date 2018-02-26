#ifndef POWER_GALLERY_HPP
#define POWER_GALLERY_HPP

#include <QWidget>

namespace core
{
class ic;
}

namespace gui
{

class viewer_3d;

class power_gallery : public QWidget
{
    Q_OBJECT
public:
    explicit power_gallery(core::ic*, QWidget *parent = nullptr);

private:
    using layers = QList<viewer_3d*>;

private:
    layers m_layers = {};
};

}

#endif // POWER_GALLERY_HPP
