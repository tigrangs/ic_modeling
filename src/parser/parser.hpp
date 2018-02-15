#ifndef PARSER_HPP
#define PARSER_HPP

#include "power_cell.hpp"

#include <list>
#include <string>

namespace files_parser
{

class parser
{
public:
    static parser* get_instance();

public:
    using power_cells = std::list<power_cell>;

public:
    power_cells get_cells(const std::string&);

private:
    parser();

private:
    static parser* s_parser;

private:
    parser(const parser&) = delete;
    parser& operator=(const parser&) = delete;

};

}

#endif
