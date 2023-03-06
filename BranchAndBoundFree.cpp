#include "Tableau.hpp"
#include "Stack.hpp"
#include "PriorityQueue.hpp"

/*=============================================================================
branch_and_bound - performs DFS traversal of tree where at each visited node:
                    1. Simplex is run to find new bound constraints
                    2. child nodes are inserted into the tree and pushed with
                        corresponding subproblems with additional constraints
=============================================================================*/
OptimalSolution* branch_and_bound(Tableau* root_tab){

  Node root={'R', NULL, root_tab};
  PriorityQueue best_sols;
  Stack next_up;
  next_up.push(&root);

  int branch_var;
  Node* curr;

  while(!next_up.is_empty()){

    //examine next problem
    curr=next_up.top();
    next_up.pop();

    //solve simplex
    curr->problem->simplex();

    //determine if branching is needed and what the variable would be
    branch_var=find_float(curr->problem->get_sol()->args,curr->problem->get_vars());

    //If a candidate solution has been found, enqueue into the PriorityQueue based on objective function value
    if (branch_var==-1){
      curr->weight=curr->problem->get_sol()->eval;
      best_sols.insert(curr);
    }
    //If there are potentially still candidate solutions down this path
    if (curr->problem->get_feasibility() && branch_var != -1){

      // branch down...
      Node* right_child = new Node;
      right_child->problem = new Tableau(*curr->problem, branch_var,std::floor(curr->problem->get_sol()->args[branch_var]), false);

      // and up on nearest integer bounds of floating point value
      Node* left_child = new Node;
      left_child->problem = new Tableau(*curr->problem, branch_var, std::ceil(curr->problem->get_sol()->args[branch_var]), true);

      // And push them onto the stack
      next_up.push(right_child);
      next_up.push(left_child);


    }
  }
    //If there are no integer-feasible solutions to the problem, return NULL for UI-level handling
    if (best_sols.is_empty()) return NULL;


    return best_sols.front()->problem->get_sol();
}



  /*========================================
  Driver with example Pure Integer Program
  ==========================================*/
int main(int argc, char const *argv[]) {


  //============Insert IP Here================
  //******************************************

          /*Max:*/ float obj[]={8,5};

          /* s.t.*/ float c1[]={1,1,6};
                    float c2[]={9,5,45};
                    float* cons[]={c1,c2};

  //******************************************
  //=========================================

  //Initializing base objects
  Tableau tab(obj,2,cons,2);

  // Run branch_and_bound function and print solution;
  OptimalSolution* opt=branch_and_bound(&tab);
  print_solution(opt,2);

  return 0;
}
