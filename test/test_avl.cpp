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
#include <chrono>
#include <set>
#include <thread>         // std::this_thread::sleep_for
#include "../src/trees/avl_tree.h"
#include "../src/util.h"

using namespace std;

bool tree_insert(AVLTree * tree, const int & key) {
    cout << "Inserting " << key << "..." << endl;
    return tree->insert(key);
}

bool tree_delete(AVLTree * tree, const int & key) {
    cout << "Deleting " << key << "..." << endl;
    return tree->erase(key);
}

void simple_test() {
    AVLTree * tree = new AVLTree();
    
    assert( tree_insert(tree, 4) ) ;
    assert( tree_insert(tree, 5) );
    assert( tree_insert(tree, 6) );
    assert( tree_insert(tree, 1) );
    assert( tree_insert(tree, 3) );
    assert( tree_insert(tree, 2) );
    tree->printInOrderTraversal();
    tree->printBFSOrder();
    cout << "AVL Property? " << boolalpha << tree->checkAVL() << endl;
    
    assert( !tree_insert(tree, 2) );
    tree->printInOrderTraversal();
    tree->printBFSOrder();
    
    assert( tree_delete(tree, 3) );
    tree->printInOrderTraversal();
    tree->printBFSOrder();
    cout << "AVL Property? " << boolalpha << tree->checkAVL() << endl;
    
    assert( !tree_delete(tree, 3) );
    cout << "AVL Property? " << boolalpha << tree->checkAVL() << endl;
    
    assert( tree_delete(tree, 2));
    tree->printInOrderTraversal();
    tree->printBFSOrder();
    
    cout << "AVL Property? " << boolalpha << tree->checkAVL() << endl;
    
}

void timed_test(int millis) {
    set<int> numbers;
    AVLTree * tree = new AVLTree();
    PaddedRandom * rng = new PaddedRandom();
    
    
    const int KEYRANGE = 10000;
    
    /* PREFILL */
    for (int i = 0; i < KEYRANGE / 2; i++) {
        int num = rng->nextNatural() % 10000;
        tree->insert(num);
        numbers.insert(num);
        assert( tree->checkAVL() );
    }
    
    
    /* TEST */
    
    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    uint64_t numOps = 0;
    while ( ( (int) chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count()) < millis ) {
        int num = rng->nextNatural() % KEYRANGE;
        if (rng->nextNatural() % 2 == 0) {
            bool op = tree->insert(num);
            bool expected;
            if ( numbers.count(num) == 0 )
                expected = true; //num is currently not in numbers
            else
                expected = false;
            assert(op == expected);
            if (op == true)
                numbers.insert(num);
            
        }
        else {
            bool op = tree->erase(num);
            bool expected;
            if ( numbers.count(num) != 0 )
                expected = true;
            else
                expected = false;
            assert(op == expected);
            if (op == true)
                numbers.erase(num);
        }
        numOps++;
        assert(tree->checkAVL());
            
    }
    cout << "Number of operations: " << numOps << endl;
    cout << "Timed test complete." << endl;
    
}

/*
 * 
 */
int main(int argc, char** argv) {
    
    simple_test( );
    timed_test( 30000 );
    
    return 0;
}

