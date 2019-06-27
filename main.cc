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
extern int parse_ss(long *sN_ad, long **source_array, char *aName);

#define SZ_DIJK		"Dijkstra Algoritm"

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
	char gName[100], aName[100], oName[100];
	FILE *oFile;
	long long dist;
	double dDist;
	long long maxArcLen, minArcLen;
	SP *sp;

#if (defined CHECKSUM) && (!defined SINGLE_PAIR)
	Node *node;
#endif

	if (argc != 4) {
		fprintf(stderr, 
				"Usage: \"%s <graph file> <aux file> <out file> [0]\"\n    or \"%s <graph file> <aux file> <out file>  \"\n", argv[0], argv[0]);
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

	printf("p res ss dijkstra\n");
	parse_ss(&nQ, &source_array, aName);

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

	sp = new SP(n, nodes);

	sp->init();

	fprintf(stderr,"c Nodes: %24ld       Arcs: %22ld\n",  n, m);
	fprintf(stderr,"c MinArcLen: %20lld       MaxArcLen: %17lld\n", 
			minArcLen, maxArcLen);
	fprintf(stderr,"c Trials: %23ld\n", nQ);

	dist = 0;

	tm = timer();          // start timing
	for (int i = 0; i < nQ; i++) {
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

	sp->~SP();
	free(source_array);
#ifdef SINGLE_PAIR
	free(sink_array);
#endif   
	fclose(oFile);
	return 0;
}
