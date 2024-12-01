#include "uni_ifo_source.h"

uni_ifo_source::uni_ifo_source()
{

}

uni_ifo_source::uni_ifo_source(IFO_SOURCE<double, QString> p1, IFO_SOURCE<double, QString> p2)
{
    uni_source_first=p1.GET_MY_MAP_LIST();
    uni_source_second=p2.GET_MY_MAP_LIST();
    uni_source_first_size=p1.Symbol_size;
    uni_source_second_size=p2.Symbol_size;
    INIT_UNI_MATRIX();
}

void uni_ifo_source::INIT_UNI_MATRIX()
{  
    if(uni_source_matrix==nullptr){
        uni_source_matrix=new QVector<QVector<double>>(uni_source_first_size, QVector<double>(uni_source_second_size, 0.0));
    }
    else{
        delete uni_source_matrix;
        uni_source_matrix=nullptr;
        uni_source_matrix=new QVector<QVector<double>>(uni_source_first_size, QVector<double>(uni_source_second_size, 0.0));
    }
    for(int i=0;i<uni_source_first_size;i++){
        for(int j=0;j<uni_source_second_size;j++){
            (*uni_source_matrix)[i][j]=uni_source_first[i].second*uni_source_second[j].second;
        }
    }
}

QPair<QString, QString> uni_ifo_source::SAMPLE_UNI_SOURCE()
{
    QString symbol_symbol_1;
    QString symbol_symbol_2;
    double randomDouble = QRandomGenerator::global()->generateDouble();
    double temp=0;
    for(int i=0;i<this->uni_source_first_size;i++){
        temp+=this->uni_source_first[i].second;
        if(temp>randomDouble){
            symbol_symbol_1=uni_source_first[i].first;
            break;
        }
    }

    randomDouble = QRandomGenerator::global()->generateDouble();
    temp=0;
    for(int i=0;i<this->uni_source_second_size;i++){
        temp+=this->uni_source_second[i].second;
        if(temp>randomDouble){
            symbol_symbol_2=uni_source_second[i].first;
            break;
        }
    }

    return {symbol_symbol_1,symbol_symbol_2};
}

double uni_ifo_source::entropy_uni(int base)
{
    double _result = 0.0;

    for (const auto& innerVector : (*uni_source_matrix)) {
        for (const auto& temp : innerVector) {
            if (temp > 0) {  // 确保temp为有效概率
                _result += temp * (qLn(temp) / qLn(base));
            }
        }
    }
    return -_result;  // 返回负值以符合熵的定义
}
//计算条件熵
double uni_ifo_source::entropy_condition(int base)
{
    double conditional_entropy = 0.0;

    for (int i = 0; i < uni_source_first_size; i++) {
        double p_y = uni_source_first[i].second;  // P(Y=y)

        if (p_y > 0) {
            for (int j = 0; j < uni_source_second_size; j++) {
                double p_xy = (*uni_source_matrix)[i][j];  // 联合概率 P(X=x, Y=y)

                if (p_xy > 0) {
                    double p_x_given_y = p_xy / p_y;  // 条件概率 P(X=x|Y=y)
                    conditional_entropy -= p_xy * (qLn(p_x_given_y) / qLn(base));
                }
            }
        }
    }
    return conditional_entropy;
}

QVector<QVector<double> > uni_ifo_source::GET_UNI_SOURCE_MATRIX()
{
    QVector<QVector<double>> temp=*uni_source_matrix;
    return temp;
}

int uni_ifo_source::GET_UNI_SOURCE_FIRST_SIZE()
{
    return uni_source_first_size;
}

int uni_ifo_source::GET_UNI_SOURCE_SECOND_SIZE()
{
    return uni_source_second_size;
}
