#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "queue.h"
#include "sp.h"

class SP;
class Dijkstra{
private:
	PriorityQueue* q;
	void init();
public:
	Dijkstra(long n, Node* nodes);
	~Dijkstra();
	void dijkstra(Node *source, SP *sp);
};
#endif
