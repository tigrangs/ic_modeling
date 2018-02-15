#include "parser.hpp"

#include <cassert>

namespace files_parser
{

parser* parser::s_parser = 0;

parser* parser::get_instance()
{
    if (s_parser == 0) {
        s_parser = new parser();
    }
    return s_parser;
}

parser::parser()
{
}

parser::power_cells parser::get_cells(const std::string& f)
{
    assert(!f.empty());
    power_cells cells;
    return cells;
}

}
