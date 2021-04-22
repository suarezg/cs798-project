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

#include <cassert>
#include <iostream>

#include "interfaces.h"

#define INVALID_KEY     -1

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
    };
    
    void insertFixup(Node * node);
    void eraseFixup(Node * node);
    void transplant(Node * u, Node * v);
    
    void leftRotate(Node * x);
    void rightRotate(Node * x);
    
    Node * root;
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

RedBlackTree::RedBlackTree() {
    root = NULL;
    size = 0;
}


bool RedBlackTree::contains(const int & key) {
    return false;
}


bool RedBlackTree::insert(const int & key) {
    return false;
}


bool RedBlackTree::erase(const int & key) {
    return false;
}

IOrderedSet * RedBlackTree::join(IOrderedSet * rightSet) {
    return NULL;
}


std::tuple<int, IOrderedSet *, IOrderedSet *> RedBlackTree::split() {
    return std::make_tuple(INVALID_KEY, nullptr, nullptr);
}

int RedBlackTree::sumOfKeys() {
    return 0;
}

int RedBlackTree::getSize() {
    return size;
}

void RedBlackTree::printKeys() {
    
}

int RedBlackTree::getBlackHeight() {
    return 0;
}

 void RedBlackTree::printBFSOrder() {
     
 }





#endif /* REDBLACK_TREE_H */

