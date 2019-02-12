
#ifndef _WRL_MAINWINDOW_HPP_
#define _WRL_MAINWINDOW_HPP_

#include <string>

#include <QMainWindow>
#include <QTimer>
#include <QGridLayout>

#include "io/AppLoader.hpp"
#include "io/AppSaver.hpp"
#include "WrlGLWidget.hpp"
#include "WrlToolsWidget.hpp"

QT_FORWARD_DECLARE_CLASS(QOpenGLWidget)

class WrlMainWindow : public QMainWindow {

  Q_OBJECT

public:

  WrlMainWindow();

  void timerStop()  { _timer->stop(); }
  void timerStart() { _timer->start(_timerInterval); }
  void showStatusBarMessage(const QString & message);

  int  getGLWidgetWidth();
  int  getGLWidgetHeight();
  void setGLWidgetSize(int width, int height);

  WrlViewerData& getData() const;
  SceneGraph*    getSceneGraph();
  void           setSceneGraph(SceneGraph* pWrl, bool resetHomeView);

  SceneGraph*    loadSceneGraph(const char* fname);

  void updateState();
  void refresh();

private slots:

  void onMenuFileExit();
  void onMenuFileLoad();
  void onMenuFileSave();
  void onMenuFileLoadBunny();
  void onMenuToolsShow();
  void onMenuToolsHide();
  void onMenuHelpAbout();

protected:

  virtual void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;
  void _resize(int width, int height);

private:

  AppLoader       _loader;
  AppSaver        _saver;
  WrlGLWidget    *_glWidget;
  WrlToolsWidget *_toolsWidget;
  QWidget*        _centralWidget; 
  QStatusBar     *_statusBar;
  QTimer         *_timer;

  int             _bdryTop;
  int             _bdryBottom;;
  int             _bdryLeft;
  int             _bdryCenter;
  int             _bdryRight;
  int             _toolsWidth;

  static int      _timerInterval;

};

#endif // _WRL_MAINWINDOW_HPP_
