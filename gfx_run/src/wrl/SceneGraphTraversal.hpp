

// Use as follows
//
// SceneGraph* wrl=(SceneGraph*)0;
// // load wrl from file or create
// SceneGraphTraversal sgt(*wrl);
// for(Node* node=(Node*)0;(node=sgt.next())!=(Node*)0;sgt.advance()) {
//   do something with node
// }

#ifndef _SCENE_GRAPH_TRAVERSAL_HPP_
#define _SCENE_GRAPH_TRAVERSAL_HPP_

#include "SceneGraph.hpp"

class SceneGraphTraversal {

private:

  SceneGraph&    _wrl;
  vector<Node*> _node;

public:

  SceneGraphTraversal(SceneGraph& wrl);

  void  start();
  Node* next();
  void  advance() {}

};

#endif // _SCENE_GRAPH_TRAVERSAL_HPP_
