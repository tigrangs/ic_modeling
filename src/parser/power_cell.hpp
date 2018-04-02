#ifndef POWER_CELL_HPP
#define POWER_CELL_HPP

#include <string>
#include <utility>

namespace files_parser
{

class power_cell
{
public:
    using position = std::pair<double, double>;

public:
    power_cell(const std::string& n,
               position p,
               double w,
               double h,
               double pw,
               unsigned l,
               double fr);

    power_cell& operator=(const power_cell&)/* = default*/;
    power_cell(const power_cell&)/* = default*/;

public:
    const std::string& name() const;
    const position& pos() const;
    double width() const;
    double height() const;
    double power() const;
    unsigned layer() const;
    double frequency() const;

private:
    std::string m_name;
    position m_pos;
    double m_width;
    double m_height;
    double m_power;
    unsigned m_layer;
    double m_frequency;

};

}

#endif // POWER_CELL_HPP
