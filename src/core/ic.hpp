#ifndef IC_HPP
#define IC_HPP

#include <vector>

namespace core
{

class layer;

class ic
{
public:
        using layers = std::vector<layer*>;

public:
        ic(unsigned l, unsigned w, unsigned h);
        ~ic();

public:
        void dump() const;

private:
        layers m_layers = {};
};

}

#endif // IC_HPP
