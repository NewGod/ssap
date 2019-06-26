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
#define SZ_SPFA			"SPFA Algorithm"
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
   long param;

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
   szAlgorithm = SZ_SPFA;
   param = atoi(argv[1]);

   }

   sp = new SP(n, nodes);

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
