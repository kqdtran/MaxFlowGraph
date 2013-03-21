// FHflowGraph client
#include <iostream>
#include <string>
using namespace std;
#include "FHflowGraph.h"

// --------------- main ---------------
int main()
{
   int final_flow;

   // build the first graph
   FHflowGraph<string, int> my_G;

   my_G.addEdge("s","a", 3); my_G.addEdge("s","b", 2); 
   my_G.addEdge("a","b", 1); my_G.addEdge("a","c", 3); my_G.addEdge("a","d", 4); 
   my_G.addEdge("b","d", 2);
   my_G.addEdge("c","t", 2); 
   my_G.addEdge("d","t", 3);  

   // show the original flow graph
   cout << "\nInitial Residual and Flow Adjacency Table:\n";
   my_G.showResAdjTable();
   my_G.showFlowAdjTable();

   my_G.setStartVert("s");
   my_G.setEndVert("t");

   final_flow = my_G.findMaxFlow();

   cout << "Final flow: " << final_flow << endl;
   cout << "\nFinal Residual and Flow Adjacency Table:\n";
   my_G.showResAdjTable();
   my_G.showFlowAdjTable(); 

   cout << "\n-------------------------------------------\n\n";

   // build the second graph
   FHflowGraph<string, int> my_G2;

   my_G2.addEdge("a","b", 6); my_G2.addEdge("a","c", 8); 
   my_G2.addEdge("b","d", 6); my_G2.addEdge("b","e", 3);
   my_G2.addEdge("c","d", 3); my_G2.addEdge("c","e", 3);
   my_G2.addEdge("d","f", 8); 
   my_G2.addEdge("e","f", 6);  

   // show the original flow graph
   cout << "\nInitial Residual and Flow Adjacency Table:\n";
   my_G2.showResAdjTable();
   my_G2.showFlowAdjTable();

   my_G2.setStartVert("a");
   my_G2.setEndVert("f");

   final_flow = my_G2.findMaxFlow();

   cout << "Final flow: " << final_flow << endl;
   cout << "\nFinal Residual and Flow Adjacency Table:\n";
   my_G2.showResAdjTable();
   my_G2.showFlowAdjTable();

   return 0; 
}

/* ------------------------------- Sample Run ----------------------------------

Initial Residual and Flow Adjacency Table:
------------------------
Adj Res List for s: a(3) b(2)
Adj Res List for a: s(0) b(1) c(3) d(4)
Adj Res List for b: s(0) a(0) d(2)
Adj Res List for c: a(0) t(2)
Adj Res List for d: a(0) b(0) t(3)
Adj Res List for t: c(0) d(0)

------------------------
Adj Flow List for s: a(0) b(0)
Adj Flow List for a: b(0) c(0) d(0)
Adj Flow List for b: d(0)
Adj Flow List for c: t(0)
Adj Flow List for d: t(0)
Adj Flow List for t:

Final flow: 5

Final Residual and Flow Adjacency Table:
------------------------
Adj Res List for s: a(0) b(0)
Adj Res List for a: s(3) b(1) c(1) d(3)
Adj Res List for b: s(2) a(0) d(0)
Adj Res List for c: a(2) t(0)
Adj Res List for d: a(1) b(2) t(0)
Adj Res List for t: c(2) d(3)

------------------------
Adj Flow List for s: a(3) b(2)
Adj Flow List for a: b(0) c(2) d(1)
Adj Flow List for b: d(2)
Adj Flow List for c: t(2)
Adj Flow List for d: t(3)
Adj Flow List for t:


-------------------------------------------


Initial Residual and Flow Adjacency Table:
------------------------
Adj Res List for a: b(6) c(8)
Adj Res List for b: a(0) d(6) e(3)
Adj Res List for c: a(0) d(3) e(3)
Adj Res List for d: b(0) c(0) f(8)
Adj Res List for e: b(0) c(0) f(6)
Adj Res List for f: d(0) e(0)

------------------------
Adj Flow List for a: b(0) c(0)
Adj Flow List for b: d(0) e(0)
Adj Flow List for c: d(0) e(0)
Adj Flow List for d: f(0)
Adj Flow List for e: f(0)
Adj Flow List for f:

Final flow: 12

Final Residual and Flow Adjacency Table:
------------------------
Adj Res List for a: b(0) c(2)
Adj Res List for b: a(6) d(1) e(2)
Adj Res List for c: a(6) d(0) e(0)
Adj Res List for d: b(5) c(3) f(0)
Adj Res List for e: b(1) c(3) f(2)
Adj Res List for f: d(8) e(4)

------------------------
Adj Flow List for a: b(6) c(6)
Adj Flow List for b: d(5) e(1)
Adj Flow List for c: d(3) e(3)
Adj Flow List for d: f(8)
Adj Flow List for e: f(4)
Adj Flow List for f:

Press any key to continue . . .
----------------------------------------------------------------------------- */
