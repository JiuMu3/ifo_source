#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->width()*1.2,this->height()*1.2);

    this->setWindowTitle("信息论与编码综合训练");
    connect(this,&MainWindow::delete_add_list_line,this,&MainWindow::do_delete_add_list_line);
    ui->radioButton->setChecked(true);
    ui->radioButton_3->setChecked(true);

    ui->pro1_source_list->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}


void MainWindow::update_table()
{
    //不为空清空列表
    if(!qtable_list_sym.empty()||!qtable_list_pro.empty()){
        for(int i=0;i<qtable_list_sym.size();i++){
            delete qtable_list_sym[i];
            delete qtable_list_pro[i];
        }
        qtable_list_sym.clear();
        qtable_list_pro.clear();

        //先清空列表
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
    }

    for(int i=0;i<this->My_Source->Symbol_size;i++){
        if(i+1>=ui->tableWidget->rowCount()){
//            qDebug()<<"新建行"<<i+1;
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        }
        this->qtable_list_sym.append(new QTableWidgetItem(this->My_Source->GET_MY_SYMBOL_LIST()[i]));
        this->qtable_list_pro.append(new QTableWidgetItem(QString::number(this->My_Source->GET_MY_PROB_LIST()[i])));

        ui->tableWidget->setItem(i,0,qtable_list_sym[i]);
        ui->tableWidget->setItem(i,1,qtable_list_pro[i]);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pro1_add_line_clicked()
{
    int newCow=ui->pro1_source_list->rowCount();
    ui->pro1_source_list->insertRow(newCow);
    emit delete_add_list_line();
}

int MainWindow::init_single_source(QVector<QString> &_Symbol_list, QVector<double> &_Prob_list, QTableWidget *TABLE_WIDGET)
{
    double total_Prob=0;
    for(int i=0;i<TABLE_WIDGET->rowCount();++i){
        for(int j=0;j<TABLE_WIDGET->columnCount();++j){
                QTableWidgetItem* item= TABLE_WIDGET->item(i,j);
                if(item){
                    if(j){
                            _Prob_list.append(item->text().toDouble());
                            total_Prob+=item->text().toDouble();
                    }
                    else{
                            _Symbol_list.append(item->text());
                    }
                }
                else{
                    //存在空格
                    return 0;
                }
        }
    }

    //判断里面是否有重复元素
    for(int i=0;i<TABLE_WIDGET->rowCount();i++){
        for(int j=i;j<TABLE_WIDGET->rowCount()-i-1;j++){
            if(_Symbol_list[j]==_Symbol_list[j+1]){
                return -1;
            }
        }
    }
    //概率和不为1
    if(qAbs(total_Prob - 1.0) > 0.000001){
        return -2;
    }

    //一切正常
    return 1;
}

QString MainWindow::init_plaintext_str(QString old_str)
{
    QString temp="===============\r\n";
    temp+=old_str+"\r\n";
    temp+="===============\r\n";
    return temp;
}

void MainWindow::on_pro1_init_source_clicked()
{
    QVector<QString> _Symbol_list;
    QVector<double> _Prob_list;

    int result_init=init_single_source(_Symbol_list,_Prob_list,ui->pro1_source_list);
    if(result_init==0){
        ui->pro1_tips_1->setText("不能有空格");
        return;
    }
    else if(result_init==-1){
        ui->pro1_tips_1->setText("符号存在重复元素");
        return;
    }
    else if(result_init==-2){
        ui->pro1_tips_1->setText("概率和不为1");
        return;
    }

    if(this->My_Source==nullptr){
        this->My_Source=new IFO_SOURCE<double,QString>(_Symbol_list,_Prob_list);
        this->My_Source->PRINT_IFO_SOURCE();
    }
    else{
        delete My_Source;
        this->My_Source=new IFO_SOURCE<double,QString>(_Symbol_list,_Prob_list);
        this->My_Source->PRINT_IFO_SOURCE();
    }
    ui->label->setText("已保存修改");
    ui->pro1_tips_1->setText("初始化完成");
}

void MainWindow::on_pro1_delete_line_clicked()
{
    ui->pro1_source_list->removeRow(ui->pro1_source_list->rowCount()-1);
    //？
    on_pro1_init_source_clicked();
    emit delete_add_list_line();
}

void MainWindow::on_pro1_source_list_cellChanged(int row, int column)
{
    Q_UNUSED(row)
    Q_UNUSED(column)
    ui->label->setText("未保存");
}

void MainWindow::do_delete_add_list_line()
{
    ui->label->setText("未保存");
}

void MainWindow::on_pushButton_5_clicked()
{
    if(this->My_Source==nullptr){
        ui->pro1_tips_1->setText("当前信源为空！");
        return;
    }
  QString temp_str= this->My_Source->SAMPLE();
  ui->lineEdit->setText(temp_str);
}

void MainWindow::on_pushButton_6_clicked()
{
    if(this->My_Source==nullptr){
        ui->pro1_tips_1->setText("信源未初始化！");
        return;
    }
    int base=2;
    if(ui->radioButton->isChecked()) base=2;
    else if(ui->radioButton_2->isChecked()) base=10;

    double temp_double= this->My_Source->entropy(base);
    QString temp_str= QString::number(temp_double);
    ui->lineEdit_2->setText(temp_str);
}


void MainWindow::on_pushButton_7_clicked()
{
    QPair<QVector<QPair<QString, double>>,double> temp;
    int RETURN_SULT= this->OBSERVE_IFO_SOURCE<double,   QString>(this->My_Source,temp);
    if(RETURN_SULT==-1){
        ui->label_5->setText("信源未初始化！");
        return;
    }
    QVector<QPair<QString, double>> temp_vector=temp.first;
    QString temp_str="=====================================\r\n";
    temp_str=temp_str+"当前信源符号全集："+"\r\n";
    for(int i=0;i<this->My_Source->Symbol_size;i++){
        temp_str+=temp_vector[i].first+"\t";
        temp_str+=QString::number(temp_vector[i].second)+"\r\n";
    }
    temp_str+="=====================================\r\n";
    ui->plainTextEdit->appendPlainText(temp_str);
    ui->label_5->setText("获取信源符号全集列表成功！");
    this->update_table();
    return;
}

void MainWindow::on_pushButton_4_clicked()
{
    QPair<QVector<QPair<QString, double>>,double> temp;

    int base=2;
    if(ui->radioButton->isChecked()) base=2;
    else if(ui->radioButton_2->isChecked()) base=10;

    int RETURN_SULT= this->OBSERVE_IFO_SOURCE<double,   QString>(this->My_Source,temp,base);
    if(RETURN_SULT==-1){
        ui->label_5->setText("信源未初始化！");
        return;
    }
    ui->lineEdit_3->setText(QString::number(temp.second));
    QString temp_str="=====================================\r\n";
    temp_str=temp_str+"当前信源熵："+QString::number(temp.second)+"\r\n";
    temp_str+="=====================================\r\n";
    ui->plainTextEdit->appendPlainText(temp_str);
    ui->label_5->setText("获取信源熵成功！");
    return;
}
//项目三
void MainWindow::on_pushButton_12_clicked()
{
    int newCow=ui->tableWidget_2->rowCount();
    ui->tableWidget_2->insertRow(newCow);
}

void MainWindow::on_pushButton_14_clicked()
{
    int newCow=ui->tableWidget_3->rowCount();
    ui->tableWidget_3->insertRow(newCow);
}

void MainWindow::on_pushButton_13_clicked()
{
    //如果行数为0则不执行代码
    if(ui->tableWidget_2->rowCount()==0){
        return;
    }
    ui->tableWidget_2->removeRow(ui->tableWidget_2->rowCount()-1);
}

void MainWindow::on_pushButton_15_clicked()
{
    //如果行数为0则不执行代码
    if(ui->tableWidget_3->rowCount()==0){
        return;
    }
    ui->tableWidget_3->removeRow(ui->tableWidget_3->rowCount()-1);
}
//初始化联合信源
void MainWindow::on_pushButton_8_clicked()
{
    //首先初始化两个普通信源
    QVector<QString> _Symbol_list_1;
    QVector<double> _Prob_list_1;
    QVector<QString> _Symbol_list_2;
    QVector<double> _Prob_list_2;

    int result_of_s1= init_single_source(_Symbol_list_1,_Prob_list_1,ui->tableWidget_2);

    if(result_of_s1==0){
        ui->plainTextEdit_2->appendPlainText(init_plaintext_str("信源1：不能存在空格！"));return;
    }
    else if(result_of_s1==-1){
        ui->plainTextEdit_2->appendPlainText(init_plaintext_str("信源1：符号存在重复元素"));return;
    }
    else if(result_of_s1==-2){
        ui->plainTextEdit_2->appendPlainText(init_plaintext_str("信源1：概率和不为1"));return;
    }

    int result_of_s2=init_single_source(_Symbol_list_2,_Prob_list_2,ui->tableWidget_3);
    if(result_of_s2==0){
        ui->plainTextEdit_2->appendPlainText(init_plaintext_str("信源2：不能存在空格！"));return;
    }
    else if(result_of_s2==-1){
        ui->plainTextEdit_2->appendPlainText(init_plaintext_str("信源2：符号存在重复元素"));return;
    }
    else if(result_of_s2==-2){
        ui->plainTextEdit_2->appendPlainText(init_plaintext_str("信源2：概率和不为1"));return;
    }

    //检测是否为空
    if(_Symbol_list_1.isEmpty()||_Prob_list_1.isEmpty()||_Symbol_list_2.isEmpty()||_Prob_list_2.isEmpty()){
        ui->plainTextEdit_2->appendPlainText(init_plaintext_str("存在空表！不能初始化！"));
        return;
    }

    IFO_SOURCE<double,QString> TEMP_S1(_Symbol_list_1,_Prob_list_1);
    IFO_SOURCE<double,QString> TEMP_S2(_Symbol_list_2,_Prob_list_2);

    this->My_Uni_Source=new uni_ifo_source(TEMP_S1,TEMP_S2);

    ui->plainTextEdit_2->appendPlainText(init_plaintext_str("初始化联合信源成功！"));

    print_uni_source_matrix();
    //利用两个普通信源开始初始化联合信源
}

void MainWindow::print_uni_source_matrix()
{
    QVector<QVector<double>> temp=this->My_Uni_Source->GET_UNI_SOURCE_MATRIX();
    QString temp_str="====================\r\n";
    temp_str+="--------------------\r\n";
    for(int i=0;i<My_Uni_Source->GET_UNI_SOURCE_FIRST_SIZE();i++){
        for(int j=0;j<My_Uni_Source->GET_UNI_SOURCE_SECOND_SIZE();j++){
            temp_str+=QString::number(temp[i][j])+"\t";
        }
        temp_str+="--------------------\r\n";
    }
    temp_str+="====================\r\n";
    ui->plainTextEdit_2->appendPlainText(temp_str);
}

void MainWindow::on_pushButton_9_clicked()
{
    QPair<QString,QString> temp= this->My_Uni_Source->SAMPLE_UNI_SOURCE();
    ui->lineEdit_4->setText(temp.first);
    ui->lineEdit_5->setText(temp.second);
}

void MainWindow::on_pushButton_10_clicked()
{
    if(this->My_Uni_Source==nullptr){
        ui->plainTextEdit_2->appendPlainText(init_plaintext_str("你没初始化联合信源"));
        return;
    }
    int base=2;
    if(ui->radioButton_3->isChecked()){
        base=2;
    }
    else if(ui->radioButton_4->isChecked()){
        base=10;
    }
    double entrop_uni_result= this->My_Uni_Source->entropy_uni(base);
    ui->lineEdit_6->setText(QString::number(entrop_uni_result));
}

void MainWindow::on_pushButton_11_clicked()
{
    if(this->My_Uni_Source==nullptr){
        ui->plainTextEdit_2->appendPlainText(init_plaintext_str("你没初始化联合信源"));
        return;
    }
    int base=2;
    if(ui->radioButton_3->isChecked()){
        base=2;
    }
    else if(ui->radioButton_4->isChecked()){
        base=10;
    }
    double entrop_condition_result=this->My_Uni_Source->entropy_condition(base);
    ui->lineEdit_7->setText(QString::number(entrop_condition_result));
}
void MainWindow::tips()
{
    QMessageBox tips;
    tips.setText("还没写 _(:з」∠)_");
    tips.setStandardButtons(QMessageBox::Ok);
    tips.setDefaultButton(QMessageBox::Ok);
    tips.show();
    tips.exec();
}

void MainWindow::on_action_output_ifo_triggered()
{
    tips();
}

void MainWindow::on_action_input_ifo_triggered()
{
    tips();
}

void MainWindow::on_action_output_uni_triggered()
{
    tips();
}

void MainWindow::on_action_input_uni_triggered()
{
    tips();
}

void MainWindow::on_pushButton_16_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButton_17_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_2_clicked()
{
     ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_pushButton_3_clicked()
{
     ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_19_clicked()
{
    if(this->My_Source==nullptr){
        ui->plainTextEdit_4->appendPlainText("未初始化信源！");
        return;
    }
    if(this->My_Code==nullptr){
        this->My_Code=new ifo_code(*My_Source);
    }
    else{
        delete My_Code;
        My_Code =nullptr;
        this->My_Code=new ifo_code(*My_Source);
    }
    for(std::pair<QString, std::string> Pair:this->My_Code->huffmanCodeQString){
        QString temp=Pair.first+" : "+QString::fromStdString(Pair.second);
        ui->plainTextEdit_4->appendPlainText(temp);
    }
    ui->plainTextEdit_4->appendPlainText("====================");
}

void MainWindow::Plain_Text_My_Code()
{
    if(this->My_Code==nullptr){
        ui->plainTextEdit_4->appendPlainText("未初始化编码类");
        return;
    }
    for(std::pair<char, std::string> Pair:this->My_Code->huffmanCode){
        QString temp=QString(1,Pair.first) +" : "+QString::fromStdString(Pair.second);
        ui->plainTextEdit_4->appendPlainText(temp);
    }
    ui->plainTextEdit_4->appendPlainText("====================");
}

void MainWindow::on_pushButton_18_clicked()
{
    QString text=ui->plainTextEdit_3->toPlainText();
    if(this->My_Code==nullptr){
        this->My_Code=new ifo_code(text.toStdString());
    }
    else{
        delete My_Code;
        My_Code =nullptr;
        this->My_Code=new ifo_code(text.toStdString());
    }
    Plain_Text_My_Code();
}

void MainWindow::on_pushButton_20_clicked()
{
    if(this->My_Source==nullptr){
        ui->plainTextEdit_4->appendPlainText("未初始化信源！");
        return;
    }
    if(My_Code==nullptr){
        ui->plainTextEdit_4->appendPlainText("请初始化编码类");
        return;
    }
    QString temp=QString::number(My_Code->CodeEffi(*this->My_Source));
    ui->plainTextEdit_4->appendPlainText(temp);
}

std::pair<bool, bool> MainWindow::checkEncoding(const std::map<char, string> &encoding)
{
    bool isFixedLength = true;
    bool isPrefixFreeCode = true;

    // 1. 判断定长编码还是变长编码
    size_t length = encoding.begin()->second.size();
    for (const auto& entry : encoding) {
        if (entry.second.size() != length) {
            isFixedLength = false; // 有不同长度的编码，说明是变长编码
            break;
        }
    }

        for (auto it1 = encoding.begin(); it1 != encoding.end(); ++it1) {
        for (auto it2 = encoding.begin(); it2 != encoding.end(); ++it2) {
            if (it1 != it2 && it2->second==it1->second) {
                isPrefixFreeCode= false; // it1->second 是 it2->second 的前缀
                break;
            }
        }
    }
        return {isFixedLength, isPrefixFreeCode};
}

std::pair<bool, bool> MainWindow::checkEncoding_Now(const std::map<char, string> &encoding)
{
    bool isNow=true;
    bool isOnly=true;
    for (auto it1 = encoding.begin(); it1 != encoding.end(); ++it1) {
        for (auto it2 = encoding.begin(); it2 != encoding.end(); ++it2) {
            if (it1 != it2 && it2->second.find(it1->second) == 0) {
                isNow= false; // it1->second 是 it2->second 的前缀
                break;
            }
        }
    }
    std::vector<string> codes;
    for(std::pair<char, string> Pair: encoding){
        codes.push_back(Pair.second);
    }
    isOnly= sardinasPatterson(codes);

    return {isOnly,isNow};
}

bool MainWindow::sardinasPatterson(const vector<string> &code)
{
    // 将码字集合转化为set，去除重复
    set<string> codeSet(code.begin(), code.end());

    // 初始化步骤
    vector<set<string>> S_sets;
    int index = 1;

    // 构建初始集合S1
    set<string> S;
    for (const auto& x : codeSet) {
        for (const auto& y : codeSet) {
            if (x != y) {
                // 如果一个码字是另一个码字的前缀，添加后缀到S
                if (y.find(x) == 0) {
                    string suffix = y.substr(x.size());
                    if (!suffix.empty()) {
                        S.insert(suffix);
                    } else {
                        // 如果后缀为空，加入空串
                        S.insert("");
                    }
                }
                if (x.find(y) == 0) {
                    string suffix = x.substr(y.size());
                    if (!suffix.empty()) {
                        S.insert(suffix);
                    } else {
                        S.insert("");
                    }
                }
            }
        }
    }

    S_sets.push_back(S);

    // 执行迭代步骤
    while (true) {
        // 检查是否包含空串
        if (containsEmptyString(S_sets.back())) {
            return false;
        }

        // 构建下一个集合
        set<string> next_S;
        for (const auto& s : S_sets.back()) {
            for (const auto& c : codeSet) {
                if (s.find(c) == 0) {
                    string suffix = s.substr(c.size());
                    if (!suffix.empty()) {
                        next_S.insert(suffix);
                    } else {
                        next_S.insert("");
                    }
                }
                if (c.find(s) == 0) {
                    string suffix = c.substr(s.size());
                    if (!suffix.empty()) {
                        next_S.insert(suffix);
                    } else {
                        next_S.insert("");
                    }
                }
            }
        }
        index++;

        // 检查是否包含空串
        if (containsEmptyString(next_S)) {
            return false;
        }

        // 检查是否重复或为空
        if (next_S.empty()) {
            return true;
        }

        // 检查S集合是否与之前的集合重复
        if (std::find(S_sets.begin(), S_sets.end(), next_S) != S_sets.end()) {
            return true;
        }

        S_sets.push_back(next_S);
    }
}


void MainWindow::on_pushButton_21_clicked()
{
    this->encoding.clear();
    std::pair<char,std::string> temp_Pair;
    for(int i=0;i<ui->tableWidget_4->rowCount();++i){
        for(int j=0;j<ui->tableWidget_4->columnCount();++j){
                QTableWidgetItem* item= ui->tableWidget_4->item(i,j);
                if(item){
                    if(j){
                        temp_Pair.second=item->text().toStdString();
                        this->encoding.insert(temp_Pair);
                    }
                    else{
                        QChar c=item->text().at(0);
                        temp_Pair.first=c.toLatin1();
                    }
                }
                else{
                    //存在空格
                    ui->plainTextEdit_5->appendPlainText("存在空格");
                    this->encoding.clear();
                    return;
                }
        }
    }

    std::pair<bool,bool> Result =checkEncoding(this->encoding);
    std::pair<bool,bool> Result2=checkEncoding_Now(this->encoding);
    ui->checkBox->setChecked(Result.first);
    ui->checkBox_2->setChecked(Result.second);
    ui->checkBox_3->setChecked(Result2.first);
    ui->checkBox_4->setChecked(Result2.second);
}

void MainWindow::on_pushButton_22_clicked()
{
    int newCow=ui->tableWidget_4->rowCount();
    ui->tableWidget_4->insertRow(newCow);
}

void MainWindow::on_pushButton_23_clicked()
{
    ui->tableWidget_4->removeRow(ui->tableWidget_4->rowCount()-1);
}
