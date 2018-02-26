#ifndef IC_HPP
#define IC_HPP

#include <vector>
#include <string>

namespace core
{

class layer;

class ic
{
public:
        using layers = std::vector<layer*>;

public:
        ic(const std::vector<layer*>&);
        ic(unsigned l, unsigned w, unsigned h);
        ~ic();

public:
        void dump(std::string&) const;

public:
        int layers_count() const;

public:
        layer* get_layer(int);

private:
        layers m_layers = {};
};

}

#endif // IC_HPP
