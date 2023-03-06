#include "PriorityQueue.hpp"

/*==============================================================
 PriorityQueue Constructor - intializes sentinels that mark the
                             front and back of the queue
================================================================*/
PriorityQueue::PriorityQueue(){
  back_sentinel.next=&front_sentinel;
  front_sentinel.next=NULL;
}

/*===============================================================
 PriorityQueue::is_empty - return if there are not nodes, besides
                          sentinels, in the queue
================================================================*/
bool PriorityQueue::is_empty(){
  if (back_sentinel.next==&front_sentinel)  return true;
  return false;
}

/*===============================================================
 PriorityQueue::insert - insert node by pointer into the queue,
                          maintaing priority by the weight of
                          the node in ascending order
================================================================*/
void PriorityQueue::insert(Node* n){

  Node* curr=&back_sentinel;

  while (curr->next!=&front_sentinel){

    if (n->weight > curr->weight){

      n->next=curr->next;
      curr->next=n;

      return;
    }
    curr=curr->next;
  }

  curr->next=n;
  n->next=&front_sentinel;
  return;
}

/*===============================================================
 PriorityQueue::print - print queue, in ascending order
================================================================*/
void PriorityQueue::print(){

  Node* curr=back_sentinel.next;
  std::cout<<std::endl;

  while (curr!=&front_sentinel){

    std::cout<<curr->weight<<", ";

    curr=curr->next;
  }
  std::cout<<std::endl;
}

/*===============================================================
 PriorityQueue::front - return pointer to highest priority node
                        at the front of the queue
================================================================*/
Node* PriorityQueue::front(){

  Node* curr=&back_sentinel;

  while(curr->next!=&front_sentinel) curr=curr->next;

  return curr;
}

/*===============================================================
 PriorityQueue::dequeue - remove highest priority node at the
                            front of the queue
================================================================*/
void PriorityQueue::dequeue(){

  Node* front_node=this->front();

  Node* curr=&back_sentinel;

  while(curr->next!=front_node) curr=curr->next;

  curr->next=&front_sentinel;

}
