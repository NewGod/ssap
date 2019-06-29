#include "deltastepping.h" 
#include "sp.h"
#include <cstdio>

void DeltaStepping::init(){ 
}

DeltaStepping::DeltaStepping(SP* Sp, long n, Node* nodes, int Delta):
	delta(Delta),
	bucketCnt( Sp->mx / delta + 1),
	sp(Sp){
}

DeltaStepping::~DeltaStepping(){
}

void DeltaStepping::relax(BucketsType &buckets, Node* x, ulong dist){
	if (dist < x->dist) {
		if (x->dist != VERY_FAR) {
			int idx = int(x->dist / delta) % bucketCnt;
			buckets[idx].erase(x);
		}
		sp->cUpdates ++;
		int idx = int(dist / delta) % bucketCnt;
		buckets[idx].insert(x);
		x->dist = dist;
	}
}
std::vector<Request> DeltaStepping::findRequests(std::set<Node*> &bucket, int type){
	int idx = 0;
	int bucketSize = bucket.size();
	Node* tmp[bucketSize];
	for (auto it = bucket.begin(); it != bucket.end(); ++it) {
		tmp[idx++] = *it;
	}
	std::vector<Request> ret;
#pragma omp parallel for shared(tmp)
	for (int i=0; i < bucketSize; i++) {
		std::vector<Request> tmp_ret;
		sp->cScans ++;
		Arc* end = (tmp[i] + 1)->first;
		if (type == 0) { //light
			for (Arc* e = tmp[i]->first; e < end; e++) {
				if (e->len <= delta) 
					tmp_ret.push_back(std::make_pair(e->head, tmp[i]->dist + e->len));
			}
		}else{
			for (Arc* e = tmp[i]->first; e < end; e++) {
				if (e->len > delta) 
					tmp_ret.push_back(std::make_pair(e->head, tmp[i]->dist + e->len));
			}
		}
#pragma omp critical 
		for (auto it = tmp_ret.begin(); it != tmp_ret.end(); ++it) {
			ret.push_back(*it);
		}
	}
	return ret;
}
void DeltaStepping::relaxAll(BucketsType &buckets, std::vector<Request> requests){
	for (auto it = requests.begin(); it != requests.end(); ++it) 
		relax(buckets, it->first, it->second);
}
void DeltaStepping::deltaStepping(Node *source){
	init();
	sp->curTime++;
	sp->init();
	BucketsType buckets(bucketCnt);
	relax(buckets, source, 0);
	int cycIdx = 0;
	while (true){
		int idx = -1;
		std::vector<Request> requests;
		for (int i=0; i < bucketCnt; i++) {
			int x = i + cycIdx;
			if (x >= bucketCnt) x -= bucketCnt;
			if (!buckets[x].empty()) {
				idx = x;
				break;
			}
		}
		cycIdx++;
		if (cycIdx >= bucketCnt) cycIdx = 0;
		if (idx == -1) break;
		std::set<Node*> tmp;
		while (!buckets[idx].empty()) {
			requests = findRequests(buckets[idx], 0);
			for (auto it = buckets[idx].begin(); it != buckets[idx].end(); ++it) 
				tmp.insert(*it);
			buckets[idx].clear();
			relaxAll(buckets, requests);
		}
		requests = findRequests(tmp, 1);
		relaxAll(buckets, requests);
	}
}
