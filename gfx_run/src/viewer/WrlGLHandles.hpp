
#ifndef _WRL_GLHANDLES_HPP
#define _WRL_GLHANDLES_HPP

#include <QColor>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

class WrlGLHandles {

public:

  WrlGLHandles();
  ~WrlGLHandles();

  void setColor(QColor& materialColor);
  void setGeometry(float hx0, float hy0, float hx1, float hy1);

  QMatrix4x4&          getMatrix();
  void                 paint(QOpenGLFunctions& f);

private:

  QOpenGLShader        *_vshader;
  QOpenGLShader        *_fshader;
  QOpenGLShaderProgram *_program;

  int                   _vertexAttr;
  int                   _matrixAttr ;
  int                   _materialAttr;

  QOpenGLBuffer         *_buffer;
  QColor                _materialColor;
  QMatrix4x4            _matrix;

};

#endif // _WRL_GLHANDLES_HPP
