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
#include <string>
#include <cstring>
#include "../src/trees/avl_tree.h"
#include "../src/util.h"\

#define RANDOM_SEED         28

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
    //tree->printInOrderTraversal();
    tree->printBFSOrder();
    cout << "AVL Property? " << boolalpha << tree->checkAVL() << endl;
    
    assert( !tree_insert(tree, 2) );
    //tree->printInOrderTraversal();
    tree->printBFSOrder();
    
    assert( tree_delete(tree, 3) );
    //tree->printInOrderTraversal();
    tree->printBFSOrder();
    cout << "AVL Property? " << boolalpha << tree->checkAVL() << endl;
    
    assert( !tree_delete(tree, 3) );
    cout << "AVL Property? " << boolalpha << tree->checkAVL() << endl;
    
    assert( tree_delete(tree, 2));
    //tree->printInOrderTraversal();
    tree->printBFSOrder();
    
    cout << "AVL Property? " << boolalpha << tree->checkAVL() << endl;
    
}

void timed_test(int millis) {
    
    cout << "Timed test duration: " << millis << " ms." << endl;
    set<int> numbers;
    AVLTree * tree = new AVLTree();
    PaddedRandom * rng = new PaddedRandom();
    rng->setSeed(RANDOM_SEED);
    int checksum = 0;
    int insertOps = 0;
    int deleteOps = 0;
    
    const int KEYRANGE = 10000;
    
    /* PREFILL */
    for (int i = 0; i < KEYRANGE / 2; i++) {
        int num = rng->nextNatural() % 10000;
        if ( numbers.count(num) == 0 ) {
            tree->insert(num);
            numbers.insert(num);
            checksum += num;
        }
        assert( tree->checkAVL() );
    }
    
    
    /* TEST */
    
    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    uint64_t numOps = 0;
    while ( ( (int) chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count()) < millis ) {
        int num = rng->nextNatural() % KEYRANGE;
        if ( rng->nextNatural() % 2 == 0) {
            bool op = tree->insert(num);
            bool expected;
            
            /* Check if we've insert num before */
            if ( numbers.count(num) == 0 )
                expected = true; //num is currently not in numbers
            else
                expected = false;
            assert(op == expected);
            if (op == true)  {
                // inserted num into set 
                numbers.insert(num);
                checksum += num;
            }
                
            insertOps++;
            
        }
        else {
            
            
            bool op = tree->erase(num);
            bool expected;
            
            /* Check if we've insert num before */
            if ( numbers.count(num) != 0 )
                expected = true;
            else
                expected = false;
            
            assert(op == expected);
            
            if (op == true) {
                // deleted num from set
                numbers.erase(num);
                checksum -= num;
                
            }
            deleteOps++;
        }
        numOps++;
        assert(tree->checkAVL());
            
    }
    
    
    int avlSum = tree->sumOfKeys();
    string status = avlSum == checksum ? ".OK." : ".ERROR.";
    
    cout << "AVL Sum: " << avlSum << ". Checksum: " << checksum << status << endl;
    cout << "Number of operations: " << numOps << endl;
    cout << "Insert operations: " << insertOps << endl;
    cout << "Delete operations: " << deleteOps << endl;
    cout << "Timed test complete." << endl;
    
}

void split_join_test() {
    int MIDPOINT = 10;
    AVLTree * leftTree = new AVLTree();
    AVLTree * rightTree = new AVLTree();
    for (int i = 1; i <= MIDPOINT; i++)
        leftTree->insert(i);
    
    leftTree->printBFSOrder();
    
    for (int i = MIDPOINT + 1; i <= MIDPOINT + 3 ; i++) 
       rightTree->insert(i);

    rightTree->printBFSOrder();
    
    int leftTreeSum = leftTree->sumOfKeys();
    int rightTreeSum = rightTree->sumOfKeys();
    
    
    AVLTree * joinedTree = leftTree->join(rightTree);
    assert(joinedTree->checkAVL());
    joinedTree->printBFSOrder();
    
    int joinTreeSum = joinedTree->sumOfKeys();
    assert(joinTreeSum == (leftTreeSum + rightTreeSum));
    
    
}   

/*
 * 
 */
int main(int argc, char** argv) {
    
    if (argc == 1) {
        cout<<"USAGE: "<<argv[0]<<" [options]"<<endl;
        cout<<"Options:"<<endl;
        cout<<"    -t [int]     milliseconds to run"<<endl;
        cout<<endl;
        return 1;
    }
    int millisToRun = 0;
    
    // read command line args
    for (int i=1;i<argc;++i) {
        if (strcmp(argv[i], "-t") == 0) {
            millisToRun = atoi(argv[++i]);            
        } else {
            cout<<"bad arguments"<<endl;
            exit(1);
        }
    }
    
    simple_test( );
    timed_test( millisToRun );
    split_join_test( );
    
    
    
    return 0;
}

