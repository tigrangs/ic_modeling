#include "genetic_improvement.hpp"
#include "matrix_cell.hpp"

#include <core/submatrix.hpp>

#include <QRandomGenerator>
#include <QDebug>
#include <QTime>

#include <iostream>
#include <cassert>

namespace controller {

genetic_improvement::genetic_improvement(matrix_layer* l, unsigned size)
    : m_root_matrix(l)
    , m_submatrix_size(size)
    , m_max_value(-1)
    , m_stop(false)
    , count(0)
{
    assert(m_root_matrix != 0);
}

matrix_layer* genetic_improvement::run()
{
    assert(m_root_matrix != 0);
    for (unsigned i = 0; i < m_submatrix_size*m_submatrix_size; ++i) {
        m_parents.push_back(m_root_matrix->clone());
    }
    do {
        for (auto matrix: m_parents) {
            assert(matrix != 0);
            matrix->generate_submatrixes(m_submatrix_size);
            matrix_layer::submatrixes& subs = matrix->get_submatrixes();
            swap_items(subs);
        }
        double curent_max = remove_matrixes();
        if (count >= 10000 || (qAbs(m_max_value - curent_max) < 0.1 && m_max_value != curent_max)) {
            m_stop = true;
            std::cout<<"MAX_value "<<m_max_value<<std::endl;
            std::cout<<"MAX "<<curent_max<<std::endl;
        } else {
            std::cout<<"MAX_value "<<m_max_value<<std::endl;
            std::cout<<"MAX "<<curent_max<<std::endl;
            m_max_value = curent_max;
        }
        ++count;
        //        check_improvement(subs);
    } while(!m_stop);
    return m_parents[0];
}

double genetic_improvement::remove_matrixes()
{
    matrix_layer::submatrixes& subs = m_parents[0]->get_submatrixes();
    double max = subs[0][0]->get_sum();
    // Finding Wmax and Wmin
    for (matrix_layer::submatrixes::iterator i = subs.begin(); i != subs.end(); ++i) {
        for (std::vector<core::submatrix*>::iterator j = i->begin(); j != i->end(); ++j) {
            assert(*j != 0);
            double current = (*j)->get_sum();
            if (max < current) {
                max = current;
            }
        }
    }
    double min_max = max;
    matrix_layer* min_layer = m_parents[0];



    for (auto matrix: m_parents) {
        assert(matrix != 0);
        matrix_layer::submatrixes& subs = matrix->get_submatrixes();
        double max = subs[0][0]->get_sum();
        // Finding Wmax and Wmin
        for (matrix_layer::submatrixes::iterator i = subs.begin(); i != subs.end(); ++i) {
            for (std::vector<core::submatrix*>::iterator j = i->begin(); j != i->end(); ++j) {
                assert(*j != 0);
                double current = (*j)->get_sum();
                if (max < current) {
                    max = current;
                }
            }
        }
        if (min_max > max) {
            min_max = max;
            min_layer = matrix;
        }
    }
    assert(min_max != -1);
    assert(min_layer != 0);
    matrixes::iterator it = std::find(m_parents.begin(), m_parents.end(), min_layer);
    assert(it != m_parents.end());
    m_parents.erase(it);
    for (auto m : m_parents) {
        delete m;
    }
    m_parents.clear();
    for (unsigned i = 0; i < m_submatrix_size*m_submatrix_size; ++i) {
        m_parents.push_back(min_layer->clone());
    }
    return min_max;
}

void genetic_improvement::swap_items(matrix_layer::submatrixes& subs)
{
    double min = subs[0][0]->get_sum();
    double max = subs[0][0]->get_sum();
    core::submatrix* min_sub = subs[0][0];
    core::submatrix* max_sub = subs[0][0];
    // Finding Wmax and Wmin
    for (matrix_layer::submatrixes::iterator i = subs.begin(); i != subs.end(); ++i) {
        for (std::vector<core::submatrix*>::iterator j = i->begin(); j != i->end(); ++j) {
            assert(*j != 0);
            unsigned current = (*j)->get_sum();
            if (min >= current) {
                min = current;
                min_sub = *j;
            }
            if (max <= current) {
                max = current;
                max_sub = *j;
            }
        }
    }
/*    if (m_min_max_value > max) {
        m_min_max_value = max;
    }*/
    m_max_value = max;
    std::cout<<"SUB_SIZE "<<subs.size()<<" "<<subs[0].size()<<std::endl;
    assert(min_sub != 0);
    assert(max_sub != 0);
    //Finding max from Wmax and min from Wmin
    std::shared_ptr<core::cell> min_cell = min_sub->get_min_cell();
    std::shared_ptr<core::cell> max_cell = max_sub->get_max_cell();
    assert(min_cell != 0);
    assert(max_cell != 0);
    std::shared_ptr<matrix_cell> matrix_max_cell = std::static_pointer_cast<matrix_cell>(max_cell);
    std::shared_ptr<matrix_cell> matrix_min_cell = std::static_pointer_cast<matrix_cell>(min_cell);
    assert(matrix_min_cell != 0);
    assert(matrix_max_cell != 0);

    //SWAP max to Wmin

    //TMP values from max
    double tmp_max_value = matrix_max_cell->value();
    double tmp_max_value1 = matrix_max_cell->value_1();
    unsigned tmp_row = matrix_max_cell->source_row();
    unsigned tmp_column = matrix_max_cell->source_column();
    QGraphicsRectItem* tmp_item = matrix_max_cell->get_item();

    // Get swap cell from Wmin
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    std::shared_ptr<matrix_cell> tmp_min_cell = 0;
    do {
        unsigned index = (qrand() % (m_submatrix_size*m_submatrix_size));
        assert(index < min_sub->get_cells().size());
        tmp_min_cell = std::static_pointer_cast<matrix_cell>(min_sub->get_cells()[index]);
        std::cout<<"Index: "<<index<<std::endl;
    } while (tmp_min_cell == 0 || tmp_min_cell == matrix_min_cell || tmp_min_cell == matrix_max_cell);

    //Swap max from Wmax and tmp_min_cell from Wmin
    matrix_max_cell->set_value(tmp_min_cell->value());
    matrix_max_cell->set_value_1(tmp_min_cell->value_1());
    matrix_max_cell->set_source_position(tmp_min_cell->source_row(), tmp_min_cell->source_column());
    matrix_max_cell->add_item(tmp_min_cell->get_item());
    matrix_max_cell->notify_all();
    tmp_min_cell->set_value(tmp_max_value);
    tmp_min_cell->set_value_1(tmp_max_value1);
    tmp_min_cell->set_source_position(tmp_row, tmp_column);
    tmp_min_cell->add_item(tmp_item);
    tmp_min_cell->notify_all();

    qDebug()<<QString::fromStdString("SWAPED");
    qDebug()<<QString::number(matrix_max_cell->source_row());
    qDebug()<<QString::number(matrix_max_cell->source_column());
    qDebug()<<QString::number(tmp_min_cell->source_row());
    qDebug()<<QString::number(tmp_min_cell->source_column());

    //SWAP min to Wmax

    //TMP values from min
    double tmp_min_value = matrix_min_cell->value();
    double tmp_min_value1 = matrix_min_cell->value_1();
    tmp_row = matrix_min_cell->source_row();
    tmp_column = matrix_min_cell->source_column();
    tmp_item = matrix_min_cell->get_item();

    // Get swap cell from Wmax
    time = QTime::currentTime();
    qsrand((uint)time.msec());
    std::shared_ptr<matrix_cell> tmp_max_cell = 0;
    do {
        unsigned index = (qrand() % (m_submatrix_size*m_submatrix_size));
        assert(index < max_sub->get_cells().size());
        tmp_max_cell = std::static_pointer_cast<matrix_cell>(max_sub->get_cells()[index]);
    } while (tmp_max_cell == 0 || tmp_max_cell == matrix_max_cell || tmp_max_cell == matrix_min_cell);

    //Swap max from Wmax and tmp_min_cell from Wmin
    matrix_min_cell->set_value(tmp_max_cell->value());
    matrix_min_cell->set_value_1(tmp_max_cell->value_1());
    matrix_min_cell->set_source_position(tmp_max_cell->source_row(), tmp_max_cell->source_column());
    matrix_min_cell->add_item(tmp_max_cell->get_item());
    matrix_min_cell->notify_all();
    tmp_max_cell->set_value(tmp_min_value);
    tmp_max_cell->set_value_1(tmp_min_value1);
    tmp_max_cell->set_source_position(tmp_row, tmp_column);
    tmp_max_cell->add_item(tmp_item);
    tmp_max_cell->notify_all();

    qDebug()<<QString::fromStdString("SWAPED");
    qDebug()<<QString::number(matrix_min_cell->source_row());
    qDebug()<<QString::number(matrix_min_cell->source_column());
    qDebug()<<QString::number(tmp_max_cell->source_row());
    qDebug()<<QString::number(tmp_max_cell->source_column());
}

void genetic_improvement::check_improvement(matrix_layer::submatrixes & subs)
{
    double max = subs[0][0]->get_sum();
    // Finding Wmax and Wmin
    for (matrix_layer::submatrixes::iterator i = subs.begin(); i != subs.end(); ++i) {
        for (std::vector<core::submatrix*>::iterator j = i->begin(); j != i->end(); ++j) {
            assert(*j != 0);
            double current = (*j)->get_sum();
            if (max < current) {
                max = current;
            }
        }
    }

    if (count >= 10000 || (qAbs(m_max_value - max) < 0.1 && m_max_value != max) /*(m_max_value < max || m_max_value - max < 0.3) && m_max_value != max)*/) {
        m_stop = true;
        std::cout<<"MAX_value "<<m_max_value<<std::endl;
        std::cout<<"MAX "<<max<<std::endl;
    } else {
        std::cout<<"MAX_value "<<m_max_value<<std::endl;
        std::cout<<"MAX "<<max<<std::endl;
        m_max_value = max;
    }
    ++count;
}

}
