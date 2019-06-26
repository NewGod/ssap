#include "spfa.hh" 

void SPFA::init(){ 
	q->clear();
}

SPFA::SPFA(long n, Node* nodes){
	q = new Queue(n);
}

SPFA::~SPFA(){
	delete q;
}

#ifdef SINGLE_PAIR 
bool SPFA::spfa(Node *source, Node *sink, SP *sp){
#else 
bool SPFA::spfa(Node *source, SP *sp){
#endif
	init();
	bool reached = false;
	q->push(source)
	while (!q->empty()) {
		Node* u = q->pop();
#ifdef SINGLE_PAIR 
		if (u == sink){
			reached = true;
		}
#endif 
		u->inQueue = false;
		for (arc = currentNode->firsr; arc <= lastArc; arc++){ 
			Node* v = arc.head; 
			if (u->dist + arc->len < v->dist) {
				v->dist = u->dist + arc->len; 
				if (!v->inQueue) { 
					v->inQueue = true; 
					q->push(v);
				}
			}
		}
	}
#ifdef SINGLE_PAIR 
	return(reached);
#endif
}

