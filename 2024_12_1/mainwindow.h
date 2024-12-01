#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMath>
#include <ifo_source.h>
#include <QDebug>
#include <QVector>
#include <QTableWidget>
#include "ifo_source.cpp"
#include <uni_ifo_source.h>
#include <QMessageBox>
#include <ifo_code.h>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    IFO_SOURCE<double,QString> *My_Source=nullptr;
    uni_ifo_source *My_Uni_Source=nullptr;
    ifo_code *My_Code=nullptr;
    std::map<char, std::string> encoding;
    void Plain_Text_My_Code();
    //检测编码定长编码还是奇异码
    std::pair<bool, bool> checkEncoding(const std::map<char, std::string>& encoding);
    std::pair<bool, bool> checkEncoding_Now(const std::map<char, std::string>& encoding);
    bool sardinasPatterson(const vector<string>& code);
    // 判断一个集合是否包含空串
    bool containsEmptyString(const set<string>& S) {
        return S.find("") != S.end();
    }
    //信源观测函数
    template<typename Prob, typename Symbol>
    int OBSERVE_IFO_SOURCE(IFO_SOURCE<Prob, Symbol> *TEMP_IFO_SOURCE,QPair<QVector<QPair<Symbol, Prob>>,double> &temp,int base=2)
    {
        if(TEMP_IFO_SOURCE==nullptr){
            return -1;
        }
        QVector<QPair<Symbol, Prob>> temp_vector= TEMP_IFO_SOURCE->GET_MY_MAP_LIST();
        temp.first=temp_vector;
        temp.second=TEMP_IFO_SOURCE->entropy(base);
        return 1;
    }
    //更新table widget窗口
    void update_table();
    //存储任务二里面tablewidget的内容
    QVector<QTableWidgetItem *> qtable_list_sym;
    QVector<QTableWidgetItem *> qtable_list_pro;

    //初始化信源类封装
    int init_single_source(QVector<QString> &_Symbol_list,QVector<double> &_Prob_list,QTableWidget *TABLE_WIDGET);
    //封装输出的qstring
    QString init_plaintext_str(QString old_str);
    //输出联合信源矩阵
    void print_uni_source_matrix();
    //提示窗口
    void tips();
    ~MainWindow();

private slots:
    void on_pro1_add_line_clicked();

    void on_pro1_init_source_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

//    void on_pushButton_4_clicked();

    void on_pro1_delete_line_clicked();

    void on_pro1_source_list_cellChanged(int row, int column);

    void do_delete_add_list_line();
    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_action_output_ifo_triggered();

    void on_action_input_ifo_triggered();

    void on_action_output_uni_triggered();

    void on_action_input_uni_triggered();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_23_clicked();

signals:
    void delete_add_list_line();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
