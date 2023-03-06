#include "Tableau.hpp"

//***************************************
// Simplex implemented with Tableau class,
// accepts standardized maximization form
//***************************************

/*=============================================================================
argmin - helper function for returning most negative element index in objective
        function row
==============================================================================*/
int argmin(float* row, int length){

  float min=INT_MAX;
  int argmin;
  for (size_t i = 0; i < length; i++){
    if (row[i]<min){
      min=row[i];
      argmin=i;
    }
  }

  return argmin;
}

/*=============================================================================
find_float - Helper to determine which index of the current optimal arguments
            is a non-integer in order to branch on the lower and upper integer
            values of that float. Return -1 if they're all integers.
===============================================================================*/
int find_float(double* arr, int num_vars){

  for (size_t i = 0; i < num_vars; i++) {
    if (arr[i]-(int)arr[i] != 0) return i;
  }
  return -1;
}

/*=====================================================================
print_solution - prints formatted OptimalSolution struct information
======================================================================*/
void print_solution(OptimalSolution* sol, int num_args){

  std::cout<<std::endl<<std::endl<<"**********************************************"<<std::endl;
  std::cout<<"    Optimal Objective Function Value: "<<sol->eval<<std::endl;
  std::cout<< "    Optimal Decision Variables: ";
  for (size_t i = 0; i < num_args; i++) {
    std::cout<<"x"<<i<<" = "<<sol->args[i]<<", ";
  }



  std::cout<<std::endl;


  std::cout<<"**********************************************"<<std::endl<<std::endl;
}

/*=============================================================================
add_rows - performs basic row reduction operation of mutablerow-scale*R2
==============================================================================*/
void add_rows(float* mutable_row, const float* pivot_row, float c, int length){
  for (size_t i = 0; i < length; i++) mutable_row[i]-=c*pivot_row[i];
}

/*================================================================================
create_backup - helper to perform shallow copy of underlying 2-D array for tableau
==================================================================================*/
float** create_backup(float** arr,int m, int n){

  float** backup = new float*[m];

  for (size_t i = 0; i < m; i++) {
    backup[i]=new float[n];
    for (size_t j = 0; j < n; j++) backup[i][j]=arr[i][j];
    }

    return backup;
  }



/*================================================================
Tableau::Constructor - Creates simplex tableau for linear program
================================================================*/
Tableau::Tableau(   float* obj,             // coefficients of objective function
                    int var_count,          // number of decision variables in LP
                    float** constraints,    // 2-D array of constraint arrays, each of length var_count+1
                    int constraint_count):  // number of independent constraints
                    vars(var_count), m(constraint_count+1), n(var_count+constraint_count+1){

  //allocate space for mxn Tableau matrix
  arr = new float*[m];
  signs = new short[constraint_count];

  //Fill out constraint rows
  for (size_t i = 0; i < m-1; i++) {

    arr[i] = new float[n];
    signs[i] = 1;

    //Store coefficients of LHS of constraints
    for(size_t j = 0; j<var_count; j++) arr[i][j] = constraints[i][j];

    //Store slack variables
    for(size_t j = var_count; j<n-1; j++) arr[i][j] = (j-var_count==i) ? 1 : 0;

    //Store RHS of constraints
    arr[i][n-1]=constraints[i][var_count];
  }

  arr[m-1] = new float[n];
  //Store objective function coefficients
  for (size_t j = 0; j<n; j++) arr[m-1][j] = (j<var_count) ? -1*obj[j] : 0;

  status=false;
  feasible=true;

}


/*=========================================================================
Tableau:: Constructor - Creates simplex tableau from existing Tableau
                        by adding new branch constraint row for child node
==========================================================================*/
Tableau::Tableau(   Tableau& parent_tab,   // Original Tableau we are expanding
                    size_t decision_var,   // Variable to add branch constraint
                    float new_rhs,         // RHS of new branch constraint
                    bool geq ){            // If the new row is given in >= form

    //Determine if constraint has to be modified into standard form
    short sign = geq ? -1 : 1;

    m=parent_tab.get_rows()+1;
    n=parent_tab.get_columns()+1;
    vars=parent_tab.get_vars();

    arr = new float*[m];
    signs = new short[m-1];

    //copy first m-2 rows of parent_tab
    for (size_t i = 0; i < m-2; i++) {

      arr[i] = new float[n];
      signs[i] = parent_tab.get_signs()[i];

      //Copy parent_tab's coefficients of LHS of constraints and add new row LHS coefficients
      for(size_t j = 0; j<vars; j++) arr[i][j] = parent_tab[i][j];

      //Store slack variables
      for(size_t j = vars; j<n-1; j++) arr[i][j] = (j-vars==i) ? 1 : 0;

      //Copy parent_tab's RHS of constraints
      arr[i][n-1] = parent_tab[i][n-2];
    }

    //Add new single variable branch constraint
    arr[m-2] = new float[n];
    signs[m-2] = sign;

    for (size_t j = 0; j < n; j++) {
      arr[m-2][j] = (j==decision_var) ? sign : 0;
    }
    arr[m-2][n-2]=1;
    arr[m-2][n-1]=sign*new_rhs;

    //Copy objective function row
    arr[m-1] = new float[n];

    //Store objective function coefficients
    for (size_t j = 0; j<n; j++) arr[m-1][j] = (j<vars) ? parent_tab[m-2][j] : 0;

    status=false;
    feasible=true;


}



/*=========================================================================
Tableau::Destructor - Free space allocated for the Tableau
=========================================================================*/
Tableau::~Tableau(){

  //Free each length n column
  for (size_t i = 0; i < m; i++) {
    delete arr[i];
  }

  //Free Row memory
  delete arr;

  //Free sign cache
  delete signs;

}

/*=========================================================================
Tableau::[] - access tableau row, which can then be indexed with another []
=========================================================================*/
float* Tableau::operator[] (int i){
  return arr[i];
}


/*=================================================
Tableau::print - print current Tableau for debugging
====================================================*/
void Tableau::print(){

  std::cout<<std::endl<<"    ";
  for (int i=0;i<m;i++){

    for (int j=0; j<n;j++){
      //std::cout<<"HIT HERE"<<std::endl;
      std::cout<<arr[i][j]<<", ";
    }
    std::cout<<std::endl<<"    ";
  }

    std::cout<<std::endl;
}


/*=========================================================================
find_departing_var- find the row to use in the pivot, as well as which current
                    basic variable will be replaced.
=======================================================================*/
int find_departing_row(Tableau* tab){

  int m=tab->get_rows();
  int n=tab->get_columns();

  int entering_column=argmin((*tab)[m-1],n);
  int departing_row=0;

  //find non-negative starting variable
  for (size_t i = 0; i < m-1; i++)  if ((*tab)[i][entering_column] > 0) { departing_row = i; break;}

  for (size_t i = 0; i < m-1; i++) {

    if (((*tab)[i][entering_column]==0) || (*tab)[departing_row][entering_column] ==0) continue;

    //departing variable is the smallest ratio of (RHS/entering_column element)
    if ((*tab)[i][n-1]/(*tab)[i][entering_column]<(*tab)[departing_row][n-1]/(*tab)[departing_row][entering_column] &&
        (*tab)[i][n-1]/(*tab)[i][entering_column]>=0){
      departing_row=i;
    }

  }

  return departing_row;
}

/*=========================================================================================
Tableau::simplex - reduce tableau to optimal simplex form, and return optimal solution and arguments
===========================================================================================*/
OptimalSolution* Tableau::simplex(){

  //create backup of non-optimal tableau
  float** backup = create_backup(arr,m,n);

  //Initialize basic variables
  int* basic_vars = new int[m-1];
  for (size_t i = 0; i < m-1; i++) basic_vars[i]=vars+i;

  int entering_column, departing_row;
  float scale;

  //Determine pivot element for row operation
  entering_column=argmin(arr[m-1],n);
  departing_row=find_departing_row(this);

  //Continue until objective row has no negative values;
  while (arr[m-1][entering_column] < 0) {

    //scale pivot row
    scale=arr[departing_row][entering_column];
    for (size_t j = 0; j < n; j++) arr[departing_row][j] =   arr[departing_row][j] / scale;

    //Use row operations to create a column of zeros above and below the pivot element
    for (size_t i = 0; i < m; i++) {
      if (i != departing_row){
        add_rows(arr[i],arr[departing_row], arr[i][entering_column]/arr[departing_row][entering_column], n);
      }
    }

    //update basic variables
    basic_vars[departing_row]=entering_column;

    //move to next (possible) entering variable
    entering_column=argmin(arr[m-1],n);
    departing_row=find_departing_row(this);
  }

  //Create OptimalSolution structure for returning
  OptimalSolution* solution=new OptimalSolution;

  //Store optimal arguments from basic variables
  solution->args= new double[vars];
  for (size_t i = 0; i < vars; i++) solution->args[i]=0;

  for (size_t i = 0; i < m-1; i++) {
    //save non-slack variables
    if (basic_vars[i]<vars) solution->args[basic_vars[i]]=(double)arr[i][n-1];
  }

  solution->eval=arr[m-1][n-1];

  status=true;

  for (size_t i = 0; i < m-1; i++){if (arr[i][n-1] < 0) feasible=false;}

  delete basic_vars;

  //Restore Tableau and free memory

  //Free each length n column
  for (size_t i = 0; i < m; i++) {
    delete arr[i];
  }

  //Free Row memory
  delete arr;

  arr = backup;

  this->sol=solution;
  return solution;
}
