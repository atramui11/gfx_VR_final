
#ifndef _INDEXED_FACE_SET_HPP_
#define _INDEXED_FACE_SET_HPP_

// VRML'97
//
// IndexedFaceSet {
//   eventIn       MFInt32 set_colorIndex
//   eventIn       MFInt32 set_coordIndex
//   eventIn       MFInt32 set_normalIndex
//   eventIn       MFInt32 set_texCoordIndex
//   exposedField  SFNode  color             NULL
//   exposedField  SFNode  coord             NULL
//   exposedField  SFNode  normal            NULL
//   exposedField  SFNode  texCoord          NULL
//   field         SFBool  ccw               TRUE
//   field         MFInt32 colorIndex        []        # [-1,)
//   field         SFBool  colorPerVertex    TRUE
//   field         SFBool  convex            TRUE
//   field         MFInt32 coordIndex        []        # [-1,)
//   field         SFFloat creaseAngle       0         # [ 0,)
//   field         MFInt32 normalIndex       []        # [-1,)
//   field         SFBool  normalPerVertex   TRUE
//   field         SFBool  solid             TRUE
//   field         MFInt32 texCoordIndex     []        # [-1,)
// }

#include "Node.hpp"
#include <vector>

class IndexedFaceSet : public Node {

private:

  bool          _ccw;
  bool          _convex;
  float         _creaseAngle;
  bool          _solid;
  bool          _normalPerVertex;
  bool          _colorPerVertex;
  vector<float> _coord;
  vector<int>   _coordIndex;
  vector<float> _normal;
  vector<int>   _normalIndex;
  vector<float> _color;
  vector<int>   _colorIndex;
  vector<float> _texCoord;
  vector<int>   _texCoordIndex;

public:
  
  IndexedFaceSet();
  ~IndexedFaceSet();

  void           clear();
  bool&          getCcw();
  bool&          getConvex();
  float&         getCreaseangle();
  bool&          getSolid();
  bool&          getNormalPerVertex();
  bool&          getColorPerVertex();
  vector<float>& getCoord();
  vector<int>&   getCoordIndex();
  vector<float>& getNormal();
  vector<int>&   getNormalIndex();
  vector<float>& getColor();
  vector<int>&   getColorIndex();
  vector<float>& getTexCoord();
  vector<int>&   getTexCoordIndex();

  int            getNumberOfFaces();
  int            getNumberOfCorners();

  int            getNumberOfCoord();
  int            getNumberOfNormal();
  int            getNumberOfColor();
  int            getNumberOfTexCoord();

  void           setNormalPerVertex(bool value);
  void           setColorPerVertex(bool value);
    void removeTexCoord();

  enum Binding {
    PB_NONE = 0,
    PB_PER_VERTEX,
    PB_PER_FACE,
    PB_PER_FACE_INDEXED,
    PB_PER_CORNER
  };
  
  Binding getCoordBinding();
  Binding getNormalBinding();
  Binding getColorBinding();
  Binding getTexCoordBinding();

  virtual bool isIndexedFaceSet() const { return true; }

};

#endif // _INDEXED_FACE_SET_HPP_
