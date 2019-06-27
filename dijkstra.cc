#include "dijkstra.h" 

void Dijkstra::init(){ 
	q->clear();
}

Dijkstra::Dijkstra(long n, Node* nodes){
	q = new PriorityQueue();
}

Dijkstra::~Dijkstra(){
	delete q;
}

void Dijkstra::dijkstra(Node *source, SP *sp){
	init();
	q->push(source, 0);
	int cnt = 0;
	while (!q->empty()) {
		Node* u = (Node*) q->pop();
		if (u->done) continue;
		if (++cnt == sp->cNodes) break;
		u->done = true;
		Arc* end = (u+1) -> first;
		for (Arc* arc = u->first; arc < end; arc++){ 
			Node* v = arc->head; 
			if (u->dist + arc->len < v->dist) {
				v->dist = u->dist + arc->len; 
				v->parent = u;
				q->push(v, v->dist);
			}
		}
	}
}

