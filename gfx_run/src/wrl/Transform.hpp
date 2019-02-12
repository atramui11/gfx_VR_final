
#ifndef _TRANSFORM_HPP_
#define _TRANSFORM_HPP_

// Transform {
//   eventIn MFNode addChildren
//   eventIn MFNode removeChildren
//   exposedField SFVec3f center 0 0 0
//   exposedField MFNode children []
//   exposedField SFRotation rotation 0 0 1 0
//   exposedField SFVec3f scale 1 1 1
//   exposedField SFRotation scaleOrientation 0 0 1 0
//   exposedField SFVec3f translation 0 0 0
//   field SFVec3f bboxCenter 0 0 0
//   field SFVec3f bboxSize -1 -1 -1
// }

#include "Group.hpp"
#include "Rotation.hpp"

class Transform : public Group {

private:

  Vec3f         _center;           // 0 0 0
  Rotation      _rotation;         // 0 0 1 0
  Vec3f         _scale;            // 1 1 1
  Rotation      _scaleOrientation; // 0 0 1 0
  Vec3f         _translation;      // 0 0 0

  // inherited from Group
  // vector<Node*> _children;
  // Vec3f         _bboxCenter;
  // Vec3f         _bboxSize;

public:
  
  Transform();
  virtual ~Transform();

  Vec3f&    getCenter();
  Rotation& getRotation();
  Vec3f&    getScale();
  Rotation& getScaleOrientation();
  Vec3f&    getTranslation();

  void      setCenter(Vec3f& value);
  void      setRotation(Rotation& value);
  void      setRotation(Vec4f& value);
  void      setScale(Vec3f& value);
  void      setScaleOrientation(Rotation& value);
  void      setScaleOrientation(Vec4f& value);
  void      setTranslation(Vec3f& value);

  void      getMatrix(float* T /*[16]*/);

  virtual bool isTransform() const { return true; }

private:
  static void _makeRotation(Rotation& r, float* R /*[9]*/);

};

#endif // _TRANSFORM_HPP_
