#ifndef IFO_SOURCE_H
#define IFO_SOURCE_H

#include <QMap>
#include <QVector>
#include <QPair>
#include <QRandomGenerator>
#include <QtMath>
#include <qmath.h>
#include <QDebug>
template <typename Prob,typename Symbol>
class IFO_SOURCE
{
public:
    IFO_SOURCE();
    IFO_SOURCE(const QVector<Symbol> &symbol_list, const QVector<Prob> &Prob_list);
    QMap<Symbol,Prob> GET_MY_LIST();
    QVector<QPair<Symbol,Prob>> GET_MY_MAP_LIST()
    {
            return my_map_list;
    }
    bool isempty(){
        return my_symbol_list.empty();
    }
    QVector<Symbol> GET_MY_SYMBOL_LIST();
    QVector<Prob> GET_MY_PROB_LIST();
    double entropy(int base=2);
    void INIT_CLASS(QVector<Symbol> symbol_list,QVector<Prob> Prob_list);
    Symbol SAMPLE();
    //添加新元素进入列表
    void ADD_NEW_ELEM(Symbol sym_elem,Prob prob_elem);

    //打印测试函数
    void PRINT_IFO_SOURCE();
    void PRINT_IFO_MY_LIST();
    void PRINT_IFO_MY_MAP_LIST();
    void PRINT_IFO_MY_SYM_LIST();
    void PRINT_IFO_MY_PROB_LIST();

    int Symbol_size=0;
    ~IFO_SOURCE();
private:
    QMap<Symbol,Prob> my_list;
    QVector<Symbol> my_symbol_list;
    QVector<Prob> my_prob_list;
    QVector<QPair<Symbol,Prob>> my_map_list;
};

#endif // IFO_SOURCE_H
