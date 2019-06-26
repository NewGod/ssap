/* main.cc  by Andrew Goldberg
 *     Driver for all the sp algorithms.
 *
 */

#include <stdlib.h>       // for atoi
#include <stdio.h>        // for printf
#include "sp.h"           // shortest-path class
#include <string.h>

#define MODUL ((long long) 1 << 62)

extern double timer();            // in timer.cc: tells time use
extern int parse_gr( long *n_ad, long *m_ad, Node **nodes_ad, Arc **arcs_ad, 
		  long *node_min_ad, char *problem_name );
#ifdef SINGLE_PAIR
extern int parse_p2p(long *sN_ad, long **source_array, long **sink_array, char *aName);
#else
extern int parse_ss(long *sN_ad, long **source_array, char *aName);
#endif

#define SZ_SPFA			"SPFA Algoritm"
#define SZ_BFS          "Breadth-First Search"

int main(int argc, char **argv)
{
   double tm = 0.0;
   Arc *arcs;
   Node *nodes, *source = NULL;
#ifdef SINGLE_PAIR
   Node *sink = NULL;
#endif
   long n, m, nmin, nQ;
   long *source_array=NULL;
#ifdef SINGLE_PAIR
   long *sink_array=NULL;
#endif
   char *szAlgorithm, gName[100], aName[100], oName[100];
   FILE *oFile;
   long long dist;
   double dDist;
   long long maxArcLen, minArcLen;
   SP *sp;
   ulong cLevels;
   ulong logDelta;
   long param;
   bool doBFS = false;

#if (defined CHECKSUM) && (!defined SINGLE_PAIR)
   Node *node;
#endif

   if ((argc < 4) || (argc > 5)) {
     fprintf(stderr, 
	     "Usage: \"%s <graph file> <aux file> <out file> [0]\"\n    or \"%s <graph file> <aux file> <out file> [<levels>] \"\n    or \"%s <graph file> <aux file> <out file> [-<log delta>] \"\n", argv[0], argv[0], argv[0]);
     exit(0);
   }

   strcpy(gName, argv[1]);
   strcpy(aName, argv[2]);
   strcpy(oName, argv[3]);
   oFile = fopen(oName, "a");

   fprintf(stderr,"c ---------------------------------------------------\n");
   fprintf(stderr,"c SQ/SQP DIMACS Challenge version \n");
   fprintf(stderr,"c ---------------------------------------------------\n");

   parse_gr(&n, &m, &nodes, &arcs, &nmin, gName ); 

#ifdef SINGLE_PAIR
#ifdef MLB
   printf("p res p2p q mbp\n");
#else
   printf("p res p2p q sqp\n");
#endif
   parse_p2p(&nQ, &source_array, &sink_array, aName);
#else
#ifdef MLB
   printf("p res ss mb\n");
#else
   printf("p res ss sq\n");
#endif
   parse_ss(&nQ, &source_array, aName);
#endif

   fprintf(oFile, "f %s %s\n", gName, aName);

   fprintf(stderr,"c\n");

   ArcLen(n, nodes, &minArcLen, &maxArcLen);      // other useful stats

   // sanity check
   dDist = maxArcLen * (double) (n-1);
   if (dDist > VERY_FAR) {
     fprintf(stderr, "Warning: distances may overflow\n");
     fprintf(stderr, "         proceed at your own risk!\n");
   }

   // figure out what algorithm to use
   cLevels = 0;
   logDelta = 0;
   szAlgorithm = SZ_SPFA;
   if (argc == 3) {
     param = atoi(argv[4]);
     
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
   }
   else {
     // cLevel and delta will be optimized in smartq.cc
     cLevels = 0;
     logDelta = 0;
   }

   sp = new SP(n, nodes, cLevels, logDelta, doBFS);

   if (doBFS) {  // get baseline timing
     tm = timer();
     dist = sp->BFS(source);
     tm = (timer() - tm);             // give an avg time

     ArcLen(n, nodes, &minArcLen, &maxArcLen);      // other useful stats
   
     fprintf(stderr,"c %s (%s)\n", szAlgorithm, argv[1]);
     fprintf(stderr,"c Nodes: %20ld       Arcs: %19ld\n",  n, m);
     fprintf(stderr,"c Baseline timing\n");
     fprintf(stderr,"c Time: %21.2f       MaxArcLen: %14lld\n", tm, maxArcLen);
     fprintf(stderr,"c Found:%21lld\n", dist);
     fprintf(stderr,"\n");
   }
   else {
     sp->init();

     fprintf(stderr,"c Nodes: %24ld       Arcs: %22ld\n",  n, m);
     fprintf(stderr,"c MinArcLen: %20lld       MaxArcLen: %17lld\n", 
	    minArcLen, maxArcLen);
     fprintf(stderr,"c Trials: %23ld\n", nQ);

     dist = 0;
     
     tm = timer();          // start timing
     for (int i = 0; i < nQ; i++) {
#ifdef SINGLE_PAIR
       source = nodes + source_array[i] - 1;
       sink = nodes + sink_array[i] - 1;
       sp->initS(source);
       sp->sp(source, sink);
#ifdef CHECKSUM
       if (sink->tStamp != source->tStamp)
	 fprintf(stderr,"c No path found\n");
       else {
	 dist = sink->dist;
	 fprintf(oFile,"d %lld\n", dist);
       }
#endif	 
#else
       source = nodes + source_array[i] - 1;
       sp->initS(source);
       sp->sp(source);
#ifdef CHECKSUM
       dist = source->dist;
       for ( node = nodes; node < nodes + n; node++ )
	 if (node->tStamp == sp->curTime) {
	   dist = (dist + (node->dist % MODUL)) % MODUL;
	 }
       fprintf(oFile,"d %lld\n", dist);
#endif
       
#endif
     }
     tm = (timer() - tm);   // finish timing

#ifndef CHECKSUM
     // now print the sp-specific stats
     sp->PrintStats(nQ);
     /* *round* the time to the nearest .01 of ms */
     fprintf(stderr,"c Time (ave, ms): %18.2f\n", 
	    1000.0 * tm/(float) nQ);

     fprintf(oFile, "g %ld %ld %lld %lld\n", 
	     n, m, minArcLen, maxArcLen);
     fprintf(oFile, "t %f\n", 1000.0 * tm/(float) nQ);
     fprintf(oFile, "v %f\n", (float) sp->cScans/ (float) nQ);
     fprintf(oFile, "i %f\n", (float) sp->cUpdates/ (float) nQ);
#endif
   }

   sp->~SP();
   free(source_array);
#ifdef SINGLE_PAIR
   free(sink_array);
#endif   
   fclose(oFile);

   return 0;
}
