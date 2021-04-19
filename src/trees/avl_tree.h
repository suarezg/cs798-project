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
#include <iostream>
#include "interfaces.h"

#define INVALID_KEY     -1

/* Sequential AVL Tree */
using namespace std;
class AVLTree : public IBinarySearchTree {
private:
    struct Node {
        int key;
        Node * parent;
        Node * left;
        Node * right;
        int balance;
        
        Node(int _key, Node * _parent, Node * _left, Node * _right) :
            key(_key), parent(_parent), left(_left), right(_right), balance(0) {
            
        }
    };
    
    Node * root;
    int size;
    
    /* AVL Tree maintenance and balancing methods */
    int computeHeight();
    int minKey();
    int maxKey();
    Node * getAVLNode(const int & key);
    void rotateLeft(Node * prevNode);
    void rotateRight(Node * prevNode);
    void rotateDoubleRight(Node * prevNode);
    void rotateDoubleLeft(Node * prevNode);
    bool replaceWithRightmost(Node * toReplaceInNode);
    bool deleteBalanceLeft(Node * currentNode);
    bool deleteBalanceRight(Node * currentNode);
    
    
    /* Debugging methods */
    void printInOrderTraversal(Node * node);
    bool doesAVLHold(Node * node);
    int sumKeys(Node * node);
    void printBFSOrder(Node * node);
    
    /* Memory reclamation */
    void freeTraversal(Node * node);
    
public:
   
    AVLTree();
    ~AVLTree();
    
    /* Dictionary operations  */
    virtual bool contains(const int & key);
    virtual bool insert(const int & key);
    virtual bool erase(const int & key);
    
    /* Set operations */
    static AVLTree * join(AVLTree * leftTree, AVLTree * rightTree);
    static std::tuple<int, AVLTree *, AVLTree *> split(AVLTree * tree);
    
    /* Useful methods */
    int getSize();
    void setSize(int size);
    bool isEmpty();
    
    /* Debugging methods */
    int sumOfKeys();
    void printInOrderTraversal();
    void printBFSOrder();
    bool checkAVL( );
    
};

int AVLTree::computeHeight() {
    if (root == NULL) {
        return 0;
    }
    else {
        Node * currentNode = root;
        int heightSoFar = 1;
        while (currentNode->left != NULL || currentNode->right != NULL) {
            if (currentNode->balance == -1) {
                currentNode = currentNode->left;
            }
            else {
                currentNode = currentNode->right;
            }
            heightSoFar++;
        }
        return heightSoFar;
    }
}

int AVLTree::minKey() {
    Node * currentNode = root;
    while (currentNode->left != NULL) {
        currentNode = currentNode->left;
    }
    if ( currentNode == NULL ) {
        return INVALID_KEY;
    }
    else {
        return currentNode->key;
    }
}

int AVLTree::maxKey() {
    Node * currentNode = root;
    while (currentNode->right != NULL) {
        currentNode = currentNode->right;
    }
    if ( currentNode == NULL ) {
        return INVALID_KEY;
    }
    else {
        return currentNode->key;
    }
}

AVLTree::Node * AVLTree::getAVLNode(const int & key) {
    Node * currentNode = root;
    while (currentNode != NULL) {
        int nodeKey = currentNode->key;
        if ( key < nodeKey ) {
            currentNode = currentNode->left;
        }
        else if ( key > nodeKey ) {
            currentNode = currentNode->right;
        }
        else {
            return currentNode;
        }
    }
    return NULL;
}

void AVLTree::rotateLeft(Node* prevNode) {
    /* Single left rotation */
    Node * leftChild = prevNode->left;
    Node * prevNodeParent = prevNode->parent;
    prevNode->left = leftChild->right;
    if (prevNode->left != NULL) {
        prevNode->left->parent = prevNode;
    }
    leftChild->right = prevNode;
    prevNode->parent = leftChild;
    prevNode->balance = 0;
    if (prevNodeParent == NULL) {
        root = leftChild;
    }
    else if (prevNodeParent->left == prevNode) {
        prevNodeParent->left = leftChild;
    }
    else {
        prevNodeParent->right = leftChild;
    }
    leftChild->parent = prevNodeParent;
    leftChild->balance = 0;
}

void AVLTree::rotateRight(Node * prevNode) {
    /* Single right rotation */
    Node * rightChild = prevNode->right;
    Node * prevNodeParent = prevNode->parent;
//    printf("\nPrevNode: (key: %d,%p,  left: %p, right: %p)\n", prevNode->key, prevNode, prevNode->left, prevNode->right);
//    printf("rightChild: (key: %d,%p, left: %p, right: %p)\n", rightChild->key, rightChild, rightChild->left, rightChild->right);
//    printf("PrevNodeParent: (key: %d,%p, left: %p, right: %p)\n", prevNodeParent->key, prevNodeParent, prevNodeParent->left, prevNodeParent->right);
//    
    prevNode->right = rightChild->left;
    if (prevNode->right != NULL) {
        prevNode->right->parent = prevNode;
    }
    rightChild->left = prevNode;
    prevNode->parent = rightChild;
    prevNode->balance = 0;
    if (prevNodeParent == NULL) {
        root = rightChild;
    }
    else if (prevNodeParent->left == prevNode) {
        prevNodeParent->left = rightChild;
    }
    else  {
        prevNodeParent->right = rightChild;
    }
    rightChild->parent = prevNodeParent;
    rightChild->balance = 0;
    
//    printf("\nPrevNode: (key: %d,%p,  left: %p, right: %p)\n", prevNode->key, prevNode, prevNode->left, prevNode->right);
//    printf("rightChild: (key: %d,%p, left: %p, right: %p)\n", rightChild->key, rightChild, rightChild->left, rightChild->right);
//    printf("PrevNodeParent: (key: %d,%p, left: %p, right: %p)\n", prevNodeParent->key, prevNodeParent, prevNodeParent->left, prevNodeParent->right);
}

void AVLTree::rotateDoubleRight(Node * prevNode) {
    Node * prevNodeParent = prevNode->parent;
    Node * leftChild = prevNode->left;
    Node * leftChildRightChild = leftChild->right;
    
    leftChild->right = leftChildRightChild->left;
    if (leftChildRightChild->left != NULL) {
        leftChildRightChild->left->parent = leftChild;
    }
    
    leftChildRightChild->left = leftChild;
    leftChild->parent = leftChildRightChild;
    
    prevNode->left = leftChildRightChild->right;
    if (leftChildRightChild->right != NULL) {
        leftChildRightChild->right->parent = prevNode;
    }
    
    leftChildRightChild->right = prevNode;
    prevNode->parent = leftChildRightChild;
    
    prevNode->balance = (leftChildRightChild->balance == -1) ? +1 : 0;
    leftChild->balance = (leftChildRightChild->balance == 1) ? -1 : 0;
    
    if (prevNodeParent == NULL) {
        root = leftChildRightChild;
    }
    else if (prevNodeParent->left == prevNode) {
        prevNodeParent->left = leftChildRightChild;
    }
    else {
        prevNodeParent->right = leftChildRightChild;
    }
    
    leftChildRightChild->parent = prevNodeParent;
    leftChildRightChild->balance = 0;   
}

void AVLTree::rotateDoubleLeft(Node* prevNode) {
    Node * prevNodeParent = prevNode->parent;
    Node * rightChild = prevNode->right;
    Node * rightChildLeftChild = rightChild->left;
    
    rightChild->left = rightChildLeftChild->right;
    if (rightChildLeftChild->right != NULL) {
        rightChildLeftChild->right->parent = rightChild;
    }
    
    rightChildLeftChild->right = rightChild;
    rightChild->parent = rightChildLeftChild;
    
    prevNode->right = rightChildLeftChild->left;
    if (rightChildLeftChild->left != NULL) {
        rightChildLeftChild->left->parent = prevNode;
    }
    
    rightChildLeftChild->left = prevNode;
    prevNode->parent = rightChildLeftChild;
    
    prevNode->balance = (rightChildLeftChild->balance == 1) ? -1: 0;
    rightChild->balance = (rightChildLeftChild->balance == -1) ? 1 : 0;
    if (prevNodeParent == NULL) {
        root = rightChildLeftChild;
    }
    else if (prevNodeParent->left == prevNode) {
        prevNodeParent->left = rightChildLeftChild;
    }
    else {
        prevNodeParent->right = rightChildLeftChild;
    }
    rightChildLeftChild->parent = prevNodeParent;
    rightChildLeftChild->balance = 0;
}

bool AVLTree::replaceWithRightmost(Node * toReplaceInNode) {
    Node * currentNode = toReplaceInNode->left;
    int replacePos = 0;
    while (currentNode->right != NULL) {
        replacePos = replacePos + 1;
        currentNode = currentNode->right;
    }
    toReplaceInNode->key = currentNode->key;
    if (currentNode->parent->right == currentNode) {
        currentNode->parent->right = currentNode->left;
    }
    else {
        currentNode->parent->left = currentNode->left;
    }
    
    if (currentNode->left != NULL) {
        currentNode->left->parent = currentNode->parent;
    }
    
    bool continueBalance = true;
    currentNode = currentNode->parent;
    while (replacePos > 0 && continueBalance) {
        Node * operateOn = currentNode;
        currentNode = currentNode->parent;
        replacePos = replacePos - 1;
        continueBalance = deleteBalanceRight(operateOn);
    }
    return continueBalance;
}

bool AVLTree::deleteBalanceLeft(Node * currentNode) {
    bool continueBalance = true;
    if (currentNode->balance == -1) {
        currentNode->balance = 0;
    }
    else if (currentNode->balance == 0) {
        currentNode->balance = 1;
        continueBalance = false;
    }
    else {
        Node * currentNodeParent = currentNode->parent;
        Node * rightChild = currentNode->right;
        int rightChildBalance = rightChild->balance;
        if (rightChildBalance >= 0) {
            rotateRight(currentNode);
            if (rightChildBalance == 0) {
                currentNode->balance = 1;
                rightChild->balance = -1;
                continueBalance = false;
            }
        }
        else {
            Node * rightChildLeftChild = rightChild->left;
            int rightChildLeftChildBalance = rightChildLeftChild->balance;
            rightChild->left = rightChildLeftChild->right;
            if (rightChildLeftChild->right != NULL) {
                rightChildLeftChild->right->parent = rightChild;
            }
            rightChildLeftChild->right = rightChild;
            rightChild->parent = rightChildLeftChild;
            currentNode->right = rightChildLeftChild->left;
            if (rightChildLeftChild->left != NULL) {
                rightChildLeftChild->left->parent = currentNode;
            }
            rightChildLeftChild->left = currentNode;
            currentNode->parent = rightChildLeftChild;
            currentNode->balance = (rightChildLeftChildBalance == 1) ? -1 : 0;
            rightChild->balance = (rightChildLeftChildBalance == -1) ? 1 : 0;
            rightChildLeftChild->balance = 0;
            if (currentNodeParent == NULL) {
                root = rightChildLeftChild;
            }
            else if (currentNodeParent->left == currentNode) {
                currentNodeParent->left = rightChildLeftChild;
            }
            else {
                currentNodeParent->right = rightChildLeftChild;
            }
            rightChildLeftChild->parent = currentNodeParent;
        }
    }
    return continueBalance;
}

bool AVLTree::deleteBalanceRight(Node * currentNode) {
    bool continueBalance = true;
    if (currentNode->balance == 1) {
        currentNode->balance = 0;
    }
    else if (currentNode->balance == 0) {
        currentNode->balance = -1;
        continueBalance = false;
    }
    else {
        Node * currentNodeParent = currentNode->parent;
        Node * leftChild = currentNode->left;
        int leftChildBalance = leftChild->balance;
        if (leftChildBalance <= 0) {
            rotateLeft(currentNode);
            if (leftChildBalance == 0) {
                currentNode->balance = -1;
                leftChild->balance = 1;
                continueBalance = false;
            }
        }
        else {
            Node * leftChildRightChild = leftChild->right;
            int leftChildRightChildBalance = leftChildRightChild->balance;
            leftChild->right = leftChildRightChild->left;
            if (leftChildRightChild->left != NULL) {
                leftChildRightChild->left->parent = leftChild;
            }
            leftChildRightChild->left = leftChild;
            leftChild->parent = leftChildRightChild;
            currentNode->left = leftChildRightChild->right;
            if (leftChildRightChild->right != NULL) {
                leftChildRightChild->right->parent = currentNode;
            }
            leftChildRightChild->right = currentNode;
            currentNode->parent = leftChildRightChild;
            currentNode->balance = (leftChildRightChildBalance == -1) ? 1 : 0;
            leftChild->balance = (leftChildRightChildBalance == 1) ? -1 : 0;
            leftChildRightChild->balance = 0;
            if (currentNodeParent == NULL) {
                root = leftChildRightChild;
            }
            else if (currentNodeParent->left == currentNode) {
                currentNodeParent->left = leftChildRightChild;
            }
            else {
                currentNodeParent->right = leftChildRightChild;
            }
            leftChildRightChild->parent = currentNodeParent;
        }
    }
    return continueBalance;
}


AVLTree::AVLTree() {
    root = NULL;
    size = 0;
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



bool AVLTree::contains(const int & key) {
    return getAVLNode(key) != NULL;
}

/* Semantics: Only insert if key is absent */
bool AVLTree::insert(const int & key) {
    
    Node * prevNode = NULL;
    Node * currentNode = root;
    bool dirLeft = true;
    

    while (currentNode != NULL) {
        int nodeKey = currentNode->key;
        if ( key < nodeKey ) {
            dirLeft = true;
            prevNode = currentNode;
            currentNode = currentNode->left;
        }
        else if (key > nodeKey ) {
            dirLeft = false;
            prevNode = currentNode;
            currentNode = currentNode->right;
        }
        else {
            /* key is found, return false */
            return false;
        }
    }
    
    /* Unique key, create new node and insert */
    
    size = size + 1;
    currentNode = new Node(key, prevNode, NULL, NULL);

    if (prevNode == NULL) {
        root = currentNode;
    }
    else if (dirLeft) {
        prevNode->left = currentNode;
    }
    else {
        prevNode->right = currentNode;
    }
    
    /* Rotate and/or update balance of nodes */
    while (prevNode != NULL) {
        if (prevNode->left == currentNode) {
            if (prevNode->balance == -1) {
                Node * leftChild = prevNode->left;
                if (leftChild->balance == -1) {
                    rotateLeft(prevNode);
                }
                else {
                    rotateDoubleRight(prevNode);
                }
                break;
            }
            else if (prevNode->balance == 0) {
                prevNode->balance = -1;
            }
            else {
                prevNode->balance = 0;
                break;
            }
        }
        else {
            if (prevNode->balance == 1) {
                Node * rightChild = prevNode->right;
                if (rightChild->balance == 1) {
//                    printBFSOrder();
//                    cout << "prevNode key: " << prevNode->key << endl;
//                    cout << "currentNode key: " << currentNode->key << endl;
//                    cout << "rotating right" << endl;
                    rotateRight(prevNode);
                   // printBFSOrder();
                }
                else {
                    rotateDoubleLeft(prevNode);
                }
                break;
            }
            else if (prevNode->balance == 0) {
                prevNode->balance = 1;
            }
            else {
                prevNode->balance = 0;
                break;
            }
        }
        currentNode = prevNode;
        prevNode = prevNode->parent;
    }
    
//    cout << "Inserted key " << key << endl; 
//    printBFSOrder();
    return true;
}

bool AVLTree::erase(const int & key) {
    
    bool dirLeft = true;
    Node * currentNode = root;
    while (currentNode != NULL) {
        int nodeKey = currentNode->key;
        if (key < nodeKey) {
            dirLeft = true;
            currentNode = currentNode->left;
        }
        else if (key > nodeKey) {
            dirLeft = false;
            currentNode = currentNode->right;
        }
        else {
            size = size - 1;
            break;
        }         
    }
    
    if (currentNode == NULL) {
        return false; //key not found, return false
    }
    /* else, found key, delete it */
    Node * prevNode = currentNode->parent;
    bool continueFix = true;
    if (currentNode->left == NULL) {
        if (prevNode == NULL) {
            root = currentNode->right;
        }
        else if (dirLeft) {
            prevNode->left = currentNode->right;
        }
        else {
            prevNode->right = currentNode->right;
        }
        
        if (currentNode->right != NULL) {
            currentNode->right->parent = prevNode;
        }
        currentNode = currentNode->right;
    }
    else if (currentNode->right == NULL) {
        if (prevNode == NULL) {
            root = currentNode->left;
        }
        else if (dirLeft) {
            prevNode->left = currentNode->left;
        }
        else {
            prevNode->right = currentNode->left;
        }
        
        if (currentNode->left != NULL) {
            currentNode->left->parent = prevNode;
        }
        currentNode = currentNode->left;
    }
    else {
        if (prevNode == NULL) {
            continueFix = replaceWithRightmost(currentNode);
            currentNode = root->left;
            prevNode = root;
        }
        else if (prevNode->left == currentNode) {
            continueFix = replaceWithRightmost(currentNode);
            prevNode = prevNode->left;
            currentNode = prevNode->left;
            dirLeft = true;
        }
        else {
            continueFix = replaceWithRightmost(currentNode);
            prevNode = prevNode->right;
            currentNode = prevNode->left;
            dirLeft = true;
        }
    }

    
    /* Rebalance */
    while (continueFix && prevNode != NULL) {
        Node * nextPrevNode = prevNode->parent;
        if (nextPrevNode != NULL) {
            bool findCurrentLeftDir = true;
            if (nextPrevNode->left == prevNode) {
                findCurrentLeftDir = true;
            }
            else {
                findCurrentLeftDir = false;
            }
            
            if (currentNode == NULL) {
                if (dirLeft) 
                    continueFix = deleteBalanceLeft(prevNode);
                else 
                    continueFix = deleteBalanceRight(prevNode);
            }
            else {
                if (prevNode->left == currentNode)
                    continueFix = deleteBalanceLeft(prevNode);
                else
                    continueFix = deleteBalanceRight(prevNode);
            }
            
            if (findCurrentLeftDir)
                currentNode = nextPrevNode->left;
            else
                currentNode = nextPrevNode->right;
            prevNode = nextPrevNode;
        }
        else {
            if (currentNode == NULL) {
                if (dirLeft)
                    continueFix = deleteBalanceLeft(prevNode);
                else
                    continueFix = deleteBalanceRight(prevNode);
            }
            else {
                if (prevNode->left == currentNode) {
                    //cout << "attempting balance" << endl;
                    continueFix = deleteBalanceLeft(prevNode);
                }
                else
                    continueFix = deleteBalanceRight(prevNode);
            }
            prevNode = NULL;
        }
    }
    
    return true;
}


AVLTree * AVLTree::join(AVLTree * leftTree, AVLTree * rightTree) {
    /* Assumption: rightTree's smallest key > this tree's largest key */
    AVLTree * newTree = new AVLTree();
    Node * prevNode = NULL;
    Node * currentNode = NULL;
    
    /* Check if left tree has keys */
    if(leftTree->root == NULL){
        newTree->root = rightTree->root;
        newTree->setSize( leftTree->getSize() + rightTree->getSize());
        return newTree;
    }
    
    /* Check if right tree has keys */
    if(rightTree->root == NULL){
        newTree->root = leftTree->root;
        newTree->setSize( leftTree->getSize() + rightTree->getSize());
        return newTree;
    }
    
    /* Both trees are non-empty, non-trivial join */
    int leftHeight = leftTree->computeHeight();
    int rightHeight = rightTree->computeHeight();
    
    if ( leftHeight >= rightHeight ) {
        int minKey = rightTree->minKey();
        rightTree->erase(minKey);
        rightTree->setSize( rightTree->getSize() + 1);
        Node * newRoot = new Node(minKey, NULL, NULL, NULL);
        int newRightHeight = rightTree->computeHeight();
        
        Node * prevNode = NULL;
        Node * currentNode = leftTree->root;
        int currHeight = leftHeight;
        while (currHeight > newRightHeight + 1) {
            if (currentNode->balance == -1) {
                currHeight = currHeight - 2;
            }
            else {
                currHeight = currHeight - 1;
            }
            prevNode = currentNode;
            currentNode = currentNode->right;
        }
        Node * oldCurrentNodeParent = prevNode;
        newRoot->left = currentNode;
        if ( currentNode != NULL ) {
            currentNode->parent = newRoot;
        }
        newRoot->right = rightTree->root;
        if (rightTree->root != NULL) {
            rightTree->root->parent = newRoot;
        }
        
        newRoot->balance = newRightHeight - currHeight;
        
        if ( oldCurrentNodeParent == NULL ) {
            newTree->root = newRoot;
        }
        else if (oldCurrentNodeParent->left == currentNode) {
            prevNode->left = newRoot;
            newRoot->parent = oldCurrentNodeParent;
            newTree->root = leftTree->root;
        }
        else {
            oldCurrentNodeParent->right = newRoot;
            newRoot->parent = oldCurrentNodeParent;
            newTree->root = leftTree->root;
        }
        currentNode = newRoot;
    }
    else { /* symmetric case */
        int maxKey = leftTree->maxKey();
        leftTree->erase(maxKey);
        leftTree->setSize(leftTree->getSize() + 1);
        Node * newRoot = new Node(maxKey, NULL, NULL, NULL);
        int newLeftHeight = leftTree->computeHeight();
        
        prevNode = NULL;
        currentNode = rightTree->root;
        int currHeight = rightHeight;
        while (currHeight > newLeftHeight + 1) {
            if (currentNode->balance == 1) {
                currHeight = currHeight - 2;
            }
            else {
                currHeight = currHeight - 1;
            }
            prevNode = currentNode;
            currentNode = currentNode->left;
        }
        Node * oldCurrentNodeParent = prevNode;
        newRoot->right = currentNode;
        if ( currentNode != NULL ) {
            currentNode->parent = newRoot;
        }
        
        newRoot->left = leftTree->root;
        if (leftTree->root != NULL) {
            leftTree->root->parent = newRoot;
        }   
        newRoot->balance = currHeight - newLeftHeight;
        if ( oldCurrentNodeParent == NULL ) {
            newTree->root = newRoot;
        }
        else if (oldCurrentNodeParent->left == currentNode) {
            oldCurrentNodeParent->left = newRoot;
            newRoot->parent = oldCurrentNodeParent;
            newTree->root = rightTree->root;
        }
        else {
            oldCurrentNodeParent->right = newRoot;
            newRoot->parent = oldCurrentNodeParent;
            newTree->root = rightTree->root;
        }
        currentNode = newRoot;
    }
    
    while (prevNode != NULL) {
        if (prevNode->left == currentNode) {
            if (prevNode->balance == -1) {
                Node * leftChild = prevNode->left;
                if (leftChild->balance == -1) {
                    newTree->rotateLeft(prevNode);
                }
                else {
                    newTree->rotateDoubleRight(prevNode);
                }
                newTree->setSize( leftTree->getSize() + rightTree->getSize());
                return newTree;
            }
            else if (prevNode->balance == 0) {
                prevNode->balance = -1;
            }
            else {
                prevNode->balance = 0;
                break;
            }
        }
        else {
            if (prevNode->balance == 1) {
                Node * rightChild = prevNode->right;
                if (rightChild->balance == 1) {
                    newTree->rotateRight(prevNode);
                }
                else {
                    newTree->rotateDoubleLeft(prevNode);
                }
                newTree->setSize(leftTree->getSize() + rightTree->getSize());
                return newTree;
            }
            else if (prevNode->balance == 0) {
                prevNode->balance = 1;
            }
            else {
                prevNode->balance = 0;
                break;
            }
        }
        currentNode = prevNode;
        prevNode = prevNode->parent;
    }
    newTree->setSize( leftTree->getSize() + rightTree->getSize());
    return newTree;
}

std::tuple<int, AVLTree *, AVLTree *> AVLTree::split(AVLTree * tree) {
    Node * leftRoot = NULL;
    Node * rightRoot = NULL;
    
    int splitKey;
    if (tree->root == NULL) {
        /* empty tree */
        return std::make_tuple(INVALID_KEY, nullptr, nullptr);
    }
    else if (tree->root->left == NULL && tree->root->right == nullptr) {
        /* Tree only has one node */
        return std::make_tuple(INVALID_KEY, nullptr, nullptr);
    }
    else if (tree->root->left == NULL) {
        /* root's left child is NULL, right child is non-NULL */
        splitKey = tree->root->right->key;
        rightRoot = tree->root->right;
        rightRoot->parent = NULL;
        tree->root->right = NULL;
        leftRoot = tree->root;
    }
    else {
        /* root's left child is non-NULL */
        splitKey = tree->root->key;
        leftRoot = tree->root->left;
        leftRoot->parent = NULL;
        tree->root->left = NULL;
       
        if (tree->root->right == NULL) {
            /* root does not have a right child */
            rightRoot = tree->root;
        }
        else {
            /* re-insert splitKey into left tree */
            Node * oldRoot = tree->root;
            tree->root = tree->root->right;
            tree->root->parent = NULL;   
            delete(oldRoot);
            tree->insert(splitKey);
            rightRoot = tree->root;
            
        }
    }
    AVLTree * leftTree = new AVLTree();
    leftTree->root = leftRoot;
    AVLTree * rightTree = new AVLTree();
    rightTree->root = rightRoot;
     
    /* TODO Update heights? */
    return std::make_tuple(splitKey, leftTree, rightTree);

}

int AVLTree::getSize() {
    return size;
}

void AVLTree::setSize(int size) {
    this->size = size;
}

bool AVLTree::isEmpty() {
    return root == NULL;
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
    if ( node != NULL ) {
        if (node->left != NULL)
            holds *= doesAVLHold(node->left);
        if (node->right != NULL)
            holds *= doesAVLHold(node->right);    

        holds *= abs(node->balance) <= 1 ? true : false;
    }
   
    return holds;  
}

bool AVLTree::checkAVL( ) {
    return doesAVLHold(root);
}

void AVLTree::printBFSOrder(Node * node) {
    if (node != NULL) {
        queue<Node *> q;;
        q.push(node);
        printf("start-");
        while ( !q.empty() ) {
            Node * curr = q.front();
            q.pop();
            int parentKey = curr->parent == NULL? 0 : curr->parent->key;
            printf("%d(p(%d)->", curr->key, parentKey);
            if (curr->left != NULL)
                q.push(curr->left);
            if (curr->right != NULL)
                q.push(curr->right);
        }
        printf("end\n"); 
    }
}

void AVLTree::printBFSOrder() {
    printBFSOrder(root);
}

#endif /* AVL_TREE_H */

