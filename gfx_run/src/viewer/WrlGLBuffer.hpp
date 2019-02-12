
#ifndef _WRL_GL_BUFFER_HPP_
#define _WRL_GL_BUFFER_HPP_

#include <QColor>
#include <QVector>
#include <QVector3D>
#include <QOpenGLBuffer>
#include "wrl/IndexedFaceSet.hpp"
#include "wrl/IndexedLineSet.hpp"

class WrlGLBuffer : public QOpenGLBuffer {

public:

  enum Type {
    MATERIAL, MATERIAL_NORMAL, COLOR, COLOR_NORMAL
  };

  WrlGLBuffer();
  WrlGLBuffer(IndexedFaceSet* pIfs, QColor& materialColor);
  WrlGLBuffer(IndexedLineSet* pIls, QColor& materialColor);

  Type     getType() const             { return                       _type; } 
  unsigned getNumberOfVertices() const { return                  _nVertices; }
  unsigned getNumberOfNormals()  const { return                   _nNormals; }
  unsigned getNumberOfColors()   const { return                    _nColors; }

  bool     hasFaces()            const { return                   _hasFaces; }
  bool     hasPolylines()        const { return               _hasPolylines; }
  bool     hasPoints()           const { return !(_hasFaces||_hasPolylines); }
  bool     hasColor()            const { return                   _hasColor; }
  bool     hasNormal()           const { return                  _hasNormal; }

protected:

  Type     _type;
  unsigned _nVertices;
  unsigned _nNormals;
  unsigned _nColors;
  bool     _hasFaces;
  bool     _hasPolylines;
  bool     _hasColor;
  bool     _hasNormal;

};

#endif // _WRL_GL_BUFFER_HPP_
