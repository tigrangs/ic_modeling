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
               double pw);


public:
    const std::string& name() const;
    const position& pos() const;
    double width() const;
    double height() const;
    double power() const;


private:
    const std::string m_name;
    position m_pos;
    double m_width;
    double m_height;
    double m_power;

};

}

#endif // POWER_CELL_HPP
