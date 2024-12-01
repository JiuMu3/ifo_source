#include "ifo_code.h"
void ifo_code::generateCodes(Node *root, const std::string &code, std::unordered_map<char, std::string> &huffmanCode){
        if (!root) {
            return;
        }

        // 叶节点（包含字符）
        if (!root->left && !root->right) {
            huffmanCode[root->ch] = code;
        }

        generateCodes(root->left, code + "0", huffmanCode);
        generateCodes(root->right, code + "1", huffmanCode);
}

void ifo_code::generateCodesQString(Node *root, const std::string &code, std::unordered_map<QString, std::string> &huffmanCode)
{
        if (!root) {
            return;
        }

        // 叶节点（包含字符）
        if (!root->left && !root->right) {
            huffmanCode[root->CH] = code;
        }

        generateCodesQString(root->left, code + "0", huffmanCode);
        generateCodesQString(root->right, code + "1", huffmanCode);
}

void ifo_code::freeTree(Node *root)
{
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

void ifo_code::buildHuffmanTree(const std::string &text,std::unordered_map<char, std::string> &huffmanCode)
{
    // 统计字符频率
    std::unordered_map<char, int> freq;
    //char ch : text表示遍历text里面的所有元素，并且将text的字符传递给ch
    for (char ch : text) {
        //freq[ch]++将ch对应的频率加一
        freq[ch]++;
    }

    // 创建优先队列
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;

    // 将每个字符创建为单独的节点加入优先队列
    for (auto pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    // 构建赫夫曼树
    while (pq.size() != 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        // 合并两个最小频率的节点
        int sum = left->freq + right->freq;
        pq.push(new Node(sum, left, right)); // 新节点，不含字符
    }

    // 获取根节点
    Node* root = pq.top();

    // 生成赫夫曼编码 string用于存储赫夫曼编码 char是对应的字符

    generateCodes(root, "", huffmanCode);

    // 释放树内存
    freeTree(root);
}

void ifo_code::buildHuffmanTree_fromsource(IFO_SOURCE<double, QString> temp_source,std::unordered_map<QString, std::string> &huffmanCodeQString)
{
    QVector<double> temp_prob=temp_source.GET_MY_PROB_LIST();
    QVector<QString> temp_symbol=temp_source.GET_MY_SYMBOL_LIST();
    std::unordered_map<QString, double> freq;
    for(int i=0;i<temp_prob.size();i++){
        freq[temp_symbol[i]]=temp_prob[i];
    }
    // 创建优先队列
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    // 将每个字符创建为单独的节点加入优先队列
    for (auto pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }
    // 构建赫夫曼树
    while (pq.size() != 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        // 合并两个最小频率的节点
        int sum = left->freq + right->freq;
        pq.push(new Node(sum, left, right)); // 新节点，不含字符
    }

    // 获取根节点
    Node* root = pq.top();

    // 生成赫夫曼编码 string用于存储赫夫曼编码 char是对应的字符

    generateCodesQString(root, "", huffmanCodeQString);

    // 释放树内存
    freeTree(root);
}

void ifo_code::PrintHuffman()
{
    if(huffmanCode.empty()){
        qDebug()<<"未初始化哈夫曼";
        return;
    }

    for(std::pair<char, std::string> Pair:huffmanCode){
        qDebug()<<Pair.first<<":"<<QString::fromStdString(Pair.second);
    }
}

void ifo_code::PrintHuffmanQString()
{
    if(huffmanCodeQString.empty()){
        qDebug()<<"未初始化哈夫曼";
        return;
    }

    for(std::pair<QString, std::string> Pair:huffmanCodeQString){
        qDebug()<<Pair.first<<":"<<QString::fromStdString(Pair.second);
    }
}

double ifo_code::AverageSourceCodeLen()
{
    double num=0;
    double Codenum=0;
    for(std::pair<QString, std::string> Pair:huffmanCodeQString){
        num++;
        Codenum+=(double)Pair.second.size();
    }
    return Codenum/num;
}

double ifo_code::CodeEffi(IFO_SOURCE<double, QString> temp_source)
{
    double temp_1=temp_source.entropy(2);
    double temp_2=AverageSourceCodeLen();
    return temp_1/temp_2;
}

ifo_code::ifo_code()
{

}

ifo_code::ifo_code(std::string text)
{
    buildHuffmanTree(text,this->huffmanCode);
    PrintHuffman();
}

ifo_code::ifo_code(IFO_SOURCE<double, QString> temp_source)
{
    buildHuffmanTree_fromsource(temp_source,this->huffmanCodeQString);
    PrintHuffmanQString();
}
