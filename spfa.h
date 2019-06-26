#ifndef SPFA_H
#define SPFA_H
#include "queue.h"
#include "sp.h"

class SP;
class SPFA{
private:
	Queue* q;
	void init();
public:
	SPFA(long n, Node* nodes);
	~SPFA();
#ifdef SINGLE_PAIR 
	bool spfa(Node *source, Node *sink, SP *sp);
#else 
	bool spfa(Node *source, SP *sp);
#endif
};
#endif
