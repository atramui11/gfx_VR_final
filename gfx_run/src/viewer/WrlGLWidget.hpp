
#ifndef _WRL_GLWIDGET_HPP_
#define _WRL_GLWIDGET_HPP_

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QMatrix4x4>
#include <QTime>
#include <QVector>
#include <QPushButton>
#include <QMouseEvent>
#include <QDragMoveEvent>

#include "util/BBox.hpp"
#include "wrl/SceneGraph.hpp"
#include "wrl/Transform.hpp"
#include "wrl/Shape.hpp"
#include "wrl/IndexedFaceSet.hpp"
#include "wrl/Appearance.hpp"
#include "wrl/Material.hpp"

#include "WrlViewerData.hpp"
#include "WrlGLShader.hpp"
#include "WrlGLHandles.hpp"

class WrlMainWindow;

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(QOpenGLShader)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class WrlGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {

  Q_OBJECT

public:

  WrlGLWidget(WrlMainWindow *mw,
              const QColor& backgroundColor,
              const QColor& materialColor);
  ~WrlGLWidget();

  WrlViewerData& getData();

  SceneGraph* getSceneGraph();
  void        setSceneGraph(SceneGraph* pWrl, bool resetHomeView);

  void invertNormal(); // TODO

  WrlViewerData& getData() const;

public slots:

  void setQtLogo();

protected:

  void initializeGL()         Q_DECL_OVERRIDE;
  void paintGL()              Q_DECL_OVERRIDE;
  void resizeGL(int w, int h) Q_DECL_OVERRIDE;

  // virtual void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;

  void paintData(QMatrix4x4& mvp);
  void paintGroup(QMatrix4x4& mvp, Group* group);
  void paintTransform(QMatrix4x4& mvp, Transform* transform);
  void paintSceneGraph(QMatrix4x4& mvp, SceneGraph* wrl);
  void paintShape(QMatrix4x4& mvp, Shape* shape);

  virtual void	enterEvent(QEvent * event)                 Q_DECL_OVERRIDE;
  virtual void	leaveEvent(QEvent * event)                 Q_DECL_OVERRIDE;
  virtual void	mousePressEvent(QMouseEvent * event)       Q_DECL_OVERRIDE;
  virtual void	mouseReleaseEvent(QMouseEvent * event)     Q_DECL_OVERRIDE;
  virtual void	mouseMoveEvent(QMouseEvent * event)        Q_DECL_OVERRIDE;

private:

  void _setHomeView(const bool identity);
  void _setProjectionMatrix();
  void _zoom(const float value);

private:

  WrlMainWindow*        _mainWindow;
  WrlViewerData         _data;
  // BBox*                 _bbox;
  float                 _bboxDiameter;

  QVector3D             _eye;
  QVector3D             _center;
  QVector3D             _up;

  int                   _mouseZone;
  bool                  _mouseInside;
  bool                  _mousePressed;
  Qt::MouseButtons      _buttons;
  int                   _prevMouseX;
  int                   _prevMouseY;
  bool                  _zone4enabled;
  float                 _translateStep;

  QVector3D             _cameraTranslation;
  QMatrix4x4            _viewRotation;
  QMatrix4x4            _projectionMatrix;

  bool                  _animationOn;
  qreal                 _fAngle;

  map<Shape*,WrlGLShader*> _shaderMap;

  WrlGLHandles*         _handles;

  QColor                _background;
  QColor                _material;
  QVector3D             _lightSource;

  static int            _borderUp;
  static int            _borderDown;
  static int            _borderLeft;
  static int            _borderRight;

  static float          _angleHomeX;
  static float          _angleHomeY;
  static float          _angleHomeZ;

};

#endif // _WRL_GLWIDGET_HPP_
