/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_avl.cpp
 * Author: ginns
 *
 * Created on March 18, 2021, 9:20 PM
 */

#include <cstdlib>
#include <iostream>
#include "../src/trees/avl_tree.h"

using namespace std;

void tree_insert(AVLTree * tree, const int & key) {
    cout << "Inserting " << key << "..." << endl;
    tree->insert(key);
}

/*
 * 
 */
int main(int argc, char** argv) {
    AVLTree * tree = new AVLTree();
    
    tree_insert(tree, 4);
    tree_insert(tree, 5);
    tree_insert(tree, 6);
    tree_insert(tree, 1);
    tree_insert(tree, 3);
    tree_insert(tree, 2);
    
    tree->printInOrderTraversal();
    tree->printBFSOrder();
    
    
    
    cout << "AVL Property? " << boolalpha << tree->checkAVL() << endl;
    
    return 0;
}

