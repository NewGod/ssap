// sp.h  by Andrew Goldberg
//     Started 5/21/01
//     The main driver for shortest-path routines.  Whenever you want
//     to use an SP, you include sp.h and call sp::sp with the
//     algorithm you want, taken from the #defines below.

#ifndef SP_H
#define SP_H

#include <stdlib.h>
#include "values.h"
#include "nodearc.h"
#include "dijkstra.h"
#ifndef ulong 
typedef unsigned long ulong;
#endif

#define VERY_FAR            9223372036854775807LL // LLONG_MAX
#define FAR                 MAXLONG

int sp_openarc(Arc *arc);              // whether SP thinks an arc is usable
extern void ArcLen(long cNodes, Node *nodes,
		   long long *pMin = NULL, long long *pMax = NULL);
class Dijkstra;
class SP{
 private:
   Node *nodes;

   Dijkstra *dijkstra;	
//** add new SP data structure here **//

 public:
   long cNodes;
   SP(long cNodesGiven, Node *nodesGiven);
   ~SP();
   void init();
   void initS(Node *source);
   void initNode(Node *source);
   void sp(Node *source);
   // these must be public, alas, so Heap and Bucket can modify them
   long cCalls;         // # of times SP has been called since initialization
   long long cScans;         // # of nodes SP algorithm has looked at (since init)
   long long cUpdates;       // # of times a node value was lowered (since init)

   void PrintStats(long tries);
   void initStats();
   int nodeId(Node *i);
   unsigned int curTime;
};

#endif
