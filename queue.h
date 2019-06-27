#ifndef QUEUE_H
#define QUEUE_H
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
#include<queue>
#include<vector>

#ifndef ulong 
typedef unsigned long ulong;
#endif
class Queue{
private:
	void **data;
	long end;
	long curr;
	long maxSize;
public: 
	Queue(long size):end(0),curr(0),maxSize(size){ 
		data = new void*[maxSize];
		assert(data);
	}
	void *push(void *elt){
		data[((++end)%=maxSize)]=elt;
		assert(end != curr);
		return data[end];
	}
	bool empty(){
		return end == curr;
	}
	void clear(){ 
		end = curr = 0; 
	}
	void* pop(){
		assert(end != curr); 
		(++curr) %= maxSize;
		return data[curr];
	}
};

class PriorityQueue{
	private:
	typedef std::pair<ulong, void*> item;
	std::priority_queue<item, std::vector<item>, std::greater<item> > q; 
	public: 
	void push(void *elt, ulong dist){
		return q.push(std::make_pair(dist, elt));
	}
	void* pop(){
		item ret = q.top();
		q.pop();
		return ret.second;
	}
	bool empty(){ 
		return q.empty();
	}
	void clear(){ 
		while (!q.empty()) q.pop();
	}
};

#endif 
