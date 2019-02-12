
#ifndef _GROUP_HPP_
#define _GROUP_HPP_

// Group {
//   eventIn MFNode addChildren
//   eventIn MFNode removeChildren
//   exposedField MFNode children []
//   field SFVec3f bboxCenter 0 0 0
//   field SFVec3f bboxSize -1 -1 -1
// }

#include <vector>
#include "Node.hpp"



using namespace std;

class Group : public Node {

protected:
    vector<pNode> _children;
    Vec3f         _bboxCenter;
    Vec3f         _bboxSize;

public:
  
  Group();
  virtual ~Group();
  vector<pNode>& getChildren();
  unsigned       getNumberOfChildren();
  pNode          operator[](const int i); // returns i-th child or (Node*)0
  void           addChild(pNode child);
  void           removeChild(pNode child);

  void           clearBBox();
  bool           hasEmptyBBox();
    virtual void   updateBBox();
  Vec3f&         getBBoxCenter();
  Vec3f&         getBBoxSize();
  float          getBBoxDiameter();
  virtual bool   isGroup() const { return true; }

};

#endif // _GROUP_HPP_
