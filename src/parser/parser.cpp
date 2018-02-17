#include "parser.hpp"

#include <QByteArray>
#include <QFile>
#include <QList>
#include <QString>

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

std::string parser::get_name(const data_map& m) const
{
    data_map::ConstIterator cit = m.find("name");
    if (cit == m.end()) {
        throw exception("Syntax error: name=<cell_name>");
    }
    return cit.value().toStdString();
}

double parser::get_value_of(const data_map& m, const std::string& n) const
{
    data_map::ConstIterator it = m.find(QString::fromStdString(n));
    if (it == m.end()) {
        throw exception("Syntax error: " + n + " =<value>");
    }
    return it.value().toDouble();
}

power_cell::position parser::get_position(const data_map& m) const
{
    data_map::ConstIterator it = m.find("pos");
    if (it == m.end()) {
        throw exception("Syntax error: pos=(<x>,<y>)");
    }
    QString pos = it.value();
    if (!pos.startsWith("(") ||
        !pos.endsWith(")") ||
        pos.count('(') != 1 ||
        pos.count(')') != 1) {
        throw exception("Syntax error: pos=(<x>,<y>)");
    }

    pos = pos.mid(1, pos.size() - 2);
    QString x_str = pos.section(',', 0, 0);
    QString y_str = pos.section(',', 1, 1);
    return power_cell::position(x_str.toDouble(), y_str.toDouble());
}

parser::power_cells parser::get_cells(const std::string& f)
{
    assert(!f.empty());
    power_cells cells;
    if (!QFile::exists(QString::fromStdString(f))) {
        return cells;
    }
    QFile file(QString::fromStdString(f));
    if (!file.open(QFile::ReadOnly)) {
        return cells;
    }

    QByteArray data = file.readLine();
    while( !data.isEmpty() ) {
        //Remove spaces
        data = data.simplified();
        if(data.isEmpty()) {
            continue;
        }

        //Get the specified assignments
        QList<QByteArray> parts = data.split( ';' );

        data_map m;
        foreach(QByteArray ba, parts) {
            QString baStr = QString(ba);
            baStr = baStr.remove(' ');
            //Get two parts of each assignment
            QStringList strList = baStr.split('=');
            //If there are more than one '=' character, it is incorrect syntax
            if(strList.count() != 2) {
                throw exception("Syntax error too many '='");
            }
            //Insert pair to map
            m.insert(strList[0], strList[1]);
        }

        const std::string name = get_name(m);
        power_cell::position p = get_position(m);
        double w = get_value_of(m, "width");
        double h = get_value_of(m, "height");
        double pw = get_value_of(m, "power");
        cells.push_back(power_cell(name, p, w, h, pw));

        // Next cell
        data = file.readLine();
    }

    return cells;
}

}
