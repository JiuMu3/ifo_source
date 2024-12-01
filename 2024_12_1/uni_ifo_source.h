#ifndef UNI_IFO_SOURCE_H
#define UNI_IFO_SOURCE_H
#include <ifo_source.h>
#include <QVector>
#include <QPair>

class uni_ifo_source
{
public:
    uni_ifo_source();
    uni_ifo_source(IFO_SOURCE<double, QString> p1,IFO_SOURCE<double, QString> p2);
    //初始化信源概率矩阵
    void INIT_UNI_MATRIX();
    //联合信源的采样
    QPair<QString,QString> SAMPLE_UNI_SOURCE();
    //计算联合熵
    double entropy_uni(int base=2);
    //计算条件熵
    double entropy_condition(int base=2);
    //返回矩阵
    QVector<QVector<double>> GET_UNI_SOURCE_MATRIX();

    int GET_UNI_SOURCE_FIRST_SIZE();
    int GET_UNI_SOURCE_SECOND_SIZE();
private:
    //联合信源概率矩阵
    QVector<QVector<double>> *uni_source_matrix=nullptr;
    QVector<QPair<QString,double>> uni_source_first;
    QVector<QPair<QString,double>> uni_source_second;
    int uni_source_first_size,uni_source_second_size;
};

#endif // UNI_IFO_SOURCE_H
