#ifndef PARSER_HPP
#define PARSER_HPP

#include "power_cell.hpp"

#include <QMap>

#include <list>
#include <string>

namespace core
{

class ic;

}

namespace files_parser
{

class parser
{
public:
    static parser* get_instance();

public:
    using power_cells = std::list<power_cell>;

public:
    // Throws exceptions
    power_cells get_cells(const std::string&);

public:
    core::ic* get_ic(const std::string&);

public:
    class exception
    {
    public:
        exception(const std::string& e)
            : m_error(e)
        {}
    public:
        const std::string& what() const
        {
            return m_error;
        }

    private:
        const std::string m_error;
    };

private:
    parser();

private:
    static parser* s_parser;

public:
    using data_map = QMap<QString, QString>;

private:
    std::string get_name(const data_map& m) const;
    power_cell::position get_position(const data_map&) const;
    double get_value_of(const data_map&, const std::string&) const;
    unsigned get_layer(const data_map&) const;

private:
    parser(const parser&) = delete;
    parser& operator=(const parser&) = delete;

};

}

#endif
