/**@file elpp.h
 * @brief Elementary Longest Path Problem solver
 * @author Leonardo Taccari 
 */

#ifndef __ELPP_H__
#define __ELPP_H__

#include "type.h"
#include "graph.h"
#include <map>
#include <memory>
#include <unordered_map>
#include <string>
#include <lemon/time_measure.h>

using namespace std;
typedef unordered_map<NODE_PAIR,IloNumVar> ARC_VARS;
typedef map<NODE,vector<NODE>> ADJ_LIST;
typedef unordered_map<NODE_PAIR,int> INDEX_MAP;

class ElppSolver
{
   public:
      ElppSolver(){};
      ElppSolver(IloEnv env,
            NODE_PAIR st_,
            std::shared_ptr<Graph> g_ptr, 
            ElppForm formulation,
            bool relax,
            int timelimit,
            double epsilon,
            int max_cuts);

      void update_problem(
            const unordered_map<NODE_PAIR, double>& obj_coeff 
            );
      void update_problem(
            const unordered_map<NODE_PAIR, double>& obj_coeff, 
            const map<NODE_PAIR, double>& lbs,
            const map<NODE_PAIR, double>& ubs
            );

      void update_problem(
            const unordered_map<NODE_PAIR, double>& obj_coeff, 
            const map<NODE_PAIR, double>& lbs,
            const map<NODE_PAIR, double>& ubs,
            const unordered_map<NODE_PAIR, double>& lhs, 
            double rhs
            );

      void add_constraint(
            const unordered_map<NODE_PAIR, double>& lhs, double lb, double ub
            );

      void solve();
      void solveRoot();
      void solveLP();

      void clear();

      void append_info(string filename, string instance_name);

      void writeLP(string filename);

      void printInstance(
            string filename,
            const unordered_map<NODE_PAIR, double>& obj_coeff, 
            const map<NODE_PAIR, double>& lbs,
            const map<NODE_PAIR, double>& ubs
            );

      IloAlgorithm::Status getStatus();
      double getObjValue();
      double getBestObjValue();
      double getValue(NODE_PAIR arc);
      double getValue(NODE node);
      bool isInteger();
      int pathLength();

   private:
      void build_problem_sec();
      void build_problem_mf(bool sep);
      void build_problem_sf();
      void build_problem_rlt();
      void build_problem_mtz();
      void build_problem_dl();

      unordered_map<NODE_PAIR, IloNumVar>          sigma_vars;
      unordered_map<NODE_PAIR,IloNumVar>           u_var;
      unordered_map<TRIPLET,IloNumVar>           qq_var;
      unordered_map<NODE,IloNumVar>                zz_var;
      unordered_map<NODE,IloNumVar>                p_var;

      IloNumVarArray x_vararray;
      unordered_map<NODE_PAIR, int> index;

      IloModel                model;
      IloCplex                cplex;
      IloRange               extra_con;
      IloObjective            objective;
   

      int timelimit;
      ElppForm formulation;
      bool relax;
      double elapsed_time;
      double elapsed_ticks;
      int ncuts;
      double tol;
      int max_cuts;

      //Graph structure
      NODE_PAIR st;
      std::shared_ptr<Graph> G;

};


#endif
