#include "spfa.h" 

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
#ifdef SINGLE_PAIR 
	bool reached = false;
#endif 
	q->push(source);
	while (!q->empty()) {
		Node* u = (Node*) q->pop();
#ifdef SINGLE_PAIR 
		if (u == sink){
			reached = true;
		}
#endif 
		u->inQueue = false;
		for (Arc* arc = u->first; arc <= u->end; arc++){ 
			Node* v = arc->head; 
			if (u->dist + arc->len < v->dist) {
				v->dist = u->dist + arc->len; 
				v->parent = u;
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

