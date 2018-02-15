#include "power_cell.hpp"

namespace files_parser
{

power_cell::power_cell(const std::string& n,
                       position p,
                       double w,
                       double h)
    : m_name(n)
    , m_pos(p)
    , m_width(w)
    , m_height(h)
{
}


const std::string& power_cell::name() const
{
    return m_name;
}

const power_cell::position& power_cell::pos() const
{
    return m_pos;
}

double power_cell::width() const
{
    return m_width;
}

double power_cell::height() const
{
    return m_height;
}

}
