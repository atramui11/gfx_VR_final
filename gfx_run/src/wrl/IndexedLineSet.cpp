
#include <iostream>
#include "IndexedLineSet.hpp"

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

IndexedLineSet::IndexedLineSet():
  _colorPerVertex(true)
{}


void IndexedLineSet::clear() {
    //_colorPerVertex=true;
    
    _coord.clear();
    _coordIndex.clear();
    
    _color.clear();
    _colorIndex.clear();
}

bool& IndexedLineSet::getColorPerVertex()
{ return _colorPerVertex; }
vector<float>& IndexedLineSet::getCoord()
{return _coord; }
vector<int>& IndexedLineSet::getCoordIndex()
{ return _coordIndex; }
vector<float>& IndexedLineSet::getColor()
{ return _color; }
vector<int>& IndexedLineSet::getColorIndex()
{ return _colorIndex; }
int IndexedLineSet::getNumberOfCoord()
{ return (int)(_coord.size()/3); }
int IndexedLineSet::getNumberOfColor()
{ return (int)(_color.size()/3); }

int IndexedLineSet::getNumberOfPolylines()   {
  int nPolylines = 0;

  // count number of -1's in coordIndex

    for (int i=0; i<_coordIndex.size(); i++) {
        if (_coordIndex.at(i)==-1) {
            nPolylines++;
        }
    }
    
  return nPolylines;
}

void IndexedLineSet::setColorPerVertex(bool value) {
  _colorPerVertex = value;
}

void IndexedLineSet::printInfo(string indent) {
  std::cout << indent;
  if(_name!="") std::cout << "DEF " << _name << " ";
  std::cout << "IndexedLineSet {\n";
  std::cout << indent << "  nPolylines        = " << getNumberOfPolylines() << "\n";
  std::cout << indent << "  nCoord            = " << _coord.size()/3        << "\n";
  std::cout << indent << "  coordIndex.size() = " << _coordIndex.size()     << "\n";
  std::cout << indent << "  colorPerVertex    = " << _colorPerVertex        << "\n";
  std::cout << indent << "  nColor            = " << _color.size()/3        << "\n";
  std::cout << indent << "  colorIndex.size() = " << _colorIndex.size()     << "\n";
  std::cout << indent << "}\n";
}
