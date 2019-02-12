
#ifndef _QTLOGO_HPP_
#define _QTLOGO_HPP_

#include <QVector3D>

#include "wrl/SceneGraph.hpp"
#include "wrl/IndexedFaceSet.hpp"

class WrlQtLogo : public SceneGraph {

public:

  WrlQtLogo();

private:

  IndexedFaceSet* _pIfs;

  int newVertex(float x, float y, float z);
  int newNormal(float nx, float ny, float nz);
  void newTriangle(int i0, int i1, int i2);

  void newQuad(qreal x1, qreal y1, qreal x2, qreal y2,
            qreal x3, qreal y3, qreal x4, qreal y4);

  void newExtrude(qreal x1, qreal y1, qreal x2, qreal y2);

};

#endif // _QTLOGO_HPP_
