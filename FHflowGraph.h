// File FHflowGraph.h EXPERIMENT
// Template definitions for FHflowGraph.  
#ifndef FHFLOWGRAPH_H
#define FHFLOWGRAPH_H
#include <limits.h>
#include <utility>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <stack>
#include <iostream>
using namespace std;

// CostType is some numeric type that expresses cost of edges
// Object is the non-graph data for a vertex
template <class Object, typename CostType>
class FHflowVertex
{
   // internal typedefs to simplify syntax
   typedef FHflowVertex<Object, CostType>* VertPtr;
   typedef map<VertPtr, CostType> EdgePairList;

public:
   static int n_sort_key;
   static stack<int> key_stack;
   static enum { SORT_BY_DATA, SORT_BY_DIST } e_sort_type;
   static bool SetNSortType( int which_type );
   static void pushSortKey() { key_stack.push(n_sort_key); }
   static void popSortKey() { n_sort_key = key_stack.top(); key_stack.pop(); };

   static int const INFINITY = INT_MAX;  // defined in limits.h
   EdgePairList flow_adj_list, res_adj_list;
   Object data;
   CostType dist;
   VertPtr next_in_path;  // used for client-specific info

   FHflowVertex( const Object & x = Object() );

   void addToFlowAdjList(VertPtr neighbor, CostType cost);
   void addToResAdjList(VertPtr neighbor, CostType cost);
   bool operator<( const FHflowVertex<Object, CostType> & rhs) const;
   const FHflowVertex<Object, CostType> & operator=
      ( const FHflowVertex<Object, CostType> & rhs);
   void showFlowAdjList();
   void showResAdjList();
};

// static const initializations for Vertex --------------
template <class Object, typename CostType>
int FHflowVertex<Object, CostType>::n_sort_key 
= FHflowVertex<Object, CostType>::SORT_BY_DATA;

template <class Object, typename CostType>
stack<int> FHflowVertex<Object, CostType>::key_stack;
// ------------------------------------------------------

template <class Object, typename CostType>
bool FHflowVertex<Object, CostType>::SetNSortType( int which_type )
{
   switch (which_type)
   {
   case SORT_BY_DATA:
   case SORT_BY_DIST:
      n_sort_key = which_type;
      return true;
   default:
      return false;
   }
}

template <class Object, typename CostType>
FHflowVertex<Object, CostType>::FHflowVertex( const Object & x) 
: data(x), dist((CostType)INFINITY), next_in_path(NULL)
{
   // nothing to do
}

template <class Object, typename CostType>
void FHflowVertex<Object, CostType>::addToFlowAdjList
(FHflowVertex<Object, CostType> *neighbor, CostType cost)
{ 
   flow_adj_list[neighbor] = cost;
}

template <class Object, typename CostType>
void FHflowVertex<Object, CostType>::addToResAdjList
(FHflowVertex<Object, CostType> *neighbor, CostType cost)
{ 
   res_adj_list[neighbor] = cost;
}

template <class Object, typename CostType>
bool FHflowVertex<Object, CostType>::operator<(
   const FHflowVertex<Object, CostType> & rhs) const
{
   switch (n_sort_key)
   {
   case SORT_BY_DIST:
      return (dist < rhs.dist);
   case SORT_BY_DATA:
      return (data < rhs.data);
   default:
      return false;
   }
}

template <class Object, typename CostType>
const FHflowVertex<Object, CostType> & FHflowVertex<Object, CostType>::operator=(
   const FHflowVertex<Object, CostType> & rhs)
{
   flow_adj_list = rhs.flow_adj_list;
   res_adj_list = rhs.res_adj_list;
   data = rhs.data;
   dist = rhs.dist;
   next_in_path = rhs.next_in_path;;
   return *this;
}

template <class Object, typename CostType>
void FHflowVertex<Object, CostType>::showFlowAdjList()
{
   typename EdgePairList::iterator iter;

   cout << "Adj Flow List for " << data << ": ";
   for (iter = flow_adj_list.begin(); iter != flow_adj_list.end(); ++iter)
      cout << iter->first->data << "(" << iter->second << ") ";
   cout << endl;
}

template <class Object, typename CostType>
void FHflowVertex<Object, CostType>::showResAdjList()
{
   typename EdgePairList::iterator iter;

   cout << "Adj Res List for " << data << ": ";
   for (iter = res_adj_list.begin(); iter != res_adj_list.end(); ++iter)
      cout << iter->first->data << "(" << iter->second << ") ";
   cout << endl;
}

template <class Object, typename CostType>
class FHflowGraph
{
   // internal typedefs to simplify syntax
   typedef FHflowVertex<Object, CostType> Vertex;
   typedef FHflowVertex<Object, CostType>* VertPtr;
   typedef map<VertPtr, CostType> EdgePairList;
   typedef set<VertPtr> VertPtrSet;
   typedef set<Vertex> VertexSet;

private:
   VertPtrSet vert_ptr_set;
   VertexSet vertex_set;
   VertPtr start_vert_ptr, end_vert_ptr; // start and end of flow problem

public:
   FHflowGraph () {}
   void addEdge(const Object &source, const Object &dest, CostType cost);
   VertPtr addToVertexSet(const Object & object);
   void showFlowAdjTable();
   void showResAdjTable();
   void clear();
   bool setStartVert(const Object &x);
   bool setEndVert(const Object &x);
   CostType findMaxFlow(); 

private:
   VertPtr getVertexWithThisData(const Object & x);

   // helpers for main algorithm
   bool establishNextFlowPath();
   CostType getLimitingFlowOnResPath();
   bool adjustPathByCost(CostType cost);
   CostType getCostOfResEdge(VertPtr src, VertPtr dst);
   bool addCostToResEdge(VertPtr src, VertPtr dst, CostType cost);
   bool addCostToFlowEdge(VertPtr src, VertPtr dst, CostType cost);
};

template <class Object, typename CostType>
FHflowVertex<Object, CostType>* FHflowGraph<Object, CostType>::addToVertexSet(
   const Object & object)
{
   pair<typename VertexSet::iterator, bool> ret_val;
   VertPtr v_ptr;

   // save sort key for client
   Vertex::pushSortKey();
   Vertex::SetNSortType(Vertex::SORT_BY_DATA);

   // build and insert vertex into master list
   ret_val = vertex_set.insert( Vertex(object) );

   // get pointer to this vertex and put into vert pointer list
   v_ptr = (VertPtr)&*ret_val.first;
   vert_ptr_set.insert(v_ptr);

   Vertex::popSortKey();  // restore client sort key
   return v_ptr;
}

template <class Object, typename CostType>
void FHflowGraph<Object, CostType>::clear()
{
   vertex_set.clear();
   vert_ptr_set.clear();
}

template <class Object, typename CostType>
void FHflowGraph<Object, CostType>::addEdge(
   const Object &source, const Object &dest, CostType cost )
{
   VertPtr src, dst;

   // put both source and dest into vertex list(s) if not already there
   src = addToVertexSet(source);
   dst = addToVertexSet(dest);

   // add to adjacency lists
   src->addToResAdjList(dst, cost);
   dst->addToResAdjList(src, 0);
   src->addToFlowAdjList(dst, 0);
}

template <class Object, typename CostType>
FHflowVertex<Object, CostType>* FHflowGraph<Object, CostType>::getVertexWithThisData(
   const Object & x)
{
   typename VertexSet::iterator find_result;
   Vertex vert(x);

   Vertex::pushSortKey();  // save client sort key
   Vertex::SetNSortType(Vertex::SORT_BY_DATA);
   find_result = vertex_set.find(vert);
   Vertex::popSortKey();  // restore client value

   if ( find_result == vertex_set.end() )
      return NULL;
   return  (VertPtr)&*find_result;  // the address of the vertex in the set of originals
}

template <class Object, typename CostType>
void FHflowGraph<Object, CostType>::showFlowAdjTable()
{
   typename VertPtrSet::iterator iter;

   cout << "------------------------ \n";
   for (iter = vert_ptr_set.begin(); iter != vert_ptr_set.end(); ++iter)
      (*iter)->showFlowAdjList();
   cout << endl;
}

template <class Object, typename CostType>
void FHflowGraph<Object, CostType>::showResAdjTable()
{
   typename VertPtrSet::iterator iter;

   cout << "------------------------ \n";
   for (iter = vert_ptr_set.begin(); iter != vert_ptr_set.end(); ++iter)
      (*iter)->showResAdjList();
   cout << endl;
}

template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::setStartVert(const Object &x)
{
   VertPtr tempPtr = getVertexWithThisData(x);
   if (tempPtr == NULL)
      return false;
   start_vert_ptr = tempPtr;
   return true;
}

template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::setEndVert(const Object &x)
{
   VertPtr tempPtr = getVertexWithThisData(x);
   if (tempPtr == NULL)
      return false;
   end_vert_ptr = tempPtr;
   return true;
}

// main max flow algorithm and its private helpers
template <class Object, typename CostType>
CostType FHflowGraph<Object, CostType>::findMaxFlow()
{
   typename EdgePairList::iterator edge_pr_iter;
   CostType total_flow = 0;
   
   while (establishNextFlowPath())
      adjustPathByCost(getLimitingFlowOnResPath());

   // find the total flow
   for ( edge_pr_iter = start_vert_ptr->flow_adj_list.begin(); 
      edge_pr_iter != start_vert_ptr->flow_adj_list.end();
      edge_pr_iter++ )
      total_flow += edge_pr_iter->second;
   return total_flow;
}

template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::establishNextFlowPath()
{
   typename VertPtrSet::iterator v_iter;
   typename EdgePairList::iterator edge_pr_iter;
   VertPtr w_ptr, v_ptr;
   CostType cost_vw;
   queue<VertPtr> partially_processed_verts;

   if (start_vert_ptr == NULL)
      return false;

   // initialize the vertex list and place the starting vert in p_p_v queue
   for (v_iter = vert_ptr_set.begin(); v_iter != vert_ptr_set.end(); ++v_iter)
   {
      (*v_iter)->dist = Vertex::INFINITY;
      (*v_iter)->next_in_path = NULL;
   }

   start_vert_ptr->dist = 0;
   partially_processed_verts.push( start_vert_ptr ); // or, FHbinHeap::insert(), e.g.

   // outer dijkstra loop
   while( !partially_processed_verts.empty() )
   {
      v_ptr = partially_processed_verts.front(); 
      partially_processed_verts.pop();

      // inner dijkstra loop: for each vert adj to v, lower its dist to s if you can
      for (edge_pr_iter = v_ptr->res_adj_list.begin(); 
         edge_pr_iter != v_ptr->res_adj_list.end();
         edge_pr_iter++)
      {
         w_ptr = edge_pr_iter->first;
         cost_vw = edge_pr_iter->second;
         if (cost_vw == 0)
            continue;

         if ( v_ptr->dist + cost_vw < w_ptr->dist )
         {
            w_ptr->dist = v_ptr->dist + cost_vw;
            w_ptr->next_in_path = v_ptr; 

            // *w_ptr now has improved distance, so add w_ptr to p_p_v queue
            partially_processed_verts.push(w_ptr); 
         }

         // if we find end_vert_ptr, stop the algorithm
         if (w_ptr == end_vert_ptr)
            return true;
      }
   }

   // couldn't reach end_vert_ptr
   return false;
}

template <class Object, typename CostType>
CostType FHflowGraph<Object, CostType>::getLimitingFlowOnResPath()
{
   CostType currFlow, minFlow = Vertex::INFINITY; // set minFlow to largest value
   VertPtr vp;

   if (start_vert_ptr == NULL || end_vert_ptr == NULL)
      return Vertex::INFINITY;

   for (vp = end_vert_ptr; vp != start_vert_ptr; vp = vp->next_in_path)
   {
      if ( vp->next_in_path == NULL )
         break;
      currFlow = getCostOfResEdge(vp->next_in_path, vp);
      if (currFlow < minFlow)
         minFlow = currFlow;
   }

   return minFlow;
}

template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::adjustPathByCost(CostType cost)
{
   VertPtr vp;

   if (start_vert_ptr == NULL || end_vert_ptr == NULL)
      return false;

   for (vp = end_vert_ptr; vp != start_vert_ptr; vp = vp->next_in_path)
   {
      if ( vp->next_in_path == NULL )
         break;
      if ( !addCostToResEdge(vp->next_in_path, vp, cost ) || 
         !addCostToFlowEdge(vp->next_in_path, vp, cost ) )
         return false;
   }

   return true;
}

template <class Object, typename CostType>
CostType FHflowGraph<Object, CostType>::getCostOfResEdge(VertPtr src, VertPtr dst)
{
   typename EdgePairList::iterator edge_pr_iter;
   typename VertPtrSet::iterator find_result;
   find_result = vert_ptr_set.find(src);
   if (src == NULL || dst == NULL || find_result == vert_ptr_set.end())
      return Vertex::INFINITY;

   VertPtr vp = *find_result;
   for (edge_pr_iter = vp->res_adj_list.begin(); 
      edge_pr_iter != vp->res_adj_list.end(); 
      edge_pr_iter++)
   {
      if (edge_pr_iter->first == dst)
         return edge_pr_iter->second;
   }
   return Vertex::INFINITY;
}

template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::addCostToResEdge(VertPtr src, VertPtr dst, 
                                                     CostType cost)
{
   typename EdgePairList::iterator edge_pr_iter;
   typename VertPtrSet::iterator find_result;
   find_result = vert_ptr_set.find(src);
   if (src == NULL || dst == NULL || find_result == vert_ptr_set.end())
      return false;

   VertPtr vp = *find_result;
   for (edge_pr_iter = vp->res_adj_list.begin(); 
      edge_pr_iter != vp->res_adj_list.end(); 
      edge_pr_iter++)
   {
      if (edge_pr_iter->first == dst)
      {
         edge_pr_iter->second -= cost; // subtract cost from edges from start to end
         vp = edge_pr_iter->first; // now set the new vp to dst and find src
         for (edge_pr_iter = vp->res_adj_list.begin(); 
            edge_pr_iter != vp->res_adj_list.end(); 
            edge_pr_iter++)
         {
            if (edge_pr_iter->first == src)
            {
               edge_pr_iter->second += cost; // add cost to reverse edges
               return true;
            }
         }
      }
   }

   // couldn't find either src or dst
   return false;
}

template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::addCostToFlowEdge(VertPtr src, VertPtr dst, 
                                                      CostType cost)
{
   typename EdgePairList::iterator edge_pr_iter;
   typename VertPtrSet::iterator find_result;
   find_result = vert_ptr_set.find(src);
   if (src == NULL || dst == NULL || find_result == vert_ptr_set.end())
      return false;

   VertPtr vp = *find_result;
   for (edge_pr_iter = vp->flow_adj_list.begin(); 
      edge_pr_iter != vp->flow_adj_list.end(); 
      edge_pr_iter++)
   {
      if (edge_pr_iter->first == dst)
      {
         edge_pr_iter->second += cost;
         return true;
      }
   }

   // not found in src's flow list
   vp = edge_pr_iter->first;
   for (edge_pr_iter = vp->flow_adj_list.begin(); 
      edge_pr_iter != vp->flow_adj_list.end(); 
      edge_pr_iter++)
   {
      if (edge_pr_iter->first == src)
      {
         edge_pr_iter->second -= cost;
         return true;
      }
   }

   return false;
}

#endif