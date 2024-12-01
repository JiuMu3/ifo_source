#ifndef IFO_CODE_H
#define IFO_CODE_H
#include <unordered_map>
#include <queue>
#include <vector>
#include <string>
#include <QDebug>
#include <QPair>
#include <QString>
#include "ifo_source.h"
// 定义赫夫曼树节点结构体
struct Node {
    char ch;  // 存储字符
    int freq; // 频率
    QString CH;
    double FREQ;
    Node *left, *right;

    // 用于叶节点的构造函数（包含字符和频率）
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    Node(QString c, double f) : CH(c), FREQ(f), left(nullptr), right(nullptr) {}
    // 用于非叶节点的构造函数（合并节点）
    Node(int f, Node* l, Node* r) : ch('\0'), freq(f), left(l), right(r) {}
};

// 自定义比较函数，优先队列需要最小频率的节点优先出队
struct Compare {
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq;
    }
};

class ifo_code
{
public:
    std::unordered_map<char, std::string> huffmanCode;
    std::unordered_map<QString, std::string> huffmanCodeQString;
public:
    void generateCodes(Node* root, const std::string& code, std::unordered_map<char, std::string>& huffmanCode);
    void generateCodesQString(Node* root, const std::string& code, std::unordered_map<QString, std::string>& huffmanCode);
    void freeTree(Node* root);
    void buildHuffmanTree(const std::string &text,std::unordered_map<char, std::string> &huffmanCode);
    void buildHuffmanTree_fromsource(IFO_SOURCE<double, QString> temp_source,std::unordered_map<QString, std::string> &huffmanCodeQString);
    void PrintHuffman();
    void PrintHuffmanQString();
    double CodeEffi(IFO_SOURCE<double, QString> temp_source);
    double AverageSourceCodeLen();
public:
    ifo_code();
    //对输入字符初始化
    ifo_code(std::string text);
    //对于信源进行初始化
    ifo_code(IFO_SOURCE<double,QString> temp_source);
};

#endif // IFO_CODE_H
