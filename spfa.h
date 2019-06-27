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
	void spfa(Node *source, SP *sp);
};
#endif
