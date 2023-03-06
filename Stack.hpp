#include <iostream>
#include "Tableau.hpp"

#ifndef STACK_HPP
#define STACK_HPP

/*=====================================================================
Node - Each tree node has an associated LP Tableau object and up to
        two children nodes, also can be inserted in PriorityQueue
=======================================================================*/
struct Node{
  char  tag;            //tag for identifying Node
  Node* next;           // pointer for implementing queue with linked list
  Tableau* problem;     // corresponding LP problem for this node
  float weight;         // weight for maintaining queue priority
  Node* l_child=NULL;   // left child pointer for tree
  Node* r_child=NULL;   // right child pointer for tree

};

/*=================================================
  Stack - stack implemented with singly linked list
==================================================*/
class Stack{
  private:
    Node top_sentinel, bottom_sentinel;

  public:
    Stack();                    // Constructor, initializes sentinels
    void push(Node*);           // Push node onto top of stack
    void pop();                 // Remove top node from stack
    Node* top();                // Return pointer to top node
    bool is_empty();            // Return if only the sentinels are in the stack
    void print();               // Print current stack order for debugging
};

#endif
