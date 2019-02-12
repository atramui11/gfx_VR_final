
#ifndef _SHAPE_HPP_
#define _SHAPE_HPP_

#include "Node.hpp"

class Shape : public Node {

private:

  Node* _appearance;
  Node* _geometry;

public:
  
  Shape();
  virtual ~Shape();

  Node* getAppearance();
  Node* getGeometry();

  void setAppearance(Node* node);
  void setGeometry(Node* node);

  virtual bool isShape() const { return true; }

};

#endif // _SHAPE_HPP_
