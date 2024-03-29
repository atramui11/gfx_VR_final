
#include <iostream>
#include "SceneGraphTraversal.hpp"
#include "Shape.hpp"

// Use as follows
//
// SceneGraph* wrl=NULL;
// // load wrl from file or create
// SceneGraphTraversal t = new SceneGraphTraversal(*wrl);
// Node* child;
// t.start();
// while((child=t.next())!=null) {
//   // do something with the node
// }

SceneGraphTraversal::SceneGraphTraversal(SceneGraph& wrl):
  _wrl(wrl) {
  start();
}

void SceneGraphTraversal::start() {
  _node.clear();
  int n = _wrl.getNumberOfChildren();
  while((--n)>=0)
    _node.push_back(_wrl[n]);
}

Node* SceneGraphTraversal::next() {
  Node* next = (_node.size()>0)?_node.back():(Node*)0;
  // advance for next call
  if(_node.size()>0) {
    Node* node = _node.back(); _node.pop_back();
    if(node->isGroup()) {
      Group* group = (Group*)node;
      int n = group->getNumberOfChildren();
      while((--n)>=0)
        _node.push_back((*group)[n]);
    }
  }
  return next;
}

