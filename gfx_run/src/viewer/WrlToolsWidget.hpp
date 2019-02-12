
#ifndef _WRL_TOOLSWIDGET_HPP_
#define _WRL_TOOLSWIDGET_HPP_

#include <QResizeEvent>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>

class WrlMainWindow;

class WrlToolsWidget : public QWidget {

  Q_OBJECT

public:

  WrlToolsWidget(WrlMainWindow *mw);
  ~WrlToolsWidget();

public slots:
                   
  void updateState();

protected:

  // virtual void	enterEvent(QEvent * event)                 Q_DECL_OVERRIDE;
  // virtual void	leaveEvent(QEvent * event)                 Q_DECL_OVERRIDE;
  // virtual void	mousePressEvent(QMouseEvent * event)       Q_DECL_OVERRIDE;
  // virtual void	mouseReleaseEvent(QMouseEvent * event)     Q_DECL_OVERRIDE;
  // virtual void	mouseMoveEvent(QMouseEvent * event)        Q_DECL_OVERRIDE;

  virtual void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;

private:

private slots:

  // bounding box
  void bboxAdd();
  void bboxRemove();
  void bboxSetDepth(int depth);
  void bboxDepthUp();
  void bboxDepthDown();
  void bboxScaleEnter();
  void bboxCube();
  void bboxCubeSetState(int state);

  // scene graph
  void setNormalNone();
  void setNormalPerVertex();
  void setNormalPerFace();
  void setNormalPerCorner();
  void normalInvert();
  void shapeIfsShow();
  void shapeIfsHide();
  void shapeIlsShow();
  void shapeIlsHide();

  // points
  void pointsRemove();
  void pointsShow();
  void pointsHide();

  // edges
  void edgesAdd();
  void edgesRemove();
  void edgesShow();
  void edgesHide();

  // surface
  void surfaceRemove();
  void surfaceShow();
  void surfaceHide();

  // heuristic surface reconstruction
  void fitMultiplePlanes();
  void fitContinuous();
  void fitWatertight();

  // optimal surface reconstruction
  void optimalCGHard();
  void optimalCGSoft();
  void optimalJacobi();

  // multigrid
  void multigridFiner();
  void multigridCoarser();
  void multigridRun();

private:

  WrlMainWindow*        _mainWindow;

  QLabel*               _labelCanvas;
  QLabel*               _labelCanvasWidth;
  QLineEdit*            _editCanvasWidth;
  QLabel*               _labelCanvasHeight;
  QLineEdit*            _editCanvasHeight;

  QLabel*               _labelBBox;
  QLabel*               _labelBBoxSize;
  QSpinBox*             _spinBoxBBoxDepth;
  QPushButton*          _pushButtonBBoxAdd;
  QPushButton*          _pushButtonBBoxRemove;
  QLabel*               _labelBBoxScale;
  QLineEdit*            _editBBoxScale;
  QCheckBox*            _checkBoxBBoxCube;

  QLabel*               _labelGridCells;
  QLineEdit*            _editGridCells;
  QLabel*               _labelGridVertices;
  QLineEdit*            _editGridVertices;

  QLabel*               _labelSceneGraph;
  QLabel*               _labelSceneGraphNormalBinding;
  QPushButton*          _pushButtonSceneGraphNormalNone;
  QPushButton*          _pushButtonSceneGraphNormalPerVertex;
  QPushButton*          _pushButtonSceneGraphNormalPerFace;
  QPushButton*          _pushButtonSceneGraphNormalPerCorner;
  QPushButton*          _pushButtonSceneGraphNormalInvert;
  QLabel*               _labelSceneIndexedFaceSets;
  QPushButton*          _pushButtonSceneGraphIndexedFaceSetsShow;
  QPushButton*          _pushButtonSceneGraphIndexedFaceSetsHide;
  QLabel*               _labelSceneIndexedLineSets;
  QPushButton*          _pushButtonSceneGraphIndexedLineSetsShow;
  QPushButton*          _pushButtonSceneGraphIndexedLineSetsHide;

  QLabel*               _labelPoints;
  QLineEdit*            _editPointsNumber;
  QCheckBox*            _checkBoxPointsHasNormals;
  QCheckBox*            _checkBoxPointsHasColors;
  QPushButton*          _pushButtonPointsRemove;
  QPushButton*          _pushButtonPointsShow;
  QPushButton*          _pushButtonPointsHide;

  QLabel*               _labelEdges;
  QPushButton*          _pushButtonSceneGraphEdgesAdd;
  QPushButton*          _pushButtonSceneGraphEdgesRemove;
  QPushButton*          _pushButtonSceneGraphEdgesShow;
  QPushButton*          _pushButtonSceneGraphEdgesHide;

  QLabel*               _labelSurface;
  QLabel*               _labelSurfaceVertices;
  QLineEdit*            _editSurfaceVertices;
  QLabel*               _labelSurfaceFaces;
  QLineEdit*            _editSurfaceFaces;
  QPushButton*          _pushButtonSurfaceRemove;
  QPushButton*          _pushButtonSurfaceShow;
  QPushButton*          _pushButtonSurfaceHide;

  QLabel*               _labelHeuristicReconstruction;

  QPushButton*          _pushButtonSurfaceMultiplePlanesFit;
  QPushButton*          _pushButtonSurfaceContinuousFit;
  QPushButton*          _pushButtonSurfaceWatertightFit;

  QLabel*               _labelOptimalReconstruction;

  QPushButton*          _pushButtonOptimalCGHard;
  QPushButton*          _pushButtonOptimalCGSoft;
  QPushButton*          _pushButtonOptimalJacobi;

  QLabel*               _labelMultigridReconstruction;

  QPushButton*          _pushButtonMultigridFiner;
  QPushButton*          _pushButtonMultigridCoarser;
  QPushButton*          _pushButtonMultigridRun;

};

#endif // _WRL_TOOLSWIDGET_HPP_
