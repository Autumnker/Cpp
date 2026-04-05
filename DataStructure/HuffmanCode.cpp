/**
 * 一、统计字符频率
 * 二、构建小堆
 * 三、将小堆转换为哈夫曼树
 * 四、编码
 * 五、解码
 * 六、计算压缩率
 */

#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

// 创建哈夫曼节点
struct HuffmanNode {
    char _data;
    int _freq;
    HuffmanNode* _left;
    HuffmanNode* _right;

    HuffmanNode(char data, int freq) :_data(data), _freq(freq), _left(nullptr), _right(nullptr) {}
    HuffmanNode(int freq) :_data('\0'), _freq(freq), _left(nullptr), _right(nullptr) {}

    bool isLeaf() {
        return _left == nullptr && _right == nullptr;
    }
};

// 哈夫曼类
class HuffmanCode {
private:
    HuffmanNode* _root;
    map<char, int> _charFreqMap;
    map<char, string> _mappingTable;

    // 统计字符频率
    void buildCharFreqMap(const string& str) {
        for (const char& c : str) {
            _charFreqMap[c]++;
        }
    }

    // 比较函数
    struct Compare {
        bool operator()(HuffmanNode* a, HuffmanNode* b) {
            return a->_freq > b->_freq; // 频率小的优先级高
        }
    };

    // 构建哈夫曼树
    void buildHuffmanTree(const string& str) {
        buildCharFreqMap(str);

        // 创建小堆
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> minHeap;
        for (auto it = _charFreqMap.begin(); it != _charFreqMap.end(); ++it) {
            HuffmanNode* node = new HuffmanNode((*it).first, (*it).second);
            minHeap.push(node);
        }

        // 构建哈夫曼树
        while (minHeap.size() > 1) {    // 还剩一个则是根节点
            // 先弹出最小的两个节点
            HuffmanNode* a = minHeap.top(); minHeap.pop();
            HuffmanNode* b = minHeap.top(); minHeap.pop();

            // 构建它们的父节点并指向这两个节点
            HuffmanNode* parent = new HuffmanNode(a->_freq + b->_freq);
            parent->_left = a;
            parent->_right = b;

            // 将父节点插入小堆
            minHeap.push(parent);
        }

        _root = minHeap.top(); minHeap.pop();
    }

    // 先序遍历
    void preorderTraversal(HuffmanNode* node, string code) {
        if (node->isLeaf()) {
            _mappingTable[node->_data] = code;
            return;
        }
        preorderTraversal(node->_left, code + "0");
        preorderTraversal(node->_right, code + "1");
    }

    // 构建映射表
    void buildMappingTable(const string& str) {
        buildHuffmanTree(str);
        preorderTraversal(_root, "");
    }

    // 销毁哈夫曼树
    void destoryHuffmanTree(HuffmanNode* node) {
        if (node == nullptr) { return; }
        destoryHuffmanTree(node->_left);
        destoryHuffmanTree(node->_right);
        delete node;
    }

public:
    // 打印字符频率
    void printCharFreqMap() {
        cout << "--------------------------------------------------" << endl;
        for (auto it = _charFreqMap.begin(); it != _charFreqMap.end(); ++it) {
            cout << (*it).first << "(" << (*it).second << "); ";
        }
        cout << "\n--------------------------------------------------" << endl;
    }

    // 打印编码表
    void printMappingTable() {
        cout << "--------------------------------------------------" << endl;
        for (auto it = _mappingTable.begin(); it != _mappingTable.end(); ++it) {
            cout << (*it).first << " : " << (*it).second << endl;
        }
        cout << "--------------------------------------------------" << endl;
    }

    // 编码
    string encode(const string& str) {
        buildMappingTable(str);
        string encode;

        for (const char& c : str) {
            encode += _mappingTable[c];
        }

        return encode;
    }

    // 解码
    string decode(const string& huffmanCode) {
        HuffmanNode* curr = _root;
        string decode;
        for (const char& bit : huffmanCode) {
            if (curr->isLeaf()) {
                decode += curr->_data;
                curr = _root;
            }

            if (bit == '0') {
                curr = curr->_left;
            }
            else if (bit == '1') {
                curr = curr->_right;
            }
        }

        decode += curr->_data;  // 最后一个叶子节点的元素无法在循环内被读取

        return decode;
    }

    // 计算压缩率
    double compressibility(const string& str) {
        string encode = this->encode(str);
        int originLength = str.length() * 8;     // 字节长度
        int huffmanCodeLength = encode.length();    // 比特位

        return (double)huffmanCodeLength / originLength;
    }
};

int main(int argc, char const* argv[])
{
    string str = "hello world";
    HuffmanCode huffman;
    getline(cin, str);

    string encode = huffman.encode(str);
    string decode = huffman.decode(encode);

    cout << "encode = " << encode << endl;
    huffman.printCharFreqMap();
    huffman.printMappingTable();
    cout << "decode = " << decode << endl;
    cout << "compressibility = " << huffman.compressibility(str) * 100 << "%" << endl;;

    return 0;
}
