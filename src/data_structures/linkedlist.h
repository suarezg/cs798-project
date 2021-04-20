/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   linkedlist.h
 * Author: ginns
 *
 * Created on April 19, 2021, 9:05 PM
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <cassert>
#include <iostream>

#define INVALID_KEY     -1


using namespace std;
class LinkedList {
private:
    struct Node {
        int key;
        Node * next;
        
        Node(int _key, Node * _next) : key(_key), next(_next) {
            
        }
    };
    
    Node * head;
    Node * tail;
    int size;
    
public:
    
    LinkedList();
    ~LinkedList();
     
    /* Dictionary operations  */
    bool contains(const int & key);
    bool insert(const int & key);
    bool erase(const int & key);
    
    /* */
    static LinkedList * join(LinkedList * leftList, LinkedList * rightList);
    static std::tuple<int, LinkedList *, LinkedList *> split(LinkedList * list);
    
    int getSize();
    bool isEmpty();
    int sumOfKeys();
    void printKeys();
    bool checkSortedOrder();
   
};

LinkedList::LinkedList() {
    size = 0;
    head = NULL;
    tail = NULL;
}

LinkedList::~LinkedList() {
    
    Node * prev = NULL;
    Node * curr = head;;
    
    while (curr != NULL) {
        prev = curr;
        curr = curr->next;
        delete(prev);
    }
    
    size = 0;
}

bool LinkedList::contains(const int & key) {
    Node * curr = head;
    while ( (curr != NULL) && (curr->key < key) ) {
        curr = curr->next;
    }
    
    if (curr != NULL && curr->key == key)
        return true;
    else
        return false;
}

bool LinkedList::insert(const int & key) {
    if (head == NULL) {
        /* empty list */
        Node * newNode = new Node(key, NULL);
        head = newNode;
        tail = newNode;
    }
    else {
        Node * prev = NULL;
        Node * curr = head;
        while ( (curr != NULL) && (curr->key < key) ) {
            prev = curr;
            curr = curr->next;
        }
        
        if (curr != NULL && curr->key == key)
            return false; // key already present
        
        /* Insert key into list */
        size++;
        Node * newNode = new Node(key, curr);
        
        if (prev == NULL) {
            assert(key < curr->key);
            head = newNode;
        }
        else {
            prev->next = newNode;
            if (prev == tail)
                tail = newNode;
        }
    }
    
    return true;
}

bool LinkedList::erase(const int& key) {
    if (head == NULL) {
        return false; /* empty list */
    }
    else {
        Node * prev = NULL;
        Node * curr = head;
        while ( (curr != NULL) && (curr->key < key) ) {
            prev = curr;
            curr = curr->next;
        }
        
        if (curr == NULL || curr->key != key)
            return false; // key not in list
        
        /* Delete key from list */
        size--;
        assert(curr->key == key);
        
        if ( prev == NULL ) {
            assert(curr == head);
            if (head == tail) {
                head = NULL;
                tail = NULL;
            }
            else {
                head = curr->next;               
            }
        }
        else {
            prev->next = curr->next;
            if (curr == tail) {
                tail = prev;
            }
        }
        delete curr;
        
    }
    return true;
}

int LinkedList::getSize() {
    return size;
}

bool LinkedList::isEmpty() {
    return head == NULL;
}

int LinkedList::sumOfKeys() {
    int sum = 0;
    Node * curr = head;
    while (curr != NULL) {
        sum += curr->key;
        curr = curr->next;
    }
    return sum;
}

void LinkedList::printKeys() {
    cout << "start-";
    Node * curr = head;
    while (curr != NULL) {
        cout << curr->key << "-";
        curr = curr->next;
    }
    cout << "end" << endl;
}

bool LinkedList::checkSortedOrder() {
    bool sorted = true;
    Node * prev = NULL;
    Node * curr = head;
    while ( curr != NULL ) {
        prev = curr;
        curr = curr->next;
        if (curr != NULL)
            sorted = prev->key < curr->key; 
        if (sorted == false)
            break;
    }
    return sorted;
}

LinkedList * LinkedList::join(LinkedList * leftList, LinkedList * rightList) {
    if ( leftList->isEmpty() && rightList->isEmpty() ) {
        return NULL;
    }
    else if (leftList->isEmpty()) {
        return rightList;
    }
    else if (rightList->isEmpty()) {
        return leftList;
    }
    
    assert(leftList->tail->key < rightList->head->key);
    
    LinkedList * newList = new LinkedList();
    newList->head = leftList->head;
    newList->tail = rightList->tail;
    leftList->tail->next = rightList->head;
    newList->size = leftList->getSize() + rightList->getSize();
    return newList;
}


std::tuple<int, LinkedList *, LinkedList *> LinkedList::split(LinkedList * list) {
    
    if ( list->isEmpty() ) {
        return std::make_tuple(INVALID_KEY, nullptr, nullptr);
    }
    else if ( list->getSize() == 1 ) {
        return std::make_tuple(INVALID_KEY, nullptr, nullptr);
    }
    
    int origSize = list->getSize();
    int splitPoint = origSize / 2;
    int count = 0;
    Node * prev = NULL;
    Node * curr = list->head;
    while ( count < splitPoint ) {
        prev = curr;
        curr = curr->next;
        count++;
    }

    int splitKey = curr->key;

    LinkedList * leftList = new LinkedList();
    LinkedList * rightList = new LinkedList();

    leftList->head = list->head;
    leftList->tail = prev;
    leftList->size = splitPoint;

    rightList->head = curr;
    rightList->tail = list->tail;
    rightList->size = origSize - splitPoint;

    return std::make_tuple(splitKey, leftList, rightList); 
                  
}



#endif /* LINKEDLIST_H */
