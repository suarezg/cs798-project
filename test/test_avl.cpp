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
#include "../src/data_structures/avl_tree.h"
#include "../src/util.h"

#define RANDOM_SEED         28

using namespace std;

bool tree_insert(AVLTree * tree, const int & key) {
    cout << "Inserting " << key << "...";
    bool op = tree->insert(key);
    cout << boolalpha << op << endl;
    return op;
}

bool tree_delete(AVLTree * tree, const int & key) {
    cout << "Deleting " << key << "...";
    bool op = tree->erase(key);
    cout << boolalpha << op << endl;
    return op;
}

void simple_test() {
    AVLTree * tree = new AVLTree();
    
    assert( tree_insert(tree, 5) ) ;
    assert( tree_insert(tree, 4) );
    assert( tree_insert(tree, 6) );
    assert( tree_insert(tree, 1) );
    assert( tree_insert(tree, 3) );
    assert( tree_insert(tree, 2) );
    tree->printInOrderTraversal();
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

void test_balance() {
    AVLTree * tree = new AVLTree();
    
    /* Setup tree*/
    assert( tree_insert(tree, 7) ) ;
    assert( tree_insert(tree, 2) );
    assert( tree_insert(tree, 9) );
    assert( tree_insert(tree, 4) );
//    assert( tree_insert(tree, 4) );
//    assert( tree_insert(tree, 7) );
//    assert( tree_insert(tree, 3) );
    tree->printInOrderTraversal();
    tree->printBFSOrder();
    cout << "AVL Property? " << boolalpha << tree->checkAVL() << endl;
    
    /* Delete */
//    assert( tree_delete(tree, 2) );
//    tree->printInOrderTraversal();
//    tree->printBFSOrder();
//    cout << "AVL Property? " << boolalpha << tree->checkAVL() << endl;
    
    /* Insert */
    assert( tree_insert(tree, 5) );
    tree->printInOrderTraversal();
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
    
    const int KEYRANGE = 100;
    
    /* PREFILL */
    cout << "Starting prefill..." << endl;
    for (int i = 0; i < KEYRANGE / 2; i++) {
        int num = rng->nextNatural() % KEYRANGE;
        if ( numbers.count(num) == 0 ) {
            tree_insert(tree, num);
            numbers.insert(num);
            checksum += num;
        }
        assert( tree->checkAVL() );
    }
    
    cout << "Prefill complete." << endl;
    tree->printBFSOrder();
    
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
    
    cout << "completed operations" << endl;
    
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
    
    cout << "[Left Tree]";
    leftTree->printBFSOrder();
    
    for (int i = MIDPOINT + 1; i <= MIDPOINT + 3 ; i++) 
       rightTree->insert(i);
    
    cout << "[Right Tree]";
    rightTree->printBFSOrder();
    
    int leftTreeSum = leftTree->sumOfKeys();
    int rightTreeSum = rightTree->sumOfKeys();
    
    cout << "JOIN TREES\n==========" << endl;
    IOrderedSet * joinedSet = leftTree->join(rightTree);
    AVLTree * joinedTree = static_cast<AVLTree *>(joinedSet);
    assert(joinedTree->checkAVL());
    joinedTree->printBFSOrder();
    
    int joinTreeSum = joinedTree->sumOfKeys();
    assert(joinTreeSum == (leftTreeSum + rightTreeSum));
    
    cout << "SPLIT TREES\n==========" << endl;
    std::tuple<int, IOrderedSet *, IOrderedSet *> tuple = joinedTree->split();
   
    int splitKey = std::get<0>(tuple);
    IOrderedSet * newLeftSet = std::get<1>(tuple);
    IOrderedSet * newRightSet = std::get<2>(tuple);
    AVLTree * newLeftTree = static_cast<AVLTree *>(newLeftSet);
    AVLTree * newRightTree = static_cast<AVLTree *>(newRightSet);
    
    cout << "Split Key: " << splitKey << endl;
    
    cout << "[Left Tree]";
    newLeftTree->printBFSOrder();
    cout << "[Right Tree]";
    newRightTree->printBFSOrder();
    
    cout << "Empty tree join" << endl;
    AVLTree * emptyTree = new AVLTree();
    std::tuple<int, IOrderedSet *, IOrderedSet *> nullTuple = emptyTree->split();
    int invalidKey = std::get<0>(nullTuple);
    IOrderedSet * nullLeft = std::get<1>(nullTuple);
    IOrderedSet * nullRight = std::get<2>(nullTuple);
    assert(invalidKey == -1);
    assert(nullLeft == nullptr);
    assert(nullRight == nullptr);
    
    IOrderedSet * joinedWithEmptySet = emptyTree->join(newRightTree);
    AVLTree * joinedWithEmpty = static_cast<AVLTree *>(joinedWithEmptySet);
    joinedWithEmpty->printBFSOrder();
    assert(joinedWithEmpty->checkAVL());
    
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
    test_balance( );
    timed_test( millisToRun );
    split_join_test( );
    
    
    
    return 0;
}

