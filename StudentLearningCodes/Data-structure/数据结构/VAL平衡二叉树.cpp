#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>  
#include <stdlib.h>  

//typedef int ElementType;
//
//// AVL树节点的定义  
//typedef struct AVLNode {
//    ElementType data;
//    struct AVLNode* left;
//    struct AVLNode* right;
//    int height; // 节点的高度  
//} AVLNode, * AVLTree;
//
//// 获取节点高度  
//int getHeight(AVLNode node) {
//    if (node == NULL) {
//        return 0;
//    }
//    return node->height;
//}
//
//// 获取平衡因子  
//int getBalanceFactor(AVLNode node) {
//    if (node == NULL) {
//        return 0;
//    }
//    return getHeight(node->left) - getHeight(node->right);
//}
//
//// 右旋操作  
//AVLNode rotateRight(AVLNode y) {
//    AVLNode x = y->left;
//    AVLNode T2 = x->right;
//
//    x->right = y;
//    y->left = T2;
//
//    y->height = 1 + max(getHeight(y->left), getHeight(y->right));
//    x->height = 1 + max(getHeight(x->left), getHeight(x->right));
//
//    return x;
//}
//
//// 左旋操作  
//AVLNode rotateLeft(AVLNode x) {
//    AVLNode y = x->right;
//    AVLNode T2 = y->left;
//
//    y->left = x;
//    x->right = T2;
//
//    x->height = 1 + max(getHeight(x->left), getHeight(x->right));
//    y->height = 1 + max(getHeight(y->left), getHeight(y->right));
//
//    return y;
//}
//
//// 插入节点并更新高度和平衡因子，可能触发旋转操作  
//AVLNode insert(AVLNode node, ElementType data) {
//    // 如果树为空，返回一个新节点  
//    if (node == NULL) {
//        return (AVLNode)malloc(sizeof(AVLNode));
//    }
//
//    // 否则，递归下降树  
//    if (data < node->data) {
//        node->left = insert(node->left, data);
//    }
//    else if (data > node->data) {
//        node->right = insert(node->right, data);
//    }
//    else {
//        // 如果数据已存在，则不插入  
//        return node;
//    }
//
//    // 更新高度  
//    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
//
//    // 获取平衡因子  
//    int balance = getBalanceFactor(node);
//
//    // 如果节点不平衡，则进行旋转  
//    if (balance > 1 && data < node->left->data) {
//        return rotateRight(node);
//    }
//    if (balance < -1 && data > node->right->data) {
//        return rotateLeft(node);
//    }
//    if (balance > 1 && data > node->left->data) {
//        node->left = rotateLeft(node->left);
//        return rotateRight(node);
//    }
//    if (balance < -1 && data < node->right->data) {
//        node->right = rotateRight(node->right);
//        return rotateLeft(node);
//    }
//
//    return node;
//}
//
//// 辅助函数，用于获取两个数中的较大值  
//int max(int a, int b) {
//    return (a > b) ? a : b;
//}
//
//// 主函数，用于测试AVL树的插入操作  
//int main() {
//    AVLTree root = NULL;
//
//    // 插入一些

/*这个还没有自己写过*/
/*VAL平衡二叉树*/
