#ifndef DELTA_STEPPING_H
#define DELTA_STEPPING_H
#include "sp.h"
#include <vector>
#include <set>

typedef unsigned long ulong;
class SP;
typedef std::pair<Node*, ulong> Request;
class DeltaStepping{
private:
	void init();
	double delta;
	int bucketCnt;
	SP* sp;
	typedef std::vector<std::set<Node*>> BucketsType;
	void relax(BucketsType &buckets, Node* x, ulong dist);
	std::vector<Request> findRequests(std::set<Node*> &bucket, int type);
	void relaxAll(BucketsType &buckets, std::vector<Request> requests);
public:
	DeltaStepping(SP* sp, long n, Node* nodes, int delta);
	~DeltaStepping();
	void deltaStepping(Node *source);
};
#endif
