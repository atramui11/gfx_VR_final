
// #include <iostream>
#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>
#include "WrlMainWindow.hpp"

int main( int argc, char ** argv ) {
  // cout << "WrlViewerApp::main() {\n";

  QApplication app( argc, argv );

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  if (QCoreApplication::arguments().contains(QStringLiteral("--multisample")))
    format.setSamples(4);

  // format.setRenderableType(QSurfaceFormat::OpenGLES);

  QSurfaceFormat::setDefaultFormat(format);

  // QSurfaceFormat::RenderableType rt = format.renderableType();
  // switch(rt) {
  // case QSurfaceFormat::DefaultRenderableType:
  //   cout << "  QSurfaceFormat::DefaultRenderableType\n";
  //   break;
  // case QSurfaceFormat::OpenGL:
  //   cout << "  QSurfaceFormat::OpenGL\n";
  //   break;
  // case QSurfaceFormat::OpenGLES:
  //   cout << "  QSurfaceFormat::OpenGLES\n";
  //   break;
  // case QSurfaceFormat::OpenVG:
  //   cout << "  QSurfaceFormat::OpenVG\n";
  //   break;
  // }

  // cout << "  creating WrlMainWindow ... \n";

  WrlMainWindow mw;

  // cout << "  resizing ... \n";

  mw.setMinimumSize(500,500);

  // cout << "  showing ... \n";

  mw.show();

  // cout << "  updating geometry ... \n";

  mw.updateGeometry();

  mw.setGLWidgetSize(600,600);


  // cout << "}\n";

  return app.exec();
}
