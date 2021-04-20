/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_linkedlist.cpp
 * Author: ginns
 *
 * Created on April 19, 2021, 10:25 PM
 */

#include <cstdlib>
#include <iostream>
#include <chrono>
#include <set>
#include <thread>         // std::this_thread::sleep_for
#include <string>
#include <cstring>
#include "../src/data_structures/linkedlist.h"
#include "../src/util.h"

#define RANDOM_SEED         28

using namespace std;

bool list_insert(LinkedList * list, const int & key) {
    cout << "Inserting " << key << "...";
    bool op = list->insert(key);
    cout << boolalpha << op << endl;
    return op;
}

bool list_delete(LinkedList * list, const int & key) {
    cout << "Deleting " << key << "...";
    bool op = list->erase(key);
    cout << boolalpha << op << endl;
    return op;
}

void simple_test() {
    LinkedList * list = new LinkedList();
    
    assert( list_insert(list, 5) ) ;
    assert( list_insert(list, 4) );
    assert( list_insert(list, 6) );
    assert( list_insert(list, 1) );
    assert( list_insert(list, 3) );
    assert( list_insert(list, 2) );
    list->printKeys();
    cout << "Sorted? " << boolalpha << list->checkSortedOrder() << endl;
    
    assert( !list_insert(list, 2) );
    list->printKeys();
    
    assert( list_delete(list, 3) );
    list->printKeys();
    cout << "Sorted? " << boolalpha << list->checkSortedOrder() << endl;
    
    assert( !list_delete(list, 3) );
    cout << "Sorted? " << boolalpha << list->checkSortedOrder() << endl;
    
    assert( list_delete(list, 2));
    list->printKeys();
    cout << "Sorted? " << boolalpha << list->checkSortedOrder() << endl;
    
}

void timed_test(int millis) {
    
    cout << "Timed test duration: " << millis << " ms." << endl;
    set<int> numbers;
    LinkedList * list = new LinkedList();
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
            list_insert(list, num);
            numbers.insert(num);
            checksum += num;
        }
        assert( list->checkSortedOrder() );
    }
    
    cout << "Prefill complete." << endl;
    list->printKeys();
    
    /* TEST */
    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    uint64_t numOps = 0;
    while ( ( (int) chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count()) < millis ) {
        int num = rng->nextNatural() % KEYRANGE;
        if ( rng->nextNatural() % 2 == 0) {
            bool op = list->insert(num);
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
            
            
            bool op = list->erase(num);
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
        assert(list->checkSortedOrder());
            
    }
    
    cout << "completed operations" << endl;
    
    int listSum = list->sumOfKeys();
    string status = listSum == checksum ? ".OK." : ".ERROR.";
    
    cout << "List Sum: " << listSum << ". Checksum: " << checksum << status << endl;
    cout << "Number of operations: " << numOps << endl;
    cout << "Insert operations: " << insertOps << endl;
    cout << "Delete operations: " << deleteOps << endl;
    cout << "Timed test complete." << endl;
    
}

void split_join_test() {
    int MIDPOINT = 10;
    LinkedList * leftList = new LinkedList();
    LinkedList * rightList = new LinkedList();
    for (int i = 1; i <= MIDPOINT; i++)
        leftList->insert(i);
    
    cout << "[Left List]";
    leftList->printKeys();
    
    for (int i = MIDPOINT + 1; i <= MIDPOINT + 3 ; i++) 
       rightList->insert(i);
    
    cout << "[Right List]";
    rightList->printKeys();
    
    int leftListSum = leftList->sumOfKeys();
    int rightListSum = rightList->sumOfKeys();
    
    cout << "JOIN List\n==========" << endl;
    LinkedList * joinedList = LinkedList::join(leftList, rightList);
    assert(joinedList->checkSortedOrder());
    joinedList->printKeys();
    
    int joinListSum = joinedList->sumOfKeys();
    assert(joinListSum == (leftListSum + rightListSum));
    
    cout << "SPLIT List\n==========" << endl;
    std::tuple<int, LinkedList *, LinkedList *> tuple = LinkedList::split(joinedList);
   
    int splitKey = std::get<0>(tuple);
    LinkedList * newLeftList = std::get<1>(tuple);
    LinkedList * newRightList = std::get<2>(tuple);
    cout << "Split Key: " << splitKey << endl;
    
    cout << "[Left List]";
    newLeftList->printKeys();
    cout << "[Right List]";
    newRightList->printKeys();
    
    cout << "Empty list join" << endl;
    LinkedList * emptyList = new LinkedList();
    std::tuple<int, LinkedList *, LinkedList *> nullTuple = LinkedList::split(emptyList);
    int invalidKey = std::get<0>(nullTuple);
    LinkedList * nullLeft = std::get<1>(nullTuple);
    LinkedList * nullRight = std::get<2>(nullTuple);
    assert(invalidKey == -1);
    assert(nullLeft == nullptr);
    assert(nullRight == nullptr);
    
    LinkedList * joinedWithEmpty = LinkedList::join(emptyList, newRightList);
    joinedWithEmpty->printKeys();
    assert(joinedWithEmpty->checkSortedOrder());
    
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
    //split_join_test( );
    
    return 0;
}
