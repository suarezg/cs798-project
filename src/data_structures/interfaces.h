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
class IOrderedSet {
public:
    virtual bool contains(const int & key) = 0;
    virtual bool insert(const int & key) = 0;
    virtual bool erase(const int & key) = 0;
    //virtual void printKeys() = 0;
    virtual int sumOfKeys() = 0;
    virtual IOrderedSet * join(IOrderedSet * rightSet) = 0;
    virtual std::tuple<int, IOrderedSet *, IOrderedSet *> split() = 0;
};

class CA_Node {
    /* TODO add something here? */
public:
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual bool isValid() = 0;
    virtual void invalidate() = 0;
};

enum OrderedSetType { AVL, LINKEDLIST };

#endif /* INTERFACES_H */
