
#ifndef _WRL_VIEWER_DATA_HPP_
#define _WRL_VIEWER_DATA_HPP_

#include "wrl/SceneGraph.hpp"

class WrlViewerData {
public:
  WrlViewerData();
  ~WrlViewerData();

  void           setSceneGraph(SceneGraph* pWrl);

  SceneGraph*    getSceneGraph()
  { return _pWrl; }
  vector<float>& getFunctionVertices()
  { return _functionVertices; }
  vector<float>& getFunctionCells()
  { return _functionCells; }
  int            getBBoxDepth()
  { return _bboxDepth; }
  void           setBBoxDepth(int d)
  { _bboxDepth = (d<0)?0:(d>10)?10:d; }
  bool           getBBoxCube()
  { return _bboxCube; }
  void           setBBoxCube(bool value)
  { _bboxCube = value; }
  bool           getBBoxOccupied()
  { return _bboxOccupied; }
  void           setBBoxOccupied(bool value)
  { _bboxOccupied = value; }
  float          getBBoxScale()
  { return   _bboxScale; }
  void           setBBoxScale(float scale)
  { _bboxScale = (scale<0.0f)?0.0f:scale; }

private:

  SceneGraph*   _pWrl;
  vector<float> _functionVertices;
  vector<float> _functionCells;
  int           _bboxDepth;
  bool          _bboxCube;
  bool          _bboxOccupied;
  float         _bboxScale;

};

#endif // _WRL_VIEWER_DATA_HPP_
