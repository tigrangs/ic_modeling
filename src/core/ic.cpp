#include "ic.hpp"
#include "layer.hpp"


namespace core
{

ic::ic(unsigned l, unsigned w, unsigned h)
{
        for (unsigned i = 0; i < l; ++i) {
                m_layers.push_back(new layer(i, w, h));
        }
}

ic::~ic()
{
        for (unsigned i = 0; i < m_layers.size(); ++i) {
                delete m_layers[i];
        }
}

}
