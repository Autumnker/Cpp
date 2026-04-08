#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

// AVL树节点
struct avlTreeNode {
    int _data;
    int _height;
    avlTreeNode* _left;
    avlTreeNode* _right;

    avlTreeNode(const int& data) :_data(data), _height(1), _left(nullptr), _right(nullptr) {}
};


// AVL树类
class AVLTree {
private:
    avlTreeNode* _root;

    // 返回节点高度
    int getHeight(const avlTreeNode* node) {
        return node ? node->_height : 0;
    }

    // 计算平衡因子
    int getBalanceFactor(const avlTreeNode* node) {
        return node ? getHeight(node->_left) - getHeight(node->_right) : 0;
    }

    // 更新节点高度
    void updateHeight(avlTreeNode* node) {
        node->_height = max(getHeight(node->_left), getHeight(node->_right)) + 1;
    }

    /**左旋 */
//   root        node
//   / \        / \
//  L1 node   ->  root R1
//    / \      / \
//   L2  R1    L1  L2

    avlTreeNode* leftRoat(avlTreeNode* root) {
        avlTreeNode* node = root->_right;
        root->_right = node->_left;
        node->_left = root;
        updateHeight(root); // 因为当前root为node的子节点
        updateHeight(node);
        return node;
    }


    /**右旋 */
//   root        node
//   / \        / \
//  node R1   ->  L1  root
//  / \           / \
// L1  R2         R2  R1 

    avlTreeNode* rightRoat(avlTreeNode* root) {
        avlTreeNode* node = root->_left;
        root->_left = node->_right;
        node->_right = root;
        updateHeight(root); // 因为当前root为node的子节点
        updateHeight(node);
        return node;
    }

    // 平衡操作
    avlTreeNode* balance(avlTreeNode* root) {
        updateHeight(root); // 更新根节点高度

        // LL 右旋
        if (getBalanceFactor(root) > 1 && getBalanceFactor(root->_left) > 0) {
            root = rightRoat(root);
        }

        // LR 先左旋再右旋
        else if (getBalanceFactor(root) > 1 && getBalanceFactor(root->_left) < 0) {
            root->_left = leftRoat(root->_left);
            root = rightRoat(root);
        }

        // RR 左旋
        else if (getBalanceFactor(root) < -1 && getBalanceFactor(root->_right) < 0) {
            root = leftRoat(root);
        }

        // RL 先右旋再左旋转
        else if (getBalanceFactor(root) < -1 && getBalanceFactor(root->_right) > 0) {
            root->_right = rightRoat(root->_right);
            root = leftRoat(root);
        }

        return root;
    }

    // 插入单个节点
    avlTreeNode* insertNode(avlTreeNode* parent, int data) {
        if (!parent) {
            return new avlTreeNode(data);
        }

        if (data < parent->_data) {
            parent->_left = insertNode(parent->_left, data);
        }
        else if (data > parent->_data) {
            parent->_right = insertNode(parent->_right, data);
        }
        else {
            return parent;
        }

        return balance(parent);
    }

    // BST先序遍历
    void _preorderTraversal(avlTreeNode* node) {
        if (!node) { return; }
        if (node != _root) { cout << "->"; }
        cout << node->_data;
        _preorderTraversal(node->_left);
        _preorderTraversal(node->_right);
    }

public:
    AVLTree() :_root(nullptr) {}

    // 插入一个节点
    void insertSingleNode(int data) {
        _root = insertNode(_root, data);
    }

    // 批量插入节点
    void insertBatchNodes(const vector<int>& data) {
        for (const int& n : data) {
            _root = insertNode(_root, n);
        }
    }

    // 删除节点

    // 查询节点

    // 修改节点(先删后插)

    // 销毁整棵树

    // 遍历树(先序)
    void preorderTraversal() {
        _preorderTraversal(_root);
    }

    // 层序遍历


};


#include <time.h>
#include <random>
int main(int argc, char const* argv[])
{
    int n;
    cin >> n;

    AVLTree avlTree;
    vector<int> arr;

    srand(time(0));
    for (int i = 0; i < n; ++i) {
        arr.push_back(rand() % n + 1);
    }
    for (int i = 0; i < n; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

    avlTree.insertBatchNodes(arr);
    avlTree.preorderTraversal();

    return 0;
}
