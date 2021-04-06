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
#include "util.h"

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
        
        bool isLeaf( ) {
            return (left == NULL) && (right == NULL);
        }
        
        bool isParentOf(Node * other) {
            return (left == other || right == other);
        }
    };
    
    Node * root;
    
    int getHeight(Node * node);
    void updateHeight(Node * node);
    void rebalance(Node * node);
    void leftRotate(Node * node);
    void rightRotate(Node * node);
    Node * search(const int & key);
    Node * successor(Node * node);
    Node * predecessor(Node * node);
    Node * get_min(Node * node);
    Node * get_max(Node * node);
    void unlink(Node * node);
    
    void printInOrderTraversal(Node * node);
    bool doesAVLHold(Node * node);
    void freeTraversal(Node * node);
    int sumKeys(Node * node);
public:
    AVLTree();
    ~AVLTree();
    virtual bool contains(const int & key);
    virtual bool insert(const int & key);
    virtual bool erase(const int & key);
    int sumOfKeys();
    //virtual void traversal(const int & key);
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

AVLTree::Node * AVLTree::get_max(Node * node) {
    Node * curr = node;
    while ( curr->right != NULL) {
        curr = curr->right;
    }
    return curr;
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
    
    if ( node == NULL || node->key != key )
        return false;
    
    /* Key is in tree, delete it */
    Node * origParent = node->parent;
    
    if ( (node->left== NULL) || (node->right == NULL) ) {
        /* Node does NOT have two children */
        unlink(node);
        delete(node);
    }
    else {
        Node * pred = predecessor(node);
        
        /* swap keys */
        int temp = node->key;
        node->key = pred->key;
        pred->key = temp;
        
        /* pred contains key, and should be easier to delete */
        unlink(pred);
        delete(pred);
    }
    
    if (origParent == NULL )
        rebalance(root);
    else
        rebalance(origParent); 
    return true;
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
    
    return holds; 
    
}

bool AVLTree::checkAVL( ) {
    return doesAVLHold(root);
}



void AVLTree::printBFSOrder() {

    queue<Node *> q;;
    q.push(root);
    printf("start-");
    while ( !q.empty() ) {
        Node * curr = q.front();
        q.pop();
        printf("%d->", curr->key);
        if (curr->left != NULL)
            q.push(curr->left);
        if (curr->right != NULL)
            q.push(curr->right);
    }
    printf("end\n");
}

#endif /* AVL_TREE_H */

