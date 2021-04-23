/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_redblack.cpp
 * Author: ginns
 *
 * Created on April 22, 2021, 11:34 AM
 */

#include <cstdlib>
#include <iostream>
#include <chrono>
#include <set>
#include <thread>         // std::this_thread::sleep_for
#include <string>
#include <cstring>
#include "../src/data_structures/redblack_tree.h"
#include "../src/util.h"

#define RANDOM_SEED         28

using namespace std;


bool tree_insert(RedBlackTree * tree, const int & key) {
    cout << "Inserting " << key << "...";
    bool op = tree->insert(key);
    cout << boolalpha << op << endl;
    return op;
}

bool tree_delete(RedBlackTree * tree, const int & key) {
    cout << "Deleting " << key << "...";
    bool op = tree->erase(key);
    cout << boolalpha << op << endl;
    return op;
}

void simple_test() {
    RedBlackTree * tree = new RedBlackTree();
    cout << "Black Height (start): " << tree->verifyBlackHeight() << endl;
    
    
    assert( tree_insert(tree, 5) ) ;
    cout << "Black Height (start): " << tree->verifyBlackHeight() << endl;
    assert( tree_insert(tree, 4) );
    cout << "Black Height (start): " << tree->verifyBlackHeight() << endl;
    assert( tree_insert(tree, 6) );
    cout << "Black Height (start): " << tree->verifyBlackHeight() << endl;
    assert( tree_insert(tree, 1) );
    cout << "Black Height (start): " << tree->verifyBlackHeight() << endl;
    assert( tree_insert(tree, 3) );
    cout << "Black Height (start): " << tree->verifyBlackHeight() << endl;
    assert( tree_insert(tree, 2) );
    
    tree->printBFSOrder();
    cout << "Black Height: " << tree->verifyBlackHeight() << endl;
    
    assert( !tree_insert(tree, 2) );
    tree->printBFSOrder();
    cout << "Black Height: " << tree->verifyBlackHeight() << endl;
    
    assert( tree_delete(tree, 3) );
    tree->printBFSOrder();
    cout << "Black Height: " << tree->verifyBlackHeight() << endl;
    
    
    assert( !tree_delete(tree, 3) );
    cout << "Black Height: " << tree->verifyBlackHeight() << endl;
    
    assert( tree_delete(tree, 2));
    tree->printBFSOrder();
    cout << "Black Height: " << tree->verifyBlackHeight() << endl;
    
}

void timed_test(int millis) {
    
    cout << "Timed test duration: " << millis << " ms." << endl;
    set<int> numbers;
    RedBlackTree * tree = new RedBlackTree();
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
            tree->insert(num);
            numbers.insert(num);
            checksum += num;
        }
        assert( tree->verifyBlackHeight() != 0 );
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
        assert(tree->verifyBlackHeight() != 0);
            
    }
    
    cout << "completed operations" << endl;
    
    int rbSum = tree->sumOfKeys();
    string status = rbSum == checksum ? ".OK." : ".ERROR.";
    
    cout << "RedBlack Tree Sum: " << rbSum << ". Checksum: " << checksum << status << endl;
    cout << "BlackHeight: " << tree->verifyBlackHeight() << endl;
    cout << "Number of operations: " << numOps << endl;
    cout << "Insert operations: " << insertOps << endl;
    cout << "Delete operations: " << deleteOps << endl;
    cout << "Timed test complete." << endl;
    
}


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
    //split_join_test( );
   
    return 0;
}
