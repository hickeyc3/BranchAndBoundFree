#include "Stack.hpp"

/*============================================================
 Stack Constructor - intializes sentinels that mark the top and
                      bottom of the stack
=============================================================*/
Stack::Stack(){

  //Initialize sentinels single links
  top_sentinel.next=&bottom_sentinel;
  bottom_sentinel.next=NULL;
}

/*============================================================
 Stack::push - add node by pointer onto top of stack
=============================================================*/
void Stack::push(Node* n){
  n->next=top_sentinel.next;
  top_sentinel.next=n;
}

/*============================================================
 Stack::pop- remove top node, meaning the most recently pushed node
=============================================================*/
void Stack::pop(){
  Node* curr_top=top_sentinel.next;
  top_sentinel.next=top_sentinel.next->next;
  curr_top->next=NULL;
}

/*============================================================
 Stack::top - return pointer to most recently pushed node
=============================================================*/
Node* Stack::top(){
  return top_sentinel.next;
}

/*=============================================================
 Stack::is_empty - return if there are not nodes, besides sentinels,
                  on the stack
==============================================================*/
bool Stack::is_empty(){
  return (top_sentinel.next==&bottom_sentinel);
}

/*================================================================
 Stack::print - print out current nodes in stack, in most recently
                added order descending
==================================================================*/
void Stack::print(){
  Node* curr=top_sentinel.next;
  std::cout<<std::endl;
  while(curr!=&bottom_sentinel){

    std::cout<<curr->tag<<std::endl;
    curr=curr->next;
  }
  std::cout<<std::endl;
}

/*uncomment for DFS example
int main(){

  Node a={'a'};
  Node b={'b'};
  Node c={'c'};
  Node d={'d'};
  Node e={'e'};
  Node f={'f'};
  Node g={'g'};
  Node h={'h'};
  Node i={'i'};
  Node j={'j'};
  Node k={'k'};
  Node l={'l'};

  Node* curr;
                                  // make tree 
                                      //a root
                 a.l_child=&b;                         a.r_child=&c;
        b.l_child=&d;        b.r_child=&e;      c.l_child=&f;
d.l_child=&h; d.r_child=&i;             f.l_child=&g;  f.r_child=&k;
                  i.r_child=&j;
              j.l_child=&l;

  //push root a
  Stack s;
  s.push(&a);

  while(!s.is_empty()) {
    curr=s.top();
    s.pop();

    std::cout<<curr->tag<<std::endl;

    if (curr->r_child!=NULL) s.push(curr->r_child);
    if (curr->l_child!=NULL) s.push(curr->l_child);

  }

  return 0;
}
*/
