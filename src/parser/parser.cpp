#include "parser.hpp"

#include <core/ic.hpp>
#include <core/layer.hpp>

#include <QByteArray>
#include <QFile>
#include <QList>
#include <QString>

#include <map>

#include <cassert>
#include <iostream>

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
        throw exception("Syntax error: " + n + "=<value>");
    }
    return it.value().toDouble();
}

double parser::get_frequency(const data_map& m) const
{
    data_map::ConstIterator it = m.find(QString::fromStdString("frequency"));
    if (it == m.end()) {
        return -1;
    }
    return it.value().toDouble();
}

unsigned parser::get_layer(const data_map& m) const
{
    data_map::ConstIterator it = m.find("layer");
    if (it == m.end()) {
        throw exception("Syntax error: layer=<value>");
    }
    return it.value().toUInt();
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
    int rows = 0;
    while( !data.isEmpty() ) {
        //Remove spaces
        data = data.simplified();
        if(data.isEmpty()) {
            break;
        }
        ++rows;
        //Get the specified assignments
        QList<QByteArray> parts = data.split( ';' );

        data_map m;
        foreach(QByteArray ba, parts) {
            QString baStr = QString(ba);
            baStr = baStr.remove(' ');
            if (baStr.isEmpty()) {
                break;
            }
            std::cout<<baStr.toStdString()<<std::endl;
            //Get two parts of each assignment
            QStringList strList = baStr.split('=');
            //If there are more than one '=' character, it is incorrect syntax
            if(strList.count() != 2) {
                foreach (auto v, strList) {
                    std::cout<<v.toStdString()<<std::endl;
                }
                throw exception("Syntax error too many '=' " + QString::number(rows).toStdString() + " " + QString::number(strList.count()).toStdString() + baStr.toStdString());
            }
            //Insert pair to map
            m.insert(strList[0], strList[1]);
        }

        const std::string name = get_name(m);
        power_cell::position p = get_position(m);
        double w = get_value_of(m, "width");
        double h = get_value_of(m, "height");
        double pw = get_value_of(m, "power");
        double fr = get_frequency(m);
        unsigned l = get_layer(m);
        cells.push_back(power_cell(name, p, w, h, pw, l, fr));

        // Next cell
        data = file.readLine();
    }

    return cells;
}

struct indexs
{
    unsigned row;
    unsigned column;
    unsigned level;

    indexs(unsigned r,
           unsigned c,
           unsigned l)
        : row(r)
        , column(c)
        , level(l)
    {}

    bool operator<(const indexs& n) const
    {
        if (level < n.level) {
            return true;
        }
        if (level > n.level) {
            return false;
        }
        if (row < n.row) {
            return true;
        }
        if (row > n.row) {
            return false;
        }
        if (column < n.column) {
            return true;
        }
        if (column > n.column) {
            return false;
        }
        return false;
    }
};

core::ic* parser::get_ic(const std::string& fn)
{
    if (fn.empty()) {
        return 0;
    }
    QFile file(QString::fromStdString(fn));
    if (!file.open(QFile::ReadOnly)) {
        return 0;
    }

    unsigned maxR = 0, maxC = 0, maxL = 0;

//    using map = std::map<indexs, double>;
//    map temp_map;

    QByteArray data = file.readLine();
    while( !data.isEmpty() ) {
        //Go to the next line
        data = file.readLine();

        //Remove redundant spaces
        data = data.simplified();
        if (data.isEmpty()) {
            break;
        }

        if ( data.startsWith('*') ) {
            continue;
        }

        int idx = data.indexOf("_c");
        if ( idx == -1 ) {
            continue;
        }
        idx += 2;

        QString dataStr = QString(data);
        dataStr.insert(idx, '=');
        dataStr = dataStr.remove(' ');
        QList<QString> parts = dataStr.split( '=' );
        QStringList index = parts[1].split("_");
        if (index.back() == "s") {
            continue;
        }
        assert(index.size() == 4);
        unsigned r = 0, c = 0, l = 0;
        r = index[1].toUInt();
        c = index[2].toUInt();
        l = index[3].toUInt();
        if (maxR < r) {
            maxR = r;
        }
        if (maxC < c) {
            maxC = c;
        }
        if (maxL < l) {
            maxL = l;
        }
        //            qreal Iidx = parts[1].toDouble();
/*        QString baStr = parts[2];
        qreal temp;
        /// mikro
        if (baStr.endsWith('k')) {
            baStr = baStr.remove('k');
            temp = baStr.toDouble()*1000;
        } else if (baStr.endsWith('m')) {
            baStr = baStr.remove('m');
            temp = baStr.toDouble()*1000/2;
        } else if (baStr.endsWith('u')) {
            baStr = baStr.remove('u');
            temp = baStr.toDouble()*20;
        } else if (baStr.endsWith('n')) {
            baStr = baStr.remove('n');
            temp = baStr.toDouble()/1000;
        } else if(baStr.endsWith('p')) {
            baStr = baStr.remove('p');
            temp = baStr.toDouble()/1000000;
        } else {
            temp = baStr.toDouble();//*1000000;
        }*/
//        temp_map[indexs(r, c, l)] = temp;
//        tempMap.insert(Iidx, temp); TODO
    }

    file.close();

    core::ic* ic = new core::ic(maxL + 1, maxC + 1, maxR + 1);

/*    for (map::iterator i = temp_map.begin(); i != temp_map.end(); ++i) {
        core::layer* l = ic->get_layer(i->first.level);
        assert(l != 0);
        std::cout<<i->first.row<<std::endl;
        std::cout<<i->first.column<<std::endl;
        l->set_cell_value(i->first.row, i->first.column, i->second);
    }*/
    if (!file.open(QFile::ReadOnly)) {
        return 0;
    }


//    using map = std::map<indexs, double>;
//    map temp_map;

    data = file.readLine();
    while( !data.isEmpty() ) {
        //Go to the next line
        data = file.readLine();

        //Remove redundant spaces
        data = data.simplified();
        if (data.isEmpty()) {
            break;
        }

        if ( data.startsWith('*') ) {
            continue;
        }

        int idx = data.indexOf("_c");
        if ( idx == -1 ) {
            continue;
        }
        idx += 2;

        QString dataStr = QString(data);
        dataStr.insert(idx, '=');
        dataStr = dataStr.remove(' ');
        QList<QString> parts = dataStr.split( '=' );
        QStringList index = parts[1].split("_");
        if (index.back() == "s") {
            continue;
        }
        assert(index.size() == 4);
        unsigned r = 0, c = 0, l = 0;
        r = index[1].toUInt();
        c = index[2].toUInt();
        l = index[3].toUInt();
        QString baStr = parts[2];
        qreal temp;
        /// mikro
        if (baStr.endsWith('k')) {
            baStr = baStr.remove('k');
            temp = baStr.toDouble()*1000;
        } else if (baStr.endsWith('m')) {
            baStr = baStr.remove('m');
            temp = baStr.toDouble()*1000/2;
        } else if (baStr.endsWith('u')) {
            baStr = baStr.remove('u');
            temp = baStr.toDouble()*20;
        } else if (baStr.endsWith('n')) {
            baStr = baStr.remove('n');
            temp = baStr.toDouble()/1000;
        } else if(baStr.endsWith('p')) {
            baStr = baStr.remove('p');
            temp = baStr.toDouble()/1000000;
        } else {
            temp = baStr.toDouble();//*1000000;
        }
        core::layer* la = ic->get_layer(l);
        assert(la != 0);
        //std::cout<<r<<std::endl;
        //std::cout<<c<<std::endl;
        la->set_cell_value(r, c, temp);
//        temp_map[indexs(r, c, l)] = temp;
//        tempMap.insert(Iidx, temp); TODO
    }
    return ic;
//    qreal fmintemp = 0;
//    qreal fmaxtemp = 0;


/*    //Fill tempVector.
    QVector<qreal> tempVector;
    qreal maxTemp = 0;
    qreal minTemp = 1000000000000;
    QMap<int, qreal>::const_iterator i = tempMap.constBegin();
    QMap<int, qreal>::const_iterator iEnd = tempMap.constEnd();
    while (i != iEnd)
    {
        qreal temp = i.value();
        if(temp > maxTemp)
            maxTemp = temp;

        if(temp < minTemp)
            minTemp = temp;
        tempVector.append(temp);
        i+=2;
    }

    mMaxTemp = maxTemp;
    qDebug() << "mMaxTemp ====" << maxTemp << "\n\n";
    qDebug() << "mMinTemp ====" << minTemp << "\n\n";

    QList<QGraphicsItem *> rectItems = pScene->items();
    foreach(QGraphicsItem* grItem, rectItems)
    {
        Node* node = dynamic_cast<Node*>(grItem);
        if(node)
        {
            RectNode* rNode = new RectNode;
            rNode->m_point = node->m_point;
            rNode->m_hight = node->m_hight;
            rNode->m_weight = node->m_weight;
            rNode->m_name = node->m_name;
            rNode->m_power = node->m_power;

            rNode->setPos(rNode->m_point);
            scene->addItem(rNode);
            scene->clearSelection();
            rNode->setSelected(true);

            nodeList.append(rNode);
        }
    }

    QRectF bRect = scene->itemsBoundingRect();
    QPointF distPoint = QPoint(20,20);
    QRectF cbRect(bRect.topLeft()-distPoint, bRect.bottomRight()+distPoint);

    scene->setSceneRect(bRect);

    int itStep = GraphicsScene::GRID_STEP;
    qreal xStart = cbRect.topLeft().x();
    qreal xEnd = cbRect.topRight().x()-itStep;
    qreal yStart = cbRect.topLeft().y();
    qreal yEnd = cbRect.bottomLeft().y()-itStep;

    int tidx = 0;

    qreal factor =( cbRect.height()*cbRect.width())/(itStep*itStep);

    for(qreal x = xStart; x <= xEnd; x += itStep)
    {
        for(qreal y = yStart; y <= yEnd; y += itStep)
        {
            TempNode* tempNode = new TempNode;
            QRectF gridRect(x, y, itStep, itStep);
            tempNode->m_rect = gridRect;

            QColor color;
            //double h = m_power*256;
            qreal theTemp = tempVector[tidx];

            if(mMaxTemp)
            {
                qreal h = theTemp/mMaxTemp * 256;
                color.setHsl(256-h, 240, 140);
                tempNode->m_color = color;
                tempNode->m_temp = theTemp*100/factor;
                tempNode->m_MaxTemp = mMaxTemp;
            }

            scene->addItem(tempNode);
            violList.append(tempNode);

            ++tidx;
        }
    }

    fmintemp = minTemp*100/factor;
    fmaxtemp = maxTemp*100/factor;

    int row = 0;

    qreal temp_diff = (fmaxtemp - fmintemp)/14.0;

    for (qreal pdval = temp_diff+2; pdval <= fmaxtemp; pdval += temp_diff)
    {
        if(pdval== 0)
            continue;
        int pval = pdval;
        qreal dist = fmaxtemp - fmintemp;

        // For dock widget
        QColor theColor;
        qreal theTemp = (pval-fmintemp)/(fmaxtemp-fmintemp) * 256;
        //theColor.setHsl(255-thePower, 255-thePower, 160);
        theColor.setHsl(256-theTemp, 240, 140);

        model->insertRows(row, 1, QModelIndex());

        model->setData(model->index(row, 0, QModelIndex()),
                       theColor, Qt::DecorationRole);
        model->setData(model->index(row, 0, QModelIndex()),
                       pval);

        row++;
    }*/
}


}
