
#ifndef _WRL_ABOUTDIALOG_HPP_
#define _WRL_ABOUTDIALOG_HPP_

#include <QDialog>

class WrlAboutDialog : public QDialog {

  Q_OBJECT

public:

  WrlAboutDialog(QWidget * parent = 0, Qt::WindowFlags flags = 0);
  ~WrlAboutDialog();

public slots:

  void on_close_button_clicked() { accept(); }

};


#endif  // _WRL_ABOUTDIALOG_HPP_
