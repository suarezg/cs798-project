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
    int blackHeight;
    
    assert( tree_insert(tree, 5) ) ;
    assert( tree_insert(tree, 4) );
    assert( tree_insert(tree, 6) );
    assert( tree_insert(tree, 1) );
    assert( tree_insert(tree, 3) );
    assert( tree_insert(tree, 2) );
    
    tree->printBFSOrder();
    blackHeight = tree->getBlackHeight();
    cout << "Black Height: " << tree->getBlackHeight() << endl;
    
    assert( !tree_insert(tree, 2) );
    tree->printBFSOrder();
    blackHeight = tree->getBlackHeight();
    cout << "Black Height: " << tree->getBlackHeight() << endl;
    
    assert( tree_delete(tree, 3) );
    tree->printBFSOrder();
    blackHeight = tree->getBlackHeight();
    cout << "Black Height: " << tree->getBlackHeight() << endl;
    
    
    assert( !tree_delete(tree, 3) );
    blackHeight = tree->getBlackHeight();
    cout << "Black Height: " << tree->getBlackHeight() << endl;
    
    assert( tree_delete(tree, 2));
    tree->printBFSOrder();
    blackHeight = tree->getBlackHeight();
    cout << "Black Height: " << tree->getBlackHeight() << endl;
    
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
    //timed_test( millisToRun );
    //split_join_test( );
   
    return 0;
}
