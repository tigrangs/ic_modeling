#include "cell.hpp"
#include "layer.hpp"

#include <cassert>

namespace core
{

namespace {

static const std::string rR = "rR_";
static const std::string rr = "rr_s_";
static const std::string devider = "_";
static const std::string c = "_c_";
static const std::string R = "R=";
static const std::string _s = "_s";
static const std::string space = " ";
static const std::string rRs = "rRs_";
static const std::string vss = "vss";
static const std::string vdd = "vdd";

}

cell::cell(unsigned r, unsigned c, const layer* l)
    : m_row(r)
    , m_column(c)
    , m_layer(l)
    , m_value(0)
{
}

double cell::value() const
{
    return m_value;
}

void cell::set_value(double v)
{
    m_value = v;
}

void cell::dump(std::string& s) const
{
    const std::string i = std::to_string(m_row);
    const std::string j = std::to_string(m_column);
    const std::string k = std::to_string(m_layer->id());
    s += "\n";
    s += "\n* Cell " + i + space + j + space + k + " *\n";
    dump_I(s);
    dump_R(s);
    dump_Rsub(s);
    assert(m_layer != 0);
    if (m_row + 1 < m_layer->height()) {
        dump_R_bottom(s);
        dump_r_bottom(s);
    }
    if (m_column + 1 < m_layer->width()) {
        dump_R_right(s);
        dump_r_right(s);
    }
}

void cell::dump_I(std::string& s) const
{
    const std::string i = std::to_string(m_row);
    const std::string j = std::to_string(m_column);
    const std::string k = std::to_string(m_layer->id());
    const std::string cv = std::to_string(m_value);
    std::string tmp = ".param";
    tmp += space;
    tmp += "i" ;
    tmp += devider;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;
    tmp += space;
    tmp += "=" + cv + "\n";

    tmp += "i";
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;
    tmp += space;
    tmp += vdd;
    tmp += space;
    tmp += c;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;
    tmp += space;
    tmp += "dc =";
    tmp += space;
    tmp += "i" ;
    tmp += devider;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;
    tmp += space;
    tmp += "ac = 0\n";

    s += tmp;
}

void cell::dump_R(std::string& s) const
{
    assert(m_layer != 0);
    const std::string i = std::to_string(m_row);
    const std::string j = std::to_string(m_column);
    const std::string k = std::to_string(m_layer->id());

    std::string tmp = rR;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;

    tmp += space;
    tmp += c;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;

    tmp += space;
    tmp += c;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;
    tmp += _s;

    tmp += space;
    tmp += R;
    tmp += "Ri"; //TODO

    s += "\n";
    s += tmp;
}

void cell::dump_Rsub(std::string& s) const
{
    assert(m_layer != 0);
    const std::string i = std::to_string(m_row);
    const std::string j = std::to_string(m_column);
    const std::string k = std::to_string(m_layer->id());

    std::string tmp = rRs;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;

    tmp += space;
    tmp += c;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;
    tmp += _s;

    if (m_layer->id() == 0) {
        tmp += space + vss;
    } else {
        tmp += space;
        tmp += c;
        tmp += i;
        tmp += devider;
        tmp += j;
        tmp += devider;
        tmp += std::to_string(m_layer->id() - 1);
    }

    tmp += space;
    tmp += R;
    tmp += "Rsub"; //TODO

    s += "\n";
    s += tmp;
}

void cell::dump_R_right(std::string& s) const
{
    assert(m_layer != 0);
    const std::string i = std::to_string(m_row);
    const std::string j = std::to_string(m_column);
    const std::string k = std::to_string(m_layer->id());
    const std::string jn= std::to_string(m_column + 1);

    std::string tmp = rR;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;
    tmp += devider;
    tmp += i;
    tmp += devider;
    tmp += jn;
    tmp += devider;
    tmp += k;

    tmp += space;
    tmp += c;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;

    tmp += space;
    tmp += c;
    tmp += i;
    tmp += devider;
    tmp += jn;
    tmp += devider;
    tmp += k;

    tmp += space;
    tmp += R;
    tmp += "Rij"; //TODO

    s += "\n";
    s += tmp;
}

void cell::dump_R_bottom(std::string& s) const
{
    assert(m_layer != 0);
    const std::string i = std::to_string(m_row);
    const std::string j = std::to_string(m_column);
    const std::string k = std::to_string(m_layer->id());
    const std::string in= std::to_string(m_row + 1);

    std::string tmp = rR;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;
    tmp += devider;
    tmp += in;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;

    tmp += space;
    tmp += c;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;

    tmp += space;
    tmp += c;
    tmp += in;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;

    tmp += space;
    tmp += R;
    tmp += "Rij"; //TODO

    s += "\n";
    s += tmp;
}

void cell::dump_r_right(std::string& s) const
{
    assert(m_layer != 0);
    const std::string i = std::to_string(m_row);
    const std::string j = std::to_string(m_column);
    const std::string k = std::to_string(m_layer->id());
    const std::string jn= std::to_string(m_column + 1);

    std::string tmp = rr;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;
    tmp += devider;
    tmp += i;
    tmp += devider;
    tmp += jn;
    tmp += devider;
    tmp += k;

    tmp += space;
    tmp += c;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;
    tmp += _s;

    tmp += space;
    tmp += c;
    tmp += i;
    tmp += devider;
    tmp += jn;
    tmp += devider;
    tmp += k;
    tmp += _s;

    tmp += space;
    tmp += R;
    tmp += "rij"; //TODO

    s += "\n";
    s += tmp;
}

void cell::dump_r_bottom(std::string& s) const
{
    assert(m_layer != 0);
    const std::string i = std::to_string(m_row);
    const std::string j = std::to_string(m_column);
    const std::string k = std::to_string(m_layer->id());
    const std::string in= std::to_string(m_row + 1);

    std::string tmp = rr;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;
    tmp += devider;
    tmp += in;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;

    tmp += space;
    tmp += c;
    tmp += i;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;
    tmp += _s;

    tmp += space;
    tmp += c;
    tmp += in;
    tmp += devider;
    tmp += j;
    tmp += devider;
    tmp += k;
    tmp += _s;

    tmp += space;
    tmp += R;
    tmp += "rij"; //TODO

    s += "\n";
    s += tmp;
}

}
