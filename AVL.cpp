#include<iostream>

using namespace std;

struct AVL {
    AVL* left;
    AVL* right;
    AVL* par;
    int val;
    int height; 
};

AVL* root = nullptr;  

int height(AVL* node) {
    if (node == nullptr) return -1;
    return node->height;
}

void updateHeight(AVL* node) {
    if (node != nullptr) {
        node->height = max(height(node->left), height(node->right)) + 1;
    }
}

int balanceFactor(AVL* node) {
    if (node == nullptr) return 0;
    return height(node->left) - height(node->right);
}

AVL* rotateLeft(AVL* root) {
    AVL* newRoot = root->right;
    root->right = newRoot->left;
    if (newRoot->left != nullptr) newRoot->left->par = root;
    newRoot->left = root;
    newRoot->par = root->par;
    root->par = newRoot;
    updateHeight(root);
    updateHeight(newRoot);
    return newRoot;
}

AVL* rotateRight(AVL* root) {
    AVL* newRoot = root->left;
    root->left = newRoot->right;
    if (newRoot->right != nullptr) newRoot->right->par = root;
    newRoot->right = root;
    newRoot->par = root->par;
    root->par = newRoot;
    updateHeight(root);
    updateHeight(newRoot);
    return newRoot;
}

AVL* insert(AVL*& root, int x, AVL* par = nullptr) {
    if (root == nullptr) {
        AVL* newNode = new AVL;
        newNode->val = x;
        newNode->left = newNode->right = nullptr;
        newNode->height = 0;
        newNode->par = par;
        return newNode;
    }

    if (x < root->val) {
        root->left = insert(root->left, x, root);
    }
    else {
        root->right = insert(root->right, x, root);
    }

    updateHeight(root);

    int balance = balanceFactor(root);

    if (balance > 1 && x < root->left->val) {
        return rotateRight(root);
    }

    if (balance < -1 && x > root->right->val) {
        return rotateLeft(root);
    }

    if (balance > 1 && x > root->left->val) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && x < root->right->val) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}






















int main()
{
    return 0;
}

