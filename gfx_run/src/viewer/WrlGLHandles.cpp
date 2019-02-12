
#include <iostream>
#include "WrlGLHandles.hpp"

//////////////////////////////////////////////////////////////////////
WrlGLHandles::WrlGLHandles():
  _vshader((QOpenGLShader*)0),
  _fshader((QOpenGLShader*)0),
  _program((QOpenGLShaderProgram*)0),
  _vertexAttr(-1),
  _matrixAttr(-1),
  _materialAttr(-1),
  _buffer((QOpenGLBuffer*)0) {
  _matrix.setToIdentity();
}

//////////////////////////////////////////////////////////////////////
WrlGLHandles::~WrlGLHandles() {
  delete _program;
  delete _vshader;
  delete _fshader;
  if(_buffer==(QOpenGLBuffer*)0) return;
  _buffer->destroy();
  delete _buffer;
  _buffer = (QOpenGLBuffer*)0;
}

//////////////////////////////////////////////////////////////////////
QMatrix4x4& WrlGLHandles::getMatrix() {
  return _matrix;
}

void WrlGLHandles::setColor(QColor& materialColor) {
    _materialColor = materialColor;
}

//////////////////////////////////////////////////////////////////////
void WrlGLHandles::setGeometry
  (float hx0, float hy0, float hx1, float hy1) {

  const char *vShaderCode =
    "attribute highp vec4 vertex;\n"
    "uniform mediump mat4 matrix;\n"
    "uniform mediump vec4 matcolor;\n"
    "varying mediump vec4 color;\n"
    "void main(void) {\n"
    "  color = matcolor;\n"
    "  gl_Position = matrix * vertex;\n"
    "}\n";

  const char *fShaderCode =
    "varying mediump vec4 color;\n"
    "void main(void) {\n"
    "  gl_FragColor = color;\n"
    "}\n";

  const int nVertices = 6;
  _buffer = new QOpenGLBuffer();
  _buffer->create();
  _buffer->bind();
  QVector<GLfloat> buf;
  buf.resize(3*nVertices);
  GLfloat *p = buf.data();

  // (1,0) --- (1,1)
  //   |    \    |
  // (0,0) --- (0,1)

  // triangle 1
  //     glVertex3f(hx0,hy0,0.0f);
  *p++ = hx0; *p++ = hy0; *p++ = 0.0f;
  //     glVertex3f(hx0,hy1,0.0f);
  *p++ = hx0; *p++ = hy1; *p++ = 0.0f;
  //     glVertex3f(hx1,hy0,0.0f);
  *p++ = hx1; *p++ = hy0; *p++ = 0.0f;
  // triangle 2
  //     glVertex3f(hx1,hy1,0.0f);
  *p++ = hx1; *p++ = hy1; *p++ = 0.0f;
  //     glVertex3f(hx1,hy0,0.0f);
  *p++ = hx1; *p++ = hy0; *p++ = 0.0f;
  //     glVertex3f(hx0,hy1,0.0f);
  *p++ = hx0; *p++ = hy1; *p++ = 0.0f;

  _buffer->allocate(buf.constData(), buf.count() * sizeof(GLfloat));
  _buffer->release();

  // create the vertex shader
  _vshader = new QOpenGLShader(QOpenGLShader::Vertex);
  _vshader->compileSourceCode(vShaderCode);

  // create the fragment shader
  _fshader = new QOpenGLShader(QOpenGLShader::Fragment);
  _fshader->compileSourceCode(fShaderCode);

  // create the shader program
  _program = new QOpenGLShaderProgram;
  _program->addShader(_vshader);
  _program->addShader(_fshader);
  _program->link();

  _vertexAttr   = _program->attributeLocation("vertex");
  _materialAttr = _program->uniformLocation("matcolor");
  _matrixAttr   = _program->uniformLocation("matrix");
}

//////////////////////////////////////////////////////////////////////
void WrlGLHandles::paint(QOpenGLFunctions& f) {

  if(_buffer==(QOpenGLBuffer*)0) return;

  _program->bind();
  _program->setUniformValue(_matrixAttr, _matrix);
  _program->setUniformValue(_materialAttr, _materialColor);
  _program->enableAttributeArray(_vertexAttr);
  _buffer->bind();
  _program->setAttributeBuffer(_vertexAttr, GL_FLOAT, 0, 3, 3*sizeof(GLfloat));
  _buffer->release();

  f.glDrawArrays(GL_TRIANGLES, 0, 6);

  _program->disableAttributeArray(_vertexAttr);
  _program->release();

}
