#ifndef QUEUE_H
#define QUEUE_H
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>

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
		end = (end + 1) % maxSize;
		data[end]=elt;
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
		curr = (curr + 1) % maxSize;
		return data[curr];
	}
};
#endif 
