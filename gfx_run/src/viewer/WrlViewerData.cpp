
#include "WrlViewerData.hpp"

WrlViewerData::WrlViewerData():
  // _initialized(false),
  _pWrl((SceneGraph*)0),
  _bboxDepth(0),
  _bboxCube(true),
  _bboxOccupied(false),
  _bboxScale(1.05f) {
}

WrlViewerData::~WrlViewerData() {
  if(_pWrl!=(SceneGraph*)0) delete _pWrl;
}

void WrlViewerData::setSceneGraph(SceneGraph* pWrl) {
  if(pWrl!=_pWrl) {
    if(_pWrl!=(SceneGraph*)0) delete _pWrl;
    _pWrl = pWrl;
  }
}
