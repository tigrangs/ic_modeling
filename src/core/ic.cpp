#include "ic.hpp"
#include "layer.hpp"

#include <cassert>

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

void ic::dump(std::string& s) const
{
        for (unsigned i = 0; i < m_layers.size(); ++i) {
            layer* l = m_layers[i];
            assert(l != 0);
            l->dump(s);
        }
}

int ic::layers_count() const
{
    return m_layers.size();
}

layer* ic::get_layer(int i)
{
    assert(i < layers_count());
    return m_layers[i];
}

}
