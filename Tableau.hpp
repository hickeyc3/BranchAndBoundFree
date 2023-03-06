#include <iostream>
#include <climits>
#include <cmath>
#include <stdexcept>

#ifndef TABLEAU_HPP
#define TABLEAU_HPP


/*=============================================================================
find_float - Helper to determine which index of the current optimal arguments
            is a non-integer in order to branch on the lower and upper integer
            values of that float. Return -1 if they're all integers.
===============================================================================*/
int find_float(double*, int);

/*=================================================
OptimalSolution - structure holding the optimal objective
                  function evaluation and it's corresponding
                  decision variable arguments.
====================================================*/
struct OptimalSolution{
  float eval=0;        // function evaluation of optimal value
  double* args=NULL;   // returns optimal value of decision variables
};

/*=====================================================================
print_solution - clean print OptimalSolution arguments and evaluation
======================================================================*/
void print_solution(OptimalSolution*, int);


/*=================================================
Tableau - Augmented matrix row-reduced until optimal
  members: m,n: dimensions of underlying array)
           var: number of decision variables being optimized
           arr: underlying array
====================================================*/
class Tableau{
  private:
    int m,n;              // Dimensions of underlying 2-D array
    int vars;             // count of decision variables
    short* signs;         // signs of constraints, either 1 for standard <= or -1 for >=
    bool feasible;        // set to true if there is a feasible solution
    bool status;          // set to true if simplex has been run and Tabluea reduced
    OptimalSolution* sol; // stores solution to LP represented by Tableau

  public:
    float** arr;    // Underlying 2-D array
    Tableau(float*, int, float**, int); // Constructor accepts coefficient arrays of objective function, constraints, and dimensions.

    /*          args:   float* obj,               // coefficients of objective function
                        int var_count,            // number of decision variables in LP
                        float** constraints,      // 2-D array of constraint arrays, each of length var_count+1
                        int constraint_count):    // number of independent constraints
    */

    Tableau(Tableau&, size_t, float, bool); // Constructor for adding new row of a single variable constraint to an existing tableau

      /*        args:   Tableau& parent_tab,      // Original Tableau we are expanding
                        size_t decision_var,      // Variable to add branch constraint
                        float new_rhs,            // RHS of new branch constraint
                        bool geq                  // If the new row is given in >= form
    */

    ~Tableau();                                // Destructor - free underlying 2-D array and signs
    void print();                              // print current Tableau for debugging
    OptimalSolution* simplex();                // Find optimal solution of corresponding linear program

    // Accessors
    int get_rows(){ return m;}                 // return count of rows
    int get_columns() { return n;}             // return count of columns
    int get_vars() { return vars;}             // return count of decision variables for this problem
    float* operator[](int);                    // access Tableau element
    short* get_signs() {return signs;}         // return list of constraint signs
    bool get_feasibility() { return feasible;} // return if the tableau has a feasible solution
    bool get_status() {return status;}         // return if the tableau is currently in reduced simplex form
    OptimalSolution* get_sol() {return sol;}   // optimal solution to LP represented by Tableau
};

#endif
