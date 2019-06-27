// sp.cc  by Andrew Goldberg.

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "sp.h"

void ArcLen(long cNodes, Node *nodes,
	    long long *pMin /* = NULL */, long long *pMax /* = NULL */)
// finds the max arc length and min arc length of a graph.
// Used to init for buckets.  pMax or pMin can be NULL.  For fun,
// and utility, returns MaxArcLen.
{
   Arc *lastArc, *arc;
   long long maxLen = 0, minLen = VERY_FAR;

            // arcs are stored sequentially.  The last arc overall
            // is one before the first arc of the sentinel node.
   lastArc = (nodes+cNodes)->first - 1;
   for ( arc = nodes->first; arc <= lastArc; arc++ )
   {
      if ( arc->len > maxLen )
	 maxLen = arc->len;
      if ( arc->len < minLen )
         minLen = arc->len;
   }
   if ( pMin )   *pMin = minLen;
   if ( pMax )   *pMax = maxLen;
}


//-------------------------------------------------------------
// SP::SP()
// SP::~SP()
//     Each algorithm needs a different data structure.  We add
//     them all here and then send them in the sp() call.  This
//     way we need to allocate memory only once even though there
//     may be many sp calls.  Note that we expect the sp calls
//     all to be made on the same graph, though info about it
//     may change.
//-------------------------------------------------------------

SP::SP(long cNodesGiven, Node *nodesGiven)
{
  long long minArcLen, maxArcLen;

  cNodes = cNodesGiven;
  nodes = nodesGiven;
  cCalls = cScans = cUpdates = 0;     // no stats yet
  spfa = NULL;
  //** initialize data type for new sp algorithm here **//

  ArcLen(cNodes, nodes, &minArcLen, &maxArcLen);
  spfa = new SPFA(cNodes, nodes);
}

SP::~SP()
{
	if (spfa) delete spfa;
//** delete data structure for new sp algorithm here **//
}

//-------------------------------------------------------------
// SP::initNode()
//     Initially, all distances are set to VERY_FAR, presumably
//     longer than any real distance
//-------------------------------------------------------------
void SP::initNode(Node *currentNode)
{
   currentNode->dist = VERY_FAR;   // not yet a shortest path
   currentNode->tStamp = curTime;
   currentNode->parent = NULL;
   currentNode->inQueue = false;
   
}

//-------------------------------------------------------------
// SP::init()
//     Initially, all lengths are set to VERY_FAR, presumably
//     longer than any real distance, and the parent field is
//     set to NULL to indicate the sp tree does not yet exist.
//-------------------------------------------------------------
void SP::init()
{
   Node *currentNode;

   curTime = 0;
   for (currentNode=nodes; currentNode < nodes + cNodes; currentNode++) {
     initNode(currentNode);
   }
}

void SP::initS(Node *source)
{
  initNode(source);
  source->parent = source;
  source->dist = 0;               // all distances are to the source
}


//-------------------------------------------------------------
// SP::sp()
//     Just calls the SP algorithm specified in spType, and returns
//     the overall stat that the SP algorithm returns.  Actual
//     code for sp algorithms can be found in their cc files:
//     dijkstra.cc, etc.
//        If OPEN is not NULL, we test all arcs with OPEN and
//     ignore those that aren\'t OPEN.  Otherwise all ARCS are open
//        If StopAt is not NULL, we do shortest paths only until
//     we reach a node for which StopAt returns 1 (it may check
//     for nodes with excess capacity, for instance).  Thus,
//     only part of the distance tree will be correct, but that
//     part will include a path from StopAt to a source.
//        RETURNS the node that StopAt stopped at, or NULL if
//     StopAt was NULL or no node passed StopAt\'s test.
//-------------------------------------------------------------
void SP::sp(Node *source)
{
   cCalls++;

   spfa->spfa(source, this);
}
//-------------------------------------------------------------
// SP::PrintStats()
//     Prints stats appropriate for an sp run.  First it prints
//     stats that hold for all sp algorithms: number of times
//     it's called, number of scans (nodes looked at), number
//     of updates (times a node's distance changes).  Then it
//     calls the appropriate data structure to print out
//     algorithm-specific stats.
//-------------------------------------------------------------
void SP::PrintStats(long tries)
{
   fprintf(stderr, "c Scans (ave): %20.1f     Improvements (ave): %10.1f\n", 
	  (float) cScans / (float) tries, 
	  (float) cUpdates / (float) tries);
	// smartq->PrintStats(tries);
}

void SP::initStats()
{
  cScans = cUpdates = 0;
}

int SP::nodeId(Node *i)

{
  return i - nodes + 1;
}
