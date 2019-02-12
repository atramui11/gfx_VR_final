

#ifndef _WRL_GLSHADER_HPP
#define _WRL_GLSHADER_HPP

#include <QColor>
#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include "WrlGLBuffer.hpp"

class WrlGLShader {

private:

  static const char *s_vsMaterial;
  static const char *s_vsMaterialNormal;
  static const char *s_vsColor;
  static const char *s_vsColorNormal;
  static const char *s_fsColor;

public:

  // constructor for IndexedFaceSet : lightSource!=(QVector3D*)0
  // constructor for IndexedLineSet : lightSource==(QVector3D*)0

  WrlGLShader(QColor& materialColor, QVector3D* lightSource=(QVector3D*)0);

  ~WrlGLShader();

  int            getNumberOfVertices();
  WrlGLBuffer*   getVertexBuffer() const;
  QMatrix4x4&    getMVPMatrix();

  void           setPointSize(float pointSize);
  void           setLineWidth(float lineWidth);
  void           setVertexBuffer(WrlGLBuffer* vb);
  void           setMVPMatrix(const QMatrix4x4& mvp);

  void           paint(QOpenGLFunctions& f);

private:

  QOpenGLShader        *_vshader;
  QOpenGLShader        *_fshader;
  QOpenGLShaderProgram *_program;

  int                   _pointSizeAttr;
  int                   _lineWidthAttr;
  int                   _vertexAttr;
  int                   _normalAttr;
  int                   _colorAttr;
  int                   _mvpMatrixAttr ;
  int                   _materialAttr;
  int                   _lightSourceAttr;

  WrlGLBuffer          *_vertexBuffer;
  QColor                _materialColor;
  QVector3D*            _lightSource;
  QMatrix4x4            _mvpMatrix; // viewport * projection * modelView
  float                 _pointSize;
  float                 _lineWidth;

};

#endif // _WRL_GLSHADER_HPP
