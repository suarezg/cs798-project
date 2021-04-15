/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ca_tree.h
 * Author: ginns
 *
 * Created on April 7, 2021, 5:05 PM
 */

#ifndef CA_TREE_H
#define CA_TREE_H

#include "base_node.h"
#include "../util.h"
#include <queue>        /* used in BFS */


/*
template<typename Base, typename T>
inline bool instanceof(const T*) {
   return is_base_of<Base, T>::value;
}
*/

class CATree {
private:
    int minKey;
    int maxKey;
    int numThreads;
    volatile char padding0[PADDING_BYTES];
    
    CA_Node * root;
    /* Contention Adapting */
    void highContentionSplit(int tid, BaseNode * baseNode);
    void lowContentionJoin(int tid, BaseNode * baseNode);
    void adaptIfNeeded(int tid, BaseNode * baseNode);
    
    /* Helpers */
    BaseNode * getBaseNode(const int & key);
    BaseNode * leftmostBaseNode(CA_Node * node);
    BaseNode * rightMostBaseNode(CA_Node * node);
    RouteNode * parentOf(RouteNode * node);
    
    long calcSubtreeKeySum(CA_Node * node);
    
public:
    CATree(int totalThreads, int minKey, int maxKey);
    ~CATree();
    
    /* Set Operations */
    bool contains(int tid, const int & key);
    bool insert(int tid, const int & key);
    bool erase(int tid, const int & key);
    
    void printDebuggingDetails();
    long getSumOfKeys();
    
};

CATree::CATree(int _numThreads, int _minKey, int _maxKey) : numThreads(_numThreads), minKey(_minKey), maxKey(_maxKey) {
    BaseNode * baseRoot = new BaseNode();
    AVLTree * tree = new AVLTree();
    baseRoot->setOrderedSet(tree);
    root = baseRoot;
}

CATree::~CATree() {
    
}


BaseNode * CATree::getBaseNode(const int & key) {
    CA_Node * currNode = root;
    while ( RouteNode * currNodeR = dynamic_cast<RouteNode *>(currNode) ) {
        if ( currNodeR->getKey() < key ) {
            currNode = currNodeR->getLeft();
        }
        else {
            currNode = currNodeR->getRight();
        }
    }
    BaseNode * baseNode = dynamic_cast<BaseNode *>(currNode);
    return baseNode;
}

BaseNode * CATree::leftmostBaseNode(CA_Node * node) {
    CA_Node * currNode = node;
    while ( RouteNode * r = dynamic_cast<RouteNode *>(currNode) ) {
        currNode = r->getLeft();
    }
    /* Drop out when we reach base node, cast and return */
    BaseNode * baseNode = dynamic_cast<BaseNode *>(currNode);
    return baseNode;
}

BaseNode * CATree::rightMostBaseNode(CA_Node* node) {
    CA_Node * currNode = node;
    while ( RouteNode * r = dynamic_cast<RouteNode *>(currNode) ) {
        currNode = r->getRight();
    }
    /* Drop out when we reach base node, cast and return */
    BaseNode * baseNode = dynamic_cast<BaseNode *>(currNode);
    return baseNode;
}

RouteNode * CATree::parentOf(RouteNode* node) {
    CA_Node *prevNode = NULL;
    CA_Node * currNode = root;
    int targetKey = node->getKey();
    while (currNode != node) {
        RouteNode * currNodeR = dynamic_cast<RouteNode *>(currNode);
        prevNode = currNode;
        if (currNodeR->getKey() < targetKey)
            currNode = currNodeR->getLeft();
        else
            currNode = currNodeR->getRight();
    }
    
    RouteNode * parent = dynamic_cast<RouteNode *>(prevNode);
    return parent;
}

void CATree::adaptIfNeeded(int tid, BaseNode* baseNode) {
    
    if (baseNode->isHighContentionLimitReached()) {
        TRACE TPRINT("Attempting high contention split");
        highContentionSplit(tid, baseNode);
    }
    else if (baseNode->isLowContentionLimitReached()) {
        TRACE TPRINT("Attempting low contention join");
        lowContentionJoin(tid, baseNode);
    }
}

void CATree::lowContentionJoin(int tid, BaseNode * baseNode) {
    RouteNode * parent = baseNode->getParent();
    if (parent == NULL) {
        baseNode->resetStatistics();
    }
    else if (parent->getLeft() == baseNode) {
        BaseNode * neighborBase = leftmostBaseNode( parent->getRight() );
        if ( !neighborBase->tryLock() ){
            baseNode->resetStatistics();
            return;
        }
        else if ( !neighborBase->isValid() ) {
            neighborBase->unlock();
            baseNode->resetStatistics();
            return;
        }
        else {
            AVLTree * baseSet = baseNode->getOrderedSet();
            AVLTree * neighborSet = neighborBase->getOrderedSet();
            AVLTree * joinedSet = AVLTree::join(baseSet, neighborSet);
            BaseNode * newBase = new BaseNode();
            newBase->setOrderedSet(joinedSet);
            parent->lock();
            RouteNode * gparent = NULL;
            
            /* Unlink parent route node */
            do {
                if (gparent != NULL) {
                    gparent->unlock();
                }
                gparent = parentOf(parent);
                if (gparent != NULL) {
                    gparent->lock();
                }
            } while (gparent != NULL & !gparent->isValid());
            if (gparent == NULL) {
                root = parent->getRight();
            }
            else if ( gparent->getLeft() == parent) {
                gparent->setLeft( parent->getRight() ); 
            }
            else {
                gparent->setRight( parent->getRight() );
            }
            
            parent->invalidate();
            parent->unlock();
            if (gparent != NULL) {
                gparent->unlock();
            }
            
            /* Link new base node */
            RouteNode * neighborBaseParent = NULL;
            if (parent->getRight() == neighborBase) {
                neighborBaseParent = gparent;
            }
            else  {
                neighborBaseParent = neighborBase->getParent(); 
            }
            newBase->setParent(neighborBaseParent);
            if ( neighborBaseParent == NULL ) {
                root = newBase;
            }
            else if ( neighborBaseParent->getLeft() == neighborBase ) {
                neighborBaseParent->setLeft(newBase);
            }
            else {
                neighborBaseParent->setRight(newBase);
            }
            neighborBase->invalidate();
            neighborBase->unlock();
            baseNode->invalidate();
            
            TRACE TPRINT("Executed low contention join");
        }
    }
    else { /* Symmetric case */
        BaseNode * neighborBase = leftmostBaseNode( parent->getLeft() );
        if ( !neighborBase->tryLock() ){
            baseNode->resetStatistics();
            return;
        }
        else if ( !neighborBase->isValid() ) {
            neighborBase->unlock();
            baseNode->resetStatistics();
            return;
        }
        else {
            AVLTree * baseSet = baseNode->getOrderedSet();
            AVLTree * neighborSet = neighborBase->getOrderedSet();
            AVLTree * joinedSet = AVLTree::join(neighborSet, baseSet);
            BaseNode * newBase = new BaseNode();
            newBase->setOrderedSet(joinedSet);
            parent->lock();
            RouteNode * gparent = NULL;
            
             /* Unlink parent route node */
            do {
                if (gparent != NULL) {
                    gparent->unlock();
                }
                gparent = parentOf(parent);
                if (gparent != NULL) {
                    gparent->lock();
                }
            } while (gparent != NULL & !gparent->isValid());
            if (gparent == NULL) {
                root = parent->getLeft();
            }
            else if ( gparent->getLeft() == parent) {
                gparent->setLeft( parent->getLeft() ); 
            }
            else {
                gparent->setRight( parent->getLeft() );
            }
            
            parent->invalidate();
            parent->unlock();
            if (gparent != NULL) {
                gparent->unlock();
            }
            
            /* Link new base node */
            RouteNode * neighborBaseParent = NULL;
            if (parent->getLeft() == neighborBase) {
                neighborBaseParent = gparent;
            }
            else  {
                neighborBaseParent = neighborBase->getParent(); 
            }
            newBase->setParent(neighborBaseParent);
            if ( neighborBaseParent == NULL ) {
                root = newBase;
            }
            else if ( neighborBaseParent->getLeft() == neighborBase ) {
                neighborBaseParent->setLeft(newBase);
            }
            else {
                neighborBaseParent->setRight(newBase);
            }
            neighborBase->invalidate();
            neighborBase->unlock();
            baseNode->invalidate();
            
            TRACE TPRINT("Executed low contention join");
        }
    }
} // end lowContentionJoin

void CATree::highContentionSplit(int tid, BaseNode * baseNode) {
    RouteNode * parent = baseNode->getParent();
    AVLTree * baseSet = baseNode->getOrderedSet();
    std::tuple<int, AVLTree *, AVLTree *> tuple = AVLTree::split(baseSet);
    int splitKey = std::get<0>(tuple);
    AVLTree * leftTree = std::get<1>(tuple);
    AVLTree * rightTree = std::get<1>(tuple);
    
    if (leftTree == nullptr) {
        /* Occurs when split is not possible (one node set) */
        baseNode->resetStatistics();
        return;
    }
    
    BaseNode * newLeftBase = new BaseNode();
    BaseNode * newRightBase = new BaseNode();
    newLeftBase->setOrderedSet(leftTree);
    newRightBase->setOrderedSet(rightTree);
    
    RouteNode * newRoute = new RouteNode(splitKey, newLeftBase, newRightBase);
    
    if ( parent == NULL ) {
        root = newRoute;
    }
    else {
        if ( parent->getLeft() == baseNode ) {
            parent->setLeft(newRoute);
        }
        else {
            parent->setRight(newRoute);
        }
    }
    baseNode->invalidate();
    TRACE TPRINT("Executed high contention split, splitKey: " << splitKey);        
} // end highContentionSplit


/** Helper method for calculating the sum of the keys IN THE ORDERED SETS that 
 *  are in the subtree rooted at node
 * 
 * @param node
 * @return subtree sum
 */
long CATree::calcSubtreeKeySum(CA_Node* node) {
    assert(node != NULL);
    long sum = 0;
    if ( BaseNode * baseNode = dynamic_cast<BaseNode *>(node)) {
        /* node is a base node, get ordered set */
        sum = baseNode->getOrderedSet()->sumOfKeys();
    }
    else {
        RouteNode * routeNode = dynamic_cast<RouteNode *>(node); //use static_cast?
        if ( routeNode->getLeft() != NULL ) 
            sum += calcSubtreeKeySum(routeNode->getLeft());
        if (routeNode->getRight() != NULL )
            sum+= calcSubtreeKeySum(routeNode->getRight());
    }
    return sum;
}

/**
 * Public Methods
 */
bool CATree::contains(int tid, const int & key) {
    assert( (key >= minKey) && (key <= maxKey) );
    while (true) {
        bool result;
        BaseNode * baseNode = getBaseNode(key);
        baseNode->lock();
        
        if (baseNode->isValid() == false) {
            baseNode->unlock();
            continue;
        }
        AVLTree * set = baseNode->getOrderedSet();
        result = set->contains(key);
        adaptIfNeeded(tid, baseNode);
        baseNode->unlock();
        return result;
    }
}

bool CATree::insert(int tid, const int & key) {
    assert( (key >= minKey) && (key <= maxKey) );
    //TRACE TPRINT("Inserting key: " << key);
    while (true) {
        //TRACE TPRINT("Inserting key: " << key);
        bool result;
        BaseNode * baseNode = getBaseNode(key);
        baseNode->lock();
        
        if (baseNode->isValid() == false) {
            baseNode->unlock();
            continue;
        }
        AVLTree * set = baseNode->getOrderedSet();
        result = set->insert(key);
        if (result) {
            TRACE TPRINT("Inserted key " << key);
        }
        else {
            TRACE TPRINT("Failed to insert key " << key);
        }
        
        adaptIfNeeded(tid, baseNode);
        baseNode->unlock();
        return result;
    }
}

bool CATree::erase(int tid, const int & key) {
    assert( (key >= minKey) && (key <= maxKey) );
    while (true) {
        bool result;
        BaseNode * baseNode = getBaseNode(key);
        baseNode->lock();
        
        if (baseNode->isValid() == false) {
            baseNode->unlock();
            continue;
        }
        AVLTree * set = baseNode->getOrderedSet();
        result = set->erase(key);
        adaptIfNeeded(tid, baseNode);
        baseNode->unlock();
        return result;
    }
}



void CATree::printDebuggingDetails() {
    /* Tree walk the CA Tree and count the number of Base Nodes and Route Nodes */
    int numBaseNodes = 0;
    int numRouteNodes = 0;
    queue<CA_Node *> q;
    q.push(root);
    while ( !q.empty() ) {
        CA_Node * curr = q.front();
        q.pop();
        if ( BaseNode * baseNode = dynamic_cast<BaseNode *>(curr) ) {
            /* curr is a base node */
            numBaseNodes++;
        }
        else {
            /* curr is a route node */
            numRouteNodes++;
            RouteNode * routeNode = dynamic_cast<RouteNode *>(curr);
            if ( routeNode->getLeft() != NULL )
                q.push(routeNode->getLeft());
            
            if ( routeNode->getRight() != NULL )
                q.push(routeNode->getRight());       
        }        
    }
    
    cout << "Number of base nodes: " << numBaseNodes << endl;
    cout << "Number of route nodes: " << numRouteNodes << endl;
}

long CATree::getSumOfKeys() {
    return calcSubtreeKeySum(root);
}

#endif /* CA_TREE_H */

