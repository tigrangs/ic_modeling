#include "ic.hpp"
#include "layer.hpp"

#include <cassert>
#include <iostream>

namespace core
{

ic::ic(const std::vector<layer *> & v)
{
        unsigned W = v[0]->width();
        unsigned H = v[0]->height();
        for (unsigned i = 1; i < v.size(); ++i) {
                assert(v[i]->width() == W);
                assert(v[i]->height() == H);
        }
        m_layers = v;
        std::cout<<"IC"<<std::endl;
}

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
