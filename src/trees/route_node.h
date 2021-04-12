/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   route_node.h
 * Author: ginns
 *
 * Created on March 22, 2021, 12:06 AM
 */

#ifndef ROUTE_NODE_H
#define ROUTE_NODE_H

#include <mutex>
#include "interfaces.h"

class RouteNode : public CA_Node {
private:
    std::mutex m;
    bool valid;
    int key;
    CA_Node * left; // TODO volatile?
    CA_Node * right; //TODO volatile?
    //RouteNode parent;
    
public:
    RouteNode(int key, CA_Node * left, CA_Node * right);
    ~RouteNode();
    int getKey();
    CA_Node * getLeft();
    CA_Node * getRight();
    void setLeft(CA_Node * node);
    void setRight(CA_Node * node);
    
    /* lock methods */
    void lock() override;
    void unlock() override;

    /* valid status methods */
    void invalidate() override;
    bool isValid() override;
};


RouteNode::RouteNode(int _key, CA_Node * _left, CA_Node * _right) 
                            : key(_key), left(_left), right(_right) {
    valid = true;
}

RouteNode::~RouteNode() {
    
}

CA_Node * RouteNode::getLeft() {
    return left;
}

void RouteNode::setLeft(CA_Node * node) {
    left = node;
}

CA_Node * RouteNode::getRight() {
    return right;
}

void RouteNode::setRight(CA_Node * node) {
    right = node;
}

int RouteNode::getKey() {
    return key;
}

void RouteNode::invalidate() {
    valid = false;
}

bool RouteNode::isValid() {
    return valid;
}


void RouteNode::lock() {
    m.lock();
}

void RouteNode::unlock() {
    m.unlock();
}


#endif /* ROUTE_NODE_H */

