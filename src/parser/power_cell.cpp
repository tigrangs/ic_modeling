#include "power_cell.hpp"

namespace files_parser
{

power_cell::power_cell(const std::string& n, position p,
                       double w, double h,
                       double pw, unsigned l, double fr)
    : m_name(n)
    , m_pos(p)
    , m_width(w)
    , m_height(h)
    , m_power(pw)
    , m_layer(l)
    , m_frequency(fr)
{
}

power_cell& power_cell::operator=(const power_cell& c)
{
    if (&c == this) {
        return *this;
    }
    m_name = c.m_name;
    m_pos = c.m_pos;
    m_width = c.m_width;
    m_height = c.m_height;
    m_power = c.m_power;
    m_layer = c.m_layer;
    m_frequency = c.m_frequency;
    return *this;
}

power_cell::power_cell(const power_cell& c)
{
    m_name = c.m_name;
    m_pos = c.m_pos;
    m_width = c.m_width;
    m_height = c.m_height;
    m_power = c.m_power;
    m_layer = c.m_layer;
    m_frequency = c.m_frequency;
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

double power_cell::power() const
{
    return m_power;
}

unsigned power_cell::layer() const
{
    return m_layer;
}

double power_cell::frequency() const
{
    return m_frequency;
}

}
