/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#ifndef INTERFACES_H
#define INTERFACES_H

#include <cassert>

/**
 * Base class for CA Tree? AVL Tree, Red-Black Tree and Splay Tree should inherit
 */
class IBinarySearchTree {
public:
    virtual bool contains(const int & key) = 0;
    virtual bool insert(const int & key) = 0;
    virtual bool erase(const int & key) = 0;
    /* TODO add join and split */
    //virtual void traversal(const int & key) = 0;
};

class CA_Node {
    /* TODO add something here? */
public:
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual bool isValid() = 0;
    virtual void invalidate() = 0;
};

#endif /* INTERFACES_H */
