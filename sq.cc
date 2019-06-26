/* sq.cc  by Andrew Goldberg
 *     Driver for all the sp algorithms.  You call it like this:
 *
 */

#include <stdlib.h>       // for atoi
#include <stdio.h>        // for printf
#include "sp.h"           // shortest-path class

#ifdef SINGLE_PAIR
extern int parse( long *n_ad, long *m_ad, Node **nodes_ad, Arc **arcs_ad, 
		  Node **source_ad, Node **sink_ad,
		  long *node_min_ad, char *problem_name );
#else
extern int parse( long *n_ad, long *m_ad, Node **nodes_ad, Arc **arcs_ad, 
		  Node **source_ad, long *node_min_ad, char *problem_name );
#endif
extern double timer();            // in timer.cc: tells time use

#define ITERATIONS      1         // run the problem this many times
#define BASE_ITERATIONS 1       // run the baseline
#define SZ_DIK_SMARTQ   "Dijkstra with Smart Queues"
#define SZ_DIK_MLB      "Dijkstra with Multi-Level Buckets"
#define SZ_BFS          "Breadth-First Search"
#define BUCKETS_DEFAULT 2

int main(int argc, char **argv)
{
   double tm;
   Arc *arcs;
   Node *nodes, *source;
#ifdef SINGLE_PAIR
   Node *sink;
#else
   Node *node;
#endif
   long n, m, nmin, iIter;
   char name[21], *szAlgorithm;
   double dist;
   long long maxArcLen, minArcLen;
   SP *sp;
   ulong cLevels;
   ulong logDelta;
   long param;
   bool doBFS = false;

   if (argc != 2) {
#ifdef MLB
     fprintf(stderr, 
"Usage: \"mlb 0\" or \"mlb <levels>\" or \"mlb -<log delta>\"\n");
#else
     fprintf(stderr, 
"Usage: \"sq 0\" or \"sq <levels>\" or \"sq -<log delta>\"\n");
#endif
     exit(0);
   }

   printf("c ---------------------------------------------------\n");
   printf("c MLB/SQ DIMACS Challenge version \n");
   printf("c Copyright C by IG Systems, Inc., igsys@eclipse.net.\n");
   printf("c Commercial use requires a license\n");
   printf("c ---------------------------------------------------\n");

#ifdef SINGLE_PAIR
   parse( &n, &m, &nodes, &arcs, &source, &sink, &nmin, name );   // in parse_sp.c
#else
   parse( &n, &m, &nodes, &arcs, &source, &nmin, name );   // in parse_sp.c
#endif
   printf("c\n");

   ArcLen(n, nodes, &minArcLen, &maxArcLen);      // other useful stats

   // sanity check
   dist = (double) maxArcLen * (double) (n-1);
   if (dist > VERY_FAR) {
     fprintf(stderr, "Warning: distances may overflow\n");
     fprintf(stderr, "         proceed at your own risk!\n");
   }

   // figure out what algorithm to use
   cLevels = 0;
   logDelta = 0;
   szAlgorithm = SZ_DIK_SMARTQ;
#ifdef MLB
   szAlgorithm = SZ_DIK_MLB;
#endif
   param = atoi(argv[1]);

   if (param == -99) { // this special case goes first
     // do breadth-first search
     szAlgorithm = SZ_BFS;
     doBFS = true;
   }
   else {
     if (param == 0) {
       // cLevel and delta will be optimized in smartq.cc
       cLevels = 0;
       logDelta = 0;
     }
     
     if (param > 0) {
       cLevels = (ulong) param;
     }
     
     if (param < 0) {
       logDelta = (ulong) -param;
     }
   }

   sp = new SP(n, nodes, cLevels, logDelta, doBFS);

   if (doBFS) {  // get baseline timing
     tm = timer();
     // we only need to run sp once if we're just collecting stats
     for ( iIter = 0; iIter < BASE_ITERATIONS; iIter++ )
       dist = sp->BFS(source);
     tm = (timer() - tm);             // give an avg time

     ArcLen(n, nodes, &minArcLen, &maxArcLen);      // other useful stats
   
     printf("c %s (%s)\n", szAlgorithm, argv[1]);
     printf("c Nodes: %20ld       Arcs: %19ld\n",  n, m);
     /* *round* the time to the nearest .01 of a second */
     printf("c Baseline timing\n");
     printf("c Time: %21.2f       MaxArcLen: %14lld\n", tm + 0.005, maxArcLen);
     printf("c Found:%21.2f\n", dist);
     printf("\n");

     return 0;
   }

   tm = timer();                                           // in time.c
   // we only need to run sp once if we're just collecting stats
#ifndef ALLSTATS
   for ( iIter = 0; iIter < ITERATIONS; iIter++ )
#endif
#ifdef SINGLE_PAIR
     sp->sp(source, sink);
#else
     sp->sp(source);
#endif
   tm = (timer() - tm) / (double)ITERATIONS;               // give an avg time

   dist = 0.0;
#ifdef SINGLE_PAIR
   if (sink != NULL) {
     dist = sink->dist;
   }
#else
   for ( node = nodes; node < nodes + n; node++ )          // calculate dists
      if ( node->parent )                                  // reachable
	 dist += node->dist;
#endif


   printf("c %s (%s) -> problem name: %s\n", szAlgorithm, argv[1], name);
   printf("c Nodes: %26ld       Arcs: %25ld\n",  n, m);
        /* *round* the time to the nearest .01 of a second */
   printf("c Time: %27.2f       MaxArcLen: %20lld\n", tm + 0.005, maxArcLen);
   printf("c Cost: %27.2f       MinArcLen: %20lld\n", dist, minArcLen);

      // now print the sp-specific stats
   sp->PrintStats();
   printf("\n");

   sp->~SP();
   return 0;
}
