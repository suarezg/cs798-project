/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   redblack_tree.h
 * Author: ginns
 *
 * Created on April 21, 2021, 11:42 AM
 */

#ifndef REDBLACK_TREE_H
#define REDBLACK_TREE_H

#include <algorithm>    /* for max */
#include <queue>        /* used in BFS */
#include <cassert>
#include <iostream>


#include "interfaces.h"

#define INVALID_KEY     -1

using namespace std;
/**
 * Based on implementation found in Introduction to Algorithms [CLRS]
 */
class RedBlackTree : public IOrderedSet {
private:
    enum Color { RED, BLACK };
    struct Node {
        int key;
        Node * parent;
        Node * left;
        Node * right;
        Color color;
        
        Node(int _key, Node * _parent, Node * _left, Node * _right, Color _color) :
            key(_key), parent(_parent), left(_left), right(_right), color(_color) {
            
        }
        
        bool isBlack() {
            return color == BLACK;
        }
    };
    
    void insertFixup(Node * node);
    void eraseFixup(Node * node);
    void transplant(Node * u, Node * v);
    Node * tree_minimum(Node * node);
    
    void leftRotate(Node * x);
    void rightRotate(Node * x);
    
    int blackHeight(Node * node);
    void printBFSOrder(Node * node);
    int sumKeys(Node * node);
    
    Node * root;
    Node * sentinel;
    int size;
    
public:
    
    RedBlackTree();
    ~RedBlackTree();
        
    bool contains(const int & key);
    bool insert(const int & key);
    bool erase(const int & key);
    
    IOrderedSet * join(IOrderedSet * rightSet);
    std::tuple<int, IOrderedSet *, IOrderedSet *> split();
    
    int sumOfKeys();
    int getSize();
    void printKeys();
    
    int getBlackHeight();
    void printBFSOrder();
    
};

/**
 * 
 */


RedBlackTree::RedBlackTree() {
    sentinel = new Node(INVALID_KEY, NULL, NULL, NULL, BLACK);
    root = sentinel;
    size = 0;
}

RedBlackTree::~RedBlackTree() {
    
}


bool RedBlackTree::contains(const int & key) {
    Node * currNode = root;
    while (currNode != sentinel) {
        int nodeKey = currNode->key;
        if ( key < nodeKey ) {
            currNode = currNode->left;
        }
        else if ( key > nodeKey ) {
            currNode = currNode->right;
        }
        else { /* key found */
            return true;
        }
    }
    return false;
}

void RedBlackTree::leftRotate(Node * x) {
    Node * y = x->right;
    x->right = y->left;
    
    if (y->left != sentinel) {
        y->left->parent = x;
    }
    y->parent = x->parent;
 
    if (x->parent == sentinel) {
        root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else  {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RedBlackTree::rightRotate(Node * x) {
    Node * y = x->left;
    x->left = y->right;
    
    if (y->right != sentinel) {
        y->right->parent = x;
    }
    y->parent = x->parent;
 
    if (x->parent == sentinel) {
        root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else  {
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
}

void RedBlackTree::insertFixup(Node * z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node * y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        }
        else {
            /* symmetric case */
            Node * y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

bool RedBlackTree::insert(const int & key) {
    Node * prevNode = sentinel;
    Node * currNode = root;
    while (currNode != sentinel) {
        prevNode = currNode;
        if (key < currNode->key) {
            currNode = currNode->left;
        }
        else if (key > currNode->key) {
            currNode = currNode->right;
        }
        else {
            /* key already exists */
            return false;
        }
    }
    
    /* New key, insert into tree */
    size++;
    Node * newNode = new Node(key, prevNode, sentinel, sentinel, RED);
    if ( prevNode == sentinel ) {
        root = newNode;
    }
    else if (newNode->key < prevNode->key) {
        prevNode->left = newNode;
    }
    else {
        prevNode->right = newNode;
    }
    insertFixup(newNode);
    
    size++;
    return true;
}

void RedBlackTree::transplant(Node* u, Node* v) {
    if (u->parent == sentinel) {
        root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

RedBlackTree::Node * RedBlackTree::tree_minimum(Node * node) {
    Node * curr = node;
    while (curr->left != sentinel) {
        curr = curr->left;
    }
    return curr;
}

void RedBlackTree::eraseFixup(Node* node) {
    Node * x = node;
    Node * w;
    while ( (x != root) && (x->color == BLACK) ) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if ( (w->left->color == BLACK) && (w->right->color == BLACK) ) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        }
        else {
            /* Symmetric case */
            w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if ( (w->right->color == BLACK) && (w->left->color == BLACK) ) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

bool RedBlackTree::erase(const int & key) {
    /* Find node */
    Node * z = root;
    while (z != sentinel) {
        if (key < z->key) {
            z = z->left;
        }
        else if (key > z->key) {
            z = z->right;
        }
        else { /* found node with key */
            break;
        }
    }
    
    
    if ( z == sentinel )
        return false;  /* key is not in tree */
    
    /* Delete node */
    Node * x;
    Node * y = z;
    Color yOriginalColor = y->color;
    if (z->left == sentinel) {
        x = z->right;
        transplant(z, z->right);
    }
    else if (z->right == sentinel) {
        x = z->left;
        transplant(z, z->left);
    }
    else {
        y = tree_minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if ( y->parent == z ) {
            x->parent = y;
        }
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    
    if (yOriginalColor == BLACK)
        eraseFixup(x);
    
    size--;
    return true;
}

IOrderedSet * RedBlackTree::join(IOrderedSet * rightSet) {
    return NULL;
}


std::tuple<int, IOrderedSet *, IOrderedSet *> RedBlackTree::split() {
    return std::make_tuple(INVALID_KEY, nullptr, nullptr);
}

int RedBlackTree::sumKeys(Node* node) {
    if (node == sentinel)
        return 0;
    return node->key + sumKeys(node->left) + sumKeys(node->right);
}

int RedBlackTree::sumOfKeys() {
    return sumKeys(root);
}

int RedBlackTree::getSize() {
    return size;
}

void RedBlackTree::printKeys() {
    
}

/**
 * From StackOverflow 
 * https://stackoverflow.com/questions/13848011/how-to-check-the-black-height-of-a-node-for-all-paths-to-its-descendent-leaves
 */
int RedBlackTree::blackHeight(Node* node) {
    if (node == sentinel) {
        return 1;
    }
    
    int leftBlackHeight = blackHeight(node->left);
    if (leftBlackHeight == 0)
        return leftBlackHeight;
    
    int rightBlackHeight = blackHeight(node->right);
    if (rightBlackHeight == 0)
        return rightBlackHeight;
    
    assert(leftBlackHeight != 0);
    assert(rightBlackHeight != 0);
    
    if (leftBlackHeight != rightBlackHeight) {
        return 0;
    }
    else {
        int blackHeight = leftBlackHeight + (node->isBlack() ? 1 : 0);
        //printf("Node(%d) black height: %d\n", node->key, blackHeight);
        return blackHeight;
    }
}


int RedBlackTree::getBlackHeight() {
    return blackHeight(root);
}

void RedBlackTree::printBFSOrder(Node * node) {
    if (node != sentinel) {
        queue<Node *> q;
        q.push(node);
        printf("start-");
        while ( !q.empty() ) {
            Node * curr = q.front();
            q.pop();
            int parentKey = curr->parent == sentinel? INVALID_KEY : curr->parent->key;
            char c = curr->color == BLACK? 'b' : 'r';
            printf("%d_%c(p(%d))->", curr->key, c, parentKey);
            if (curr->left != sentinel)
                q.push(curr->left);
            if (curr->right != sentinel)
                q.push(curr->right);
        }
        printf("end\n"); 
    }
}

void RedBlackTree::printBFSOrder() {
    printBFSOrder(root);
}





#endif /* REDBLACK_TREE_H */

