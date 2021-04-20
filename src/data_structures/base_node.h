/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   base_node.h
 * Author: ginns
 *
 * Created on April 7, 2021, 4:47 PM
 */

#ifndef BASE_NODE_H
#define BASE_NODE_H

#include "route_node.h"
#include "avl_tree.h"

class BaseNode : public CA_Node {
private:
    
    /* TODO Padding? */
    bool valid;
    //int size = 0;
    RouteNode * parent;
    AVLTree * set; /* TODO replace with IBinarySearchTree interface */
    
    /* For statistic locking */
    std::mutex m;
    int statLockStatistics;
    
    /* Contention heuristics */
    static const int STAT_LOCK_HIGH_CONTENTION_LIMIT = 1000;
    static const int STAT_LOCK_LOW_CONTENTION_LIMIT = -1000;
    static const int STAT_LOCK_FAILURE_CONTRIB = 250;
    static const int STAT_LOCK_SUCCESS_CONTRIB = 1;
    
public:
    
    BaseNode();
    ~BaseNode();
    void setParent(RouteNode * newParent);
    RouteNode * getParent();
    
    /* Valid Status functions */
    bool isValid() override;
    void invalidate() override;
    
    /* Statistic lock methods */
    void lock() override;
    void unlock() override;
    bool tryLock();
    int getStatistics();
    void resetStatistics();
    int getHighContentionLimit();
    int getLowContentionLimit();
    bool isHighContentionLimitReached();
    bool isLowContentionLimitReached();
    
    /* */
    void setOrderedSet(AVLTree * set);
    AVLTree * getOrderedSet();
};

BaseNode::BaseNode() {
    valid = true;
    statLockStatistics = 0;
    parent = NULL; 
}

BaseNode::~BaseNode() {
    
}

void BaseNode::setParent(RouteNode * node) {
    parent = node;
}

RouteNode * BaseNode::getParent() {
    return parent;
}

void BaseNode::setOrderedSet(AVLTree * set) {
    this->set = set;
}

AVLTree * BaseNode::getOrderedSet() {
    return set;
}

/**
 * Valid status methods
 */
void BaseNode::invalidate() {
    valid = false;
}

bool BaseNode::isValid() {
    return valid;
}


/**
 * Statistic Lock Methods
 * 
 */
bool BaseNode::tryLock() {
    return m.try_lock();
}

void BaseNode::lock() {
    if ( m.try_lock() ) {
        statLockStatistics -= STAT_LOCK_SUCCESS_CONTRIB;
        return;
    }
    m.lock(); /* Wait and lock */
    statLockStatistics += STAT_LOCK_FAILURE_CONTRIB;
}

void BaseNode::unlock() {
    m.unlock();
}

int BaseNode::getStatistics() {
    return statLockStatistics;
}

void BaseNode::resetStatistics() {
    statLockStatistics = 0;
}

int BaseNode::getHighContentionLimit() {
    return STAT_LOCK_HIGH_CONTENTION_LIMIT;
}

int BaseNode::getLowContentionLimit() {
    return STAT_LOCK_LOW_CONTENTION_LIMIT;
}

bool BaseNode::isHighContentionLimitReached() {
    return statLockStatistics > STAT_LOCK_HIGH_CONTENTION_LIMIT;
}

bool BaseNode::isLowContentionLimitReached() {
    return statLockStatistics < STAT_LOCK_LOW_CONTENTION_LIMIT;
}


#endif /* BASE_NODE_H */

