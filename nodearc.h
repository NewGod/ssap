/* nodearc.h  by Andrew Goldberg, started 5/25/01
 *     Contains the definition of the node and arc class needed
 *     for all the code.
 */

#ifndef NODEARC_H
#define NODEARC_H

struct Node;      // so the arc knows about it
   
struct Arc {
  long long len;         // arc length; long long may be an overkill
  struct Node *head;     // where the arc ends up
};

struct Node {
  long long dist;        // tentative shortest path length to some node
  Arc *first;          // first outgoing arc
  Arc *end;
  Node *parent;
  bool inQueue;
};


#endif
