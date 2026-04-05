#include <iostream>
#include <vector>
#include <set>
#include <time.h>
#include <random>

using namespace std;

struct Node {
    int _val;
    Node* _left;
    Node* _right;

    Node(int val) :_val(val), _left(nullptr), _right(nullptr) {}
};

class BST {
private:
    // 根节点
    Node* root;

    // 递归插入
    void insertRecursion(Node* node, int val) {
        if (val < node->_val) {
            if (node->_left == nullptr) {
                Node* newNode = new Node(val);
                node->_left = newNode;
                return;
            }
            node = node->_left;
        }
        else if (val > node->_val) {
            if (node->_right == nullptr) {
                Node* newNode = new Node(val);
                node->_right = newNode;
                return;
            }
            node = node->_right;
        }
        else {
            return; // 不插入重复元素
        }

        insertRecursion(node, val);
    }

    // 迭代插入
    void insertIteration(int val) {
        Node* curr = root;
        Node* newNode = new Node(val);

        while (curr != newNode) {
            if (val < curr->_val) {
                if (curr->_left == nullptr) {
                    curr->_left = newNode;
                }
                curr = curr->_left;
            }
            else if (val > curr->_val) {
                if (curr->_right == nullptr) {
                    curr->_right = newNode;
                }
                curr = curr->_right;
            }
            else {
                delete newNode;
                return; // 不插入重复元素
            }
        }
    }

    // 递归删除树
    void destoryTree(Node* node) {
        if (node == nullptr) { return; }
        destoryTree(node->_left);
        destoryTree(node->_right);
        delete node;
    }

public:
    // 构造函数
    BST(int val) {
        root = new Node(val);
    }

    // 析构函数
    ~BST() {
        // destoryTree(root);
    }

    enum class InsertType { Recursion, Iteration };
    // 单值插入
    void insertItem(int val, InsertType flag) {
        switch (flag) {
        case InsertType::Recursion:
            insertRecursion(root, val);
            break;
        case InsertType::Iteration:
            insertIteration(val);
            break;
        default:
            cout << __LINE__ << "error" << endl;
        }

    }

    // 批量插入
    void insertBatch(const vector<int>& v, InsertType flag) {
        for (int n : v) {
            insertItem(n, flag);
        }
    }

    // 先序遍历
    void PreorderTraversal(Node* node) {
        if (node == nullptr) { return; }
        cout << node->_val << " ";
        PreorderTraversal(node->_left);
        PreorderTraversal(node->_right);
    }

    // 中序遍历
    void In_orderTraversal(Node* node) {
        if (node == nullptr) { return; }
        In_orderTraversal(node->_left);
        cout << node->_val << " ";
        In_orderTraversal(node->_right);
    }

    // 后序遍历
    void Post_orderTraversal(Node* node) {
        if (node == nullptr) { return; }
        Post_orderTraversal(node->_left);
        Post_orderTraversal(node->_right);
        cout << node->_val << " ";
    }

    enum class Traversal { PRE, IN_ORD, POST };

    // 打印树
    void printBST(Traversal flag) {
        Node* curr = root;
        switch (flag) {
        case Traversal::PRE:
            cout << "PreorderTraversal:" << endl;
            PreorderTraversal(curr);
            cout << endl;
            break;
        case Traversal::IN_ORD:
            cout << "In_orderTraversal:" << endl;
            In_orderTraversal(curr);
            cout << endl;
            break;
        case Traversal::POST:
            cout << "Post_orderTraversal:" << endl;
            Post_orderTraversal(curr);
            cout << endl;
            break;
        default:
            cout << __LINE__ << "error" << endl;
        }
    }
};

int main(int argc, char const* argv[])
{
    int n;
    vector<int> arr;
    BST bst(1);

    cin >> n;
    srand(time(0));
    for (int i = 0; i < n; ++i) {
        arr.push_back(rand() % n + 1);
    }

    for (int n : arr) {
        cout << n << " ";
    }
    cout << endl;
    cout << "==================================" << endl;

    // bst.insertBatch(arr, BST::InsertType::Iteration);
    bst.insertBatch(arr, BST::InsertType::Recursion);
    bst.printBST(BST::Traversal::PRE);
    bst.printBST(BST::Traversal::IN_ORD);
    bst.printBST(BST::Traversal::POST);

    return 0;
}
