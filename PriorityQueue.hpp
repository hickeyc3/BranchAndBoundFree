#include <iostream>
#include "Tableau.hpp"
#include "Stack.hpp"

#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP

/*====================================================
  Priority Queue - maintains descending order queue
                  implemented with singly linked list
======================================================*/
class PriorityQueue{
  private:
    Node front_sentinel, back_sentinel;

  public:
    PriorityQueue();              // Constructor
    void insert(Node*);           // insert node into the queue, maintaining order
    Node* front();                // pointer to front, highest priority, node
    void dequeue();               // remove highest priority node
    bool is_empty();              // returns if there are non-sentinel nodes
    void print();                 // print out queue in priority order
};

#endif
