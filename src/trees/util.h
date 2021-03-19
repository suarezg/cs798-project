/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <cassert>


/**
 * Base class for CA Tree? AVL Tree, Red-Black Tree and Splay Tree should inherit
 */
class IBinarySearchTree {
public:
    virtual bool contains(const int & key) = 0;
    virtual bool insert(const int & key) = 0;
    virtual bool erase(const int & key) = 0;
    //virtual void traversal(const int & key) = 0;
};
