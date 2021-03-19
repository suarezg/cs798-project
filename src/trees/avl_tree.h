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
#include <iostream>
#include <iomanip>
#include <queue> 
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
    int get_height(Node * node);
    void update_height(Node * node);
    Node * search(const int & key);
    void rebalance(Node * node);
    void left_rotate(Node * node);
    void right_rotate(Node * node);
    void printInOrderTraversal(Node * node);
    bool doesAVLHold(Node * node);
public:
    AVLTree();
    ~AVLTree();
    virtual bool contains(const int & key);
    virtual bool insert(const int & key);
    virtual bool erase(const int & key);
    //virtual void traversal(const int & key);
    void printInOrderTraversal();
    void printBFSOrder();
    bool checkAVL( );
    
};

int AVLTree::get_height(Node* node) {
    if (node == NULL)
        return -1;
    else
        return node->height;
}

void AVLTree::update_height(Node * node) {
    node->height = max(get_height(node->left), get_height(node->right)) + 1;
}

void AVLTree::rebalance(Node * node) {
    while (node != NULL) {
        update_height(node);
        if ( get_height(node->left) >= (2 + get_height(node->right)) ) {
            if ( get_height(node->left->left) >= get_height(node->left->right) )
                right_rotate(node);
            else {
                left_rotate(node->left);
                right_rotate(node);
            }
        }
        else if ( get_height(node->right) >= (2 + get_height(node->left)) )  {
            if ( get_height(node->right->right) >= get_height(node->right->left) )
                left_rotate(node);
            else {
                right_rotate(node->right);
                left_rotate(node);
            }
        }
        node = node->parent; 
    }
}

void AVLTree::left_rotate(Node * x) {
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
    update_height(x);
    update_height(y);
    
}

void AVLTree::right_rotate(Node * x) {
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
    update_height(x);
    update_height(y);
    
    
}

AVLTree::AVLTree() {
    root = NULL;
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

bool AVLTree::erase(const int & key) {
    Node * node = search(key);
    
    if ( node == NULL || node->key != key )
        return false;
    
    /* Key is in tree, delete it */
    
    return true;
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
   
    holds *= abs(get_height(node->left) - get_height(node->right)) <= 1 ? true : false;
    
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

