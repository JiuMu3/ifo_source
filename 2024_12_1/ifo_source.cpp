#include "ifo_source.h"
template <typename Prob,typename Symbol>
IFO_SOURCE<Prob,Symbol>::IFO_SOURCE()
{

}
//构造函数
template <typename Prob,typename Symbol>
IFO_SOURCE<Prob,Symbol>::IFO_SOURCE(const QVector<Symbol> &symbol_list, const QVector<Prob> &Prob_list)
{
    my_symbol_list=symbol_list;
    my_prob_list=Prob_list;
    typename QVector<Prob>::const_iterator it_prob=Prob_list.begin();
    for(typename QVector<Symbol>::const_iterator it=symbol_list.begin();it!=symbol_list.end();it++,it_prob++){
        my_list.insert(*it,*it_prob);
        my_map_list.append(QPair<Symbol,Prob>(*it,*it_prob));
        this->Symbol_size++;
    }
}
//访问map容器
template <typename Prob,typename Symbol>
QMap<Symbol, Prob> IFO_SOURCE<Prob,Symbol>::GET_MY_LIST()
{
    return my_list;
}

template<typename Prob, typename Symbol>
QVector<Symbol> IFO_SOURCE<Prob,Symbol>::GET_MY_SYMBOL_LIST()
{
    return this->my_symbol_list;
}

template<typename Prob, typename Symbol>
QVector<Prob> IFO_SOURCE<Prob,Symbol>::GET_MY_PROB_LIST()
{
    return this->my_prob_list;
}

template <typename Prob,typename Symbol>
double IFO_SOURCE<Prob,Symbol>::entropy(int base)
{
    double _result=0;
    typename QVector<QPair<Symbol,Prob>>::iterator it=my_map_list.begin();
    for(int i=0;i<this->Symbol_size;i++,it++){
        _result+=it->second*(qLn(it->second)/qLn(base));
    }
    return -_result;
}

template<typename Prob, typename Symbol>
void IFO_SOURCE<Prob,Symbol>::INIT_CLASS(QVector<Symbol> symbol_list, QVector<Prob> Prob_list)
{
    my_symbol_list=symbol_list;
    my_prob_list=Prob_list;
    typename QVector<Prob>::iterator it_prob=Prob_list.begin();
    for(typename QVector<Symbol>::iterator it=symbol_list.begin();it!=symbol_list.end();it++,it_prob++){
        my_list.insert(*it,*it_prob);
        my_map_list.append(QPair<Symbol,Prob>(*it,*it_prob));
        this->Symbol_size++;
    }
}
//采样函数
template <typename Prob,typename Symbol>
Symbol IFO_SOURCE<Prob,Symbol>::SAMPLE()
{
    Symbol symbol_symbol;
    double randomDouble = QRandomGenerator::global()->generateDouble();
    double temp=0;
    for(typename QVector<QPair<Symbol,Prob>>::iterator it=my_map_list.begin();it!=my_map_list.end();it++){
        temp+=it->second;
        if(temp>randomDouble){
            symbol_symbol=it->first;
            return symbol_symbol;
        }
    }
    return symbol_symbol;
}

template<typename Prob, typename Symbol>
void IFO_SOURCE<Prob,Symbol>::ADD_NEW_ELEM(Symbol sym_elem, Prob prob_elem)
{
    my_prob_list.append(prob_elem);
    my_symbol_list.append(sym_elem);
    my_map_list.append(QPair<Symbol,Prob>(sym_elem,prob_elem));
    my_list.insert(sym_elem,prob_elem);
    this->Symbol_size++;
}

template<typename Prob, typename Symbol>
void IFO_SOURCE<Prob,Symbol>::PRINT_IFO_SOURCE()
{
    PRINT_IFO_MY_LIST();
    PRINT_IFO_MY_MAP_LIST();
    PRINT_IFO_MY_SYM_LIST();
    PRINT_IFO_MY_PROB_LIST();
}

template<typename Prob, typename Symbol>
void IFO_SOURCE<Prob,Symbol>::PRINT_IFO_MY_LIST()
{
    qDebug() << "MY_LIST:";
    for (auto it = my_list.constBegin(); it != my_list.constEnd(); ++it) {
        qDebug() << it.key() << ":" << it.value(); // 输出键和值
    }
}

template<typename Prob, typename Symbol>
void IFO_SOURCE<Prob,Symbol>::PRINT_IFO_MY_MAP_LIST()
{
    qDebug()<<"MY_MAP_LIST"<<this->my_map_list;
}

template<typename Prob, typename Symbol>
void IFO_SOURCE<Prob,Symbol>::PRINT_IFO_MY_SYM_LIST()
{
    qDebug()<<"MY_SYM_LIST:"<<this->my_symbol_list;
}

template<typename Prob, typename Symbol>
void IFO_SOURCE<Prob,Symbol>::PRINT_IFO_MY_PROB_LIST()
{
    qDebug()<<"MY_PROB_LIST:"<<this->my_prob_list;
}

template <typename Prob,typename Symbol>
IFO_SOURCE<Prob,Symbol>::~IFO_SOURCE()
{

}
