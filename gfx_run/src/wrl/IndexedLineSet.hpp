

#ifndef _INDEXED_LINE_SET_HPP_
#define _INDEXED_LINE_SET_HPP_

// VRML'97
//
// IndexedLineSet {
//   eventIn       MFInt32 set_colorIndex
//   eventIn       MFInt32 set_coordIndex
//   exposedField  SFNode  coord             NULL
//   field         MFInt32 coordIndex        []        # [-1,)
//   exposedField  SFNode  color             NULL
//   field         MFInt32 colorIndex        []        # [-1,)
//   field         SFBool  colorPerVertex    TRUE
// }

#include "Node.hpp"
#include <vector>

using namespace std;

class IndexedLineSet : public Node {

private:

  vector<float> _coord;
  vector<int>   _coordIndex;
  vector<float> _color;
  vector<int>   _colorIndex;
  bool          _colorPerVertex;

public:
  
  IndexedLineSet();

  void           clear();
  bool&          getColorPerVertex();
  vector<float>& getCoord();
  vector<int>&   getCoordIndex();
  vector<float>& getColor();
  vector<int>&   getColorIndex();

  int            getNumberOfPolylines();

  int            getNumberOfCoord();
  int            getNumberOfColor();

  void           setColorPerVertex(bool value);

  virtual bool    isIndexedLineSet() const { return             true; }
  virtual string  getType()          const { return "IndexedLineSet"; }

  typedef bool    (*Property)(IndexedLineSet& ifs);
  typedef void    (*Operator)(IndexedLineSet& ifs);

  // can be used for debugging
  virtual void    printInfo(string indent);
};

#endif // _INDEXED_LINE_SET_HPP_
