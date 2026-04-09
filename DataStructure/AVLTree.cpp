#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

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

    avlTreeNode* leftRotation(avlTreeNode* root) {
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

    avlTreeNode* rightRotation(avlTreeNode* root) {
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
            root = rightRotation(root);
        }

        // LR 先左旋再右旋
        else if (getBalanceFactor(root) > 1 && getBalanceFactor(root->_left) < 0) {
            root->_left = leftRotation(root->_left);
            root = rightRotation(root);
        }

        // RR 左旋
        else if (getBalanceFactor(root) < -1 && getBalanceFactor(root->_right) < 0) {
            root = leftRotation(root);
        }

        // RL 先右旋再左旋转
        else if (getBalanceFactor(root) < -1 && getBalanceFactor(root->_right) > 0) {
            root->_right = rightRotation(root->_right);
            root = leftRotation(root);
        }

        return root;
    }

    // 插入单个节点
    avlTreeNode* insertNode(avlTreeNode* node, int data) {
        if (!node) {
            return new avlTreeNode(data);
        }

        if (data < node->_data) {
            node->_left = insertNode(node->_left, data);
        }
        else if (data > node->_data) {
            node->_right = insertNode(node->_right, data);
        }
        else {
            return node;
        }

        return balance(node);
    }

    // 删除节点
    avlTreeNode* _deleteNode(avlTreeNode* node, int data) {
        if (!node) { return nullptr; }
        if (data < node->_data) {
            node->_left = _deleteNode(node->_left, data);
        }
        else if (data > node->_data) {
            node->_right = _deleteNode(node->_right, data);
        }
        else {
            // 找到节点,判断其子节点数量

            // node是叶子节点或只有一个子节点
            if (!node->_left || !node->_right) {
                avlTreeNode* tmp = node->_left ? node->_left : node->_right;
                delete node;
                return tmp;
            }

            // 有两个子节点
            else {
                // 这里可以用左子树的最大节点或者右子树的最小节点替换node的位置,我选择前者
                avlTreeNode* leftMax = node->_left;
                while (leftMax->_right) {
                    leftMax = leftMax->_right;
                }

                node->_data = leftMax->_data;

                node->_left = _deleteNode(node->_left, leftMax->_data);
            }
        }
        return balance(node);
    }

    // 查询节点
    avlTreeNode* _searchNode(avlTreeNode* node, int data) {
        if (!node) {
            return nullptr;
        }

        if (data < node->_data) {
            return _searchNode(node->_left, data);
        }
        else if (data > node->_data) {
            return _searchNode(node->_right, data);
        }
        return node;
    }

    // 销毁整棵树
    void _destory(avlTreeNode* node) {
        if (!node) { return; }
        _destory(node->_left);
        _destory(node->_right);
        delete node;
    }

    // BST先序遍历
    void _preorderTraversal(avlTreeNode* node, avlTreeNode* startNode) {
        if (!node) { return; }
        if (node != startNode) { cout << "->"; }
        cout << node->_data << " ";
        _preorderTraversal(node->_left, startNode);
        _preorderTraversal(node->_right, startNode);
    }
    // BST中序遍历
    void _inOrderTraversal(avlTreeNode* node, avlTreeNode* startNode) {
        if (!node) { return; }
        _inOrderTraversal(node->_left, startNode);
        if (node != startNode) { cout << "->"; }
        cout << node->_data << " ";
        _inOrderTraversal(node->_right, startNode);
    }
    // BST后序遍历
    void _postOrderTraversal(avlTreeNode* node, avlTreeNode* startNode) {
        if (!node) { return; }
        _postOrderTraversal(node->_left, startNode);
        _postOrderTraversal(node->_right, startNode);
        if (node != startNode) { cout << "->"; }
        cout << node->_data << " ";
    }

public:
    AVLTree() :_root(nullptr) {}
    ~AVLTree() {
        destory();
    }

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
    void deleteNode(int data) {
        if (searchNode(data)) {
            _root = _deleteNode(_root, data);
        }
        else {
            cout << "data: " << data << " not found" << endl;
        }
    }

    // 查询节点
    bool searchNode(int data) {
        if (_searchNode(_root, data)) {
            return true;
        }
        return false;
    }

    // 修改节点(先删后插)
    void updateNode(int oldData, int newData) {
        deleteNode(oldData);
        insertSingleNode(newData);
    }

    // 销毁整棵树
    void destory() {
        _destory(_root);
        _root = nullptr;
    }

    // 先序遍历
    void preorderTraversal() {
        avlTreeNode* startNode = _root;
        _preorderTraversal(_root, startNode);
        puts("");
    }
    // 中序遍历
    void inOrderTraversal() {
        avlTreeNode* startNode = _root;
        while (startNode->_left) { startNode = startNode->_left; }
        _inOrderTraversal(_root, startNode);
        puts("");

    }
    // 后序遍历
    void postOrderTraversal() {
        avlTreeNode* startNode = _root;
        while (startNode->_left) { startNode = startNode->_left; }
        _postOrderTraversal(_root, startNode);
        puts("");
    }
    enum class Traversal { PreOrder, InOrder, PostOrder };
    // 遍历
    void Traversal(Traversal order) {
        switch (order) {
        case Traversal::PreOrder:
            preorderTraversal();
            break;
        case Traversal::InOrder:
            inOrderTraversal();
            break;
        case Traversal::PostOrder:
            postOrderTraversal();
            break;
        default:
            cout << __FILE__ << "Parameter error" << endl;
        }
    }

    // 层序遍历
    void levelOrderTraversal() {
        queue<avlTreeNode*> que;
        que.push(_root);

        while (!que.empty()) {
            avlTreeNode* node = que.front(); que.pop();

            if (node != _root) { cout << "->"; }
            cout << node->_data;

            if (node->_left) { que.push(node->_left); }
            if (node->_right) { que.push(node->_right); }
        }
        cout << endl;
    }
};


#include <time.h>
#include <random>
#include <unordered_set>

int main(int argc, char const* argv[])
{
    int n;
    int dataToDelete;
    int dataToSearch;
    cin >> n;

    AVLTree avlTree;
    unordered_set<int> number_set;
    vector<int> arr;

    srand(time(0));
    while (number_set.size() < n) {
        number_set.insert(rand() % (2 * n) + 1);
    }
    for (auto it = number_set.begin(); it != number_set.end(); ++it) {
        arr.push_back(*it);
        cout << *it << " ";
    }
    puts("");

    cout << "Input the data of the node to be deleted : ";
    cin >> dataToDelete;

    // 插入测试
    avlTree.insertBatchNodes(arr);

    // 遍历测试
    cout << "PreOrder : ";
    avlTree.Traversal(AVLTree::Traversal::PreOrder);
    cout << "InOrder : ";
    avlTree.Traversal(AVLTree::Traversal::InOrder);
    cout << "PostOrder : ";
    avlTree.Traversal(AVLTree::Traversal::PostOrder);
    cout << "LevelOrder : ";
    avlTree.levelOrderTraversal();

    // 删除测试
    cout << "Delete " << dataToDelete << ": ";
    avlTree.deleteNode(dataToDelete);
    cout << "PreOrder : ";
    avlTree.preorderTraversal();

    cout << "Inpute the data you want to search : ";
    cin >> dataToSearch;
    cout << avlTree.searchNode(dataToSearch) << endl;

    return 0;
}
