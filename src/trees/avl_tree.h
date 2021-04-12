/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   avl_tree.h
 * Author: ginns
 *
 * Created on March 18, 2021, 5:10 PM
 */

#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <algorithm>    /* for max */
#include <queue>        /* used in BFS */
#include <iostream>
#include "interfaces.h"

#define INVALID_SPLIT_KEY     -1

/* Sequential AVL Tree */
using namespace std;
class AVLTree : public IBinarySearchTree {
private:
    struct Node {
        int key;
        Node * parent;
        Node * left;
        Node * right;
        int height;
        
        Node(int _key, Node * _parent, Node * _left, Node * _right, int _height) :
            key(_key), parent(_parent), left(_left), right(_right), height(_height) {
            
        }
    };
    
    Node * root;
    
    /* AVL Tree maintenance and balancing methods */
    int getHeight(Node * node);
    void updateHeight(Node * node);
    int getBalance(Node * node);
    void rebalance(Node * node);
    void leftRotate(Node * node);
    void rightRotate(Node * node);
    Node * search(const int & key);
    Node * successor(Node * node);
    Node * predecessor(Node * node);
    Node * get_min(Node * node);
    Node * get_max(Node * node);
    void unlink(Node * node);
    
    /* Debugging methods */
    void printInOrderTraversal(Node * node);
    bool doesAVLHold(Node * node);
    int sumKeys(Node * node);
    void printBFSOrder(Node * node);
    
    /* Memory reclamation */
    void freeTraversal(Node * node);
    
public:
    

    
    AVLTree();
    ~AVLTree();
    
    /* Dictionary operations  */
    virtual bool contains(const int & key);
    virtual bool insert(const int & key);
    virtual bool erase(const int & key);
    
    /* Set operations */
    static AVLTree * join(AVLTree * leftTree, AVLTree * rightTree);
    static std::tuple<int, AVLTree *, AVLTree *> split(AVLTree * tree);
    
    /* Useful methods */
    int maxKey();
    int minKey();
    
    /* Debugging methods */
    int sumOfKeys();
    void printInOrderTraversal();
    void printBFSOrder();
    bool checkAVL( );
    
};



int AVLTree::getHeight(Node* node) {
    if (node == NULL)
        return -1;
    else
        return node->height;
}

void AVLTree::updateHeight(Node * node) {
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
}

int AVLTree::getBalance(Node * node) {
    return getHeight(node->left) - getHeight(node->right);
}

void AVLTree::rebalance(Node * node) {
    while (node != NULL) {
        updateHeight(node);
        if ( getHeight(node->left) >= (2 + getHeight(node->right)) ) {
            if ( getHeight(node->left->left) >= getHeight(node->left->right) )
                rightRotate(node);
            else {
                leftRotate(node->left);
                rightRotate(node);
            }
        }
        else if ( getHeight(node->right) >= (2 + getHeight(node->left)) )  {
            if ( getHeight(node->right->right) >= getHeight(node->right->left) )
                leftRotate(node);
            else {
                rightRotate(node->right);
                leftRotate(node);
            }
        }
        node = node->parent; 
    }
}

void AVLTree::leftRotate(Node * x) {
    Node * y = x->right;
    y->parent = x->parent;
    if (y->parent == NULL) 
        root = y; /* x was previously the root */
    else {
        if (y->parent->left == x)
            y->parent->left = y;
        else
            y->parent->right = y;
    }
    x->right = y->left;
    if (x->right != NULL)
        x->right->parent = x;
    y->left = x;
    x->parent = y;
    updateHeight(x);
    updateHeight(y);
    
}

void AVLTree::rightRotate(Node * x) {
    Node * y = x->left;
    y->parent = x->parent;
    if (y->parent == NULL)
        root = y;
    else {
        if (y->parent->left == x)
            y->parent->left = y;
        else
            y->parent->right = y;
    }
    x->left = y->right;
    if (x->left != NULL)
        x->left->parent = x;
    y->right = x;
    x->parent = y;
    updateHeight(x);
    updateHeight(y);
    
    
}

AVLTree::Node * AVLTree::successor(Node * node) {
    Node * succ = NULL;
    if (node->right != NULL) {
        succ = get_min(node->right);
    }
    else {
        /* Node does not have a right child, look up the tree for successor */
        Node * curr = node;
        while ( (curr->parent != NULL) && (curr->parent->right == curr) ) {
            /* Walk up the tree until we reach a node that is the left child of the parent */
            curr = curr->parent;
        }
        succ = curr->parent;
    }
    return succ;
}

AVLTree::Node * AVLTree::predecessor(Node * node) {
    Node * pred = NULL;
    if (node->left != NULL) {
        pred = get_max(node->left);
    }
    else {
        /* Node odes not have a left child, look up the tree for predecessor */
        Node * curr = node;
        while ( (curr->parent != NULL) && (curr->parent->left == curr) ) {
            /* Walk up the tree until we reach a node that is the right child of the parent */
            curr = curr->parent;
        }
        
        pred = curr->parent;    
    }
    return pred;
}

/**
 * returns the node containing the minimum of the tree rooted at @p node
 */
AVLTree::Node * AVLTree::get_min(Node * node) {
    Node * curr = node;
    while ( curr->left != NULL ) {
        curr = curr->left;
    }
    return curr;
}

int AVLTree::minKey() {
    int min = -1;
    Node * minNode = get_min(root);
    if (minNode != NULL) {
        min = minNode->key;
    }
    return min;
}

AVLTree::Node * AVLTree::get_max(Node * node) {
    Node * curr = node;
    while ( curr->right != NULL) {
        curr = curr->right;
    }
    return curr;
}

int AVLTree::maxKey() {
    int max = -1;
    Node * maxNode = get_max(root);
    if (maxNode != NULL) {
        max = maxNode->key;
    }
    return max;
}

AVLTree::AVLTree() {
    root = NULL;
}

void AVLTree::freeTraversal(Node * node) {
    assert(node != NULL);
    if ( node->left != NULL )
        freeTraversal(node->left);
    
    if ( node->right != NULL)
        freeTraversal(node->right);
    
    delete(node);
}

AVLTree::~AVLTree() {
    if (root != NULL)
        freeTraversal(root);
}

AVLTree::Node * AVLTree::search(const int & key) {
    Node * curr = root;
    while (curr != NULL) {
        if (curr->key == key)     
            break; //found key;
        else if (key < curr->key) {
            if ( curr->left == NULL )
                break;
            else
                curr = curr->left;
        }
        else {
            if ( curr->right == NULL )
                break;
            else
                curr = curr->right;
        }
    }
    return curr;   
}

bool AVLTree::contains(const int & key) {
    Node * n = search(key);
    return (n->key == key);
}

/* Semantics: Only insert if key is absent */
bool AVLTree::insert(const int & key) {
    
    /* BST Insert */
    Node * node = search(key);
    
    if ( node != NULL && (node->key == key) )
        return false; //duplicate key
    
    /* Unique key, create new node */
    Node * newNode = new Node(key, node, NULL, NULL, 0);
    if (node == NULL) {
        /* Tree was empty, set root */
        root = newNode;
    }
    else {
        assert((node->left == NULL) || (node->right == NULL));
        if ( key < node->key )
            node->left = newNode;
        else
            node->right = newNode;
    }
       
    /* Fix AVL property */
    rebalance(newNode);
    return true;
}

void AVLTree::unlink(Node * node) {
    assert( (node->left == NULL) || (node->right == NULL) );
    Node * child = node->left != NULL? node->left : node->right;
    
    /* NULL node's child pointers */
    if (node->left != NULL)
        node->left = NULL;
    else
        node->right = NULL;

    if ( node == root  ) {
        assert(node->parent == NULL);
        root = child;
        if ( child != NULL )
            child->parent = NULL;
    }
    else {
        Node * parent = node->parent;
        node->parent = NULL;  //NULL node's parent pointer
        
        if ( parent->left == node ) {
            /* node was right child of parent */
            parent->left = child;
            if ( child != NULL )
                child->parent = parent;
        }
        else {
            /* node was right child of parent */
            parent->right = child;
            if ( child != NULL )
                child->parent = parent;
        }
        
    }
     
}

bool AVLTree::erase(const int & key) {
    Node * node = search(key);
    
    if ( node == NULL || node->key != key ) {
        return false;
    }
    
    /* Key is in tree, delete it */
    Node * parent;
    
    if ( (node->left== NULL) || (node->right == NULL) ) {
        /* Node does NOT have two children */
        parent = node->parent;
        unlink(node);
        delete(node);
    }
    else {
        Node * pred = predecessor(node);
        /* swap keys */
        int temp = node->key;
        node->key = pred->key;
        pred->key = temp;
        parent = pred->parent;
        
        unlink(pred);
        delete(pred);
    }
    
    rebalance(parent); 
    return true;
}


AVLTree * AVLTree::join(AVLTree * leftTree, AVLTree * rightTree) {
    /* Assumption: rightTree's smallest key > this tree's largest key */
    AVLTree * joinedTree = new AVLTree();
    
    /* Check if left tree has keys */
    if(leftTree->root == NULL){
        joinedTree->root = rightTree->root;
        return joinedTree;
    }
    
    /* Check if right tree has keys */
    if(rightTree->root == NULL){
        joinedTree->root = leftTree->root;
        return joinedTree;
    }
    
    /* Both trees are non-empty, non-trivial join */
    int leftHeight = leftTree->root->height;
    int rightHeight = rightTree->root->height;
    
    if ( leftHeight >= rightHeight ) {
        int minKey = rightTree->minKey();
        rightTree->erase(minKey);
        int newRightHeight = rightTree->root->height;
        
        Node * newNode = new Node(minKey, NULL, NULL, NULL, 0);
        Node * u = NULL;
        Node * v = leftTree->root;
        int currHeight = leftHeight;
        while (currHeight > newRightHeight + 1) {
            if (leftTree->getBalance(v) == -1) {
                currHeight = currHeight - 2;
            }
            else {
                currHeight = currHeight - 1;
            }
            u = v;
            v = v->right;
        }
        newNode->left = v;
        if ( v != NULL ) {
            v->parent = newNode;
        }
        newNode->right = rightTree->root;
        if (rightTree->root != NULL) {
            rightTree->root->parent = newNode;
        }
        
        joinedTree->updateHeight(newNode);
        
        if ( u == NULL ) {
            joinedTree->root = newNode;
        }
        else if (u->left == v) {
            u->left = newNode;
            newNode->parent = u;
            joinedTree->root = leftTree->root;
        }
        else {
            u->right = newNode;
            newNode->parent = u;
            joinedTree->root = leftTree->root;
        }
        joinedTree->rebalance(u);
    }
    else { /* symmetric case */
        int maxKey = leftTree->maxKey();
        leftTree->erase(maxKey);
        int newLeftHeight = leftTree->root->height;
        
        Node * newNode = new Node(maxKey, NULL, NULL, NULL, 0);
        
        Node * u = NULL;
        Node * v = rightTree->root;
        int currHeight = rightHeight;
        while (currHeight > newLeftHeight + 1) {
            if (rightTree->getBalance(v) == 1) {
                currHeight = currHeight - 2;
            }
            else {
                currHeight = currHeight - 1;
            }
            u = v;
            v = v->left;
        }
        
        newNode->right = v;
        if ( v != NULL ) {
            v->parent = newNode;
        }
        newNode->left = leftTree->root;
        if (leftTree->root != NULL) {
            leftTree->root->parent = newNode;
        }   
        joinedTree->updateHeight(newNode);
        
        if ( u == NULL ) {
            joinedTree->root = newNode;
        }
        else if (u->left == v) {
            u->left = newNode;
            newNode->parent = u;
            joinedTree->root = rightTree->root;
        }
        else {
            u->right = newNode;
            newNode->parent = u;
            joinedTree->root = rightTree->root;
        }
        joinedTree->rebalance(u);
    }
    return joinedTree;
}

std::tuple<int, AVLTree *, AVLTree *> AVLTree::split(AVLTree * tree) {
    Node * leftRoot = NULL;
    Node * rightRoot = NULL;
    
    int splitKey;
    if (tree->root == NULL) {
        /* empty tree */
        return std::make_tuple(INVALID_SPLIT_KEY, nullptr, nullptr);
    }
    else if (tree->root->left == NULL && tree->root->right == nullptr) {
        /* Tree only has one node */
        return std::make_tuple(INVALID_SPLIT_KEY, nullptr, nullptr);
    }
    else if (tree->root->left == NULL) {
        /* root's left child is NULL, right child is non-NULL */
        splitKey = tree->root->right->key;
        rightRoot = tree->root->right;
        rightRoot->parent = NULL;
        tree->root->right = NULL;
        leftRoot = tree->root;
    }
    else {
        /* root's left child is non-NULL */
        splitKey = tree->root->key;
        leftRoot = tree->root->left;
        leftRoot->parent = NULL;
        tree->root->left = NULL;
       
        if (tree->root->right == NULL) {
            /* root does not have a right child */
            rightRoot = tree->root;
        }
        else {
            /* re-insert splitKey into left tree */
            Node * oldRoot = tree->root;
            tree->root = tree->root->right;
            tree->root->parent = NULL;   
            delete(oldRoot);
            tree->insert(splitKey);
            rightRoot = tree->root;
            
        }
    }
    AVLTree * leftTree = new AVLTree();
    leftTree->root = leftRoot;
    AVLTree * rightTree = new AVLTree();
    rightTree->root = rightRoot;
     
    /* TODO Update heights? */
    return std::make_tuple(splitKey, leftTree, rightTree);

}

int AVLTree::sumKeys(Node * node) {
    if (node == NULL)
        return 0;
    return node->key + sumKeys(node->left) + sumKeys(node->right);
}

int AVLTree::sumOfKeys() {
    return sumKeys(root);
}


void AVLTree::printInOrderTraversal(Node * node) {
    if (node->left != NULL)
        printInOrderTraversal(node->left);
    
    printf("%d-", node->key);
    
    if ( node->right != NULL)
        printInOrderTraversal(node->right);
}

void AVLTree::printInOrderTraversal() {
    printf("start-");
    printInOrderTraversal(root);
    printf("end\n");
}

bool AVLTree::doesAVLHold(Node * node) {
    bool holds = true;
    
    if (node->left != NULL)
        holds *= doesAVLHold(node->left);
    if (node->right != NULL)
        holds *= doesAVLHold(node->right);    
   
    holds *= abs(getHeight(node->left) - getHeight(node->right)) <= 1 ? true : false;
    
    if (holds == false) {
        cout << "Left Height: " << getHeight(node->left) << endl;
        //printBFSOrder(node->left);
        cout << "Right Height: " << getHeight(node->right) << endl;
        //printBFSOrder(node->right);
    }
    return holds; 
    
}

bool AVLTree::checkAVL( ) {
    return doesAVLHold(root);
}

void AVLTree::printBFSOrder(Node * node) {
    if (node != NULL) {
        queue<Node *> q;;
        q.push(node);
        printf("start-");
        while ( !q.empty() ) {
            Node * curr = q.front();
            q.pop();
            int parentKey = curr->parent == NULL? 0 : curr->parent->key;
            printf("%d(p(%d)->", curr->key, parentKey);
            if (curr->left != NULL)
                q.push(curr->left);
            if (curr->right != NULL)
                q.push(curr->right);
        }
        printf("end\n"); 
    }
}

void AVLTree::printBFSOrder() {
    printBFSOrder(root);
}

#endif /* AVL_TREE_H */

