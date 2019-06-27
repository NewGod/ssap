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

void SPFA::spfa(Node *source, SP *sp){
	init();
	q->push(source);
	while (!q->empty()) {
		Node* u = (Node*) q->pop();
		u->inQueue = false;
		Arc* end = (u+1) -> first;
		for (Arc* arc = u->first; arc < end; arc++){ 
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
}

