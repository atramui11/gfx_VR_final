
#include "WrlAboutDialog.hpp"

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>

WrlAboutDialog::WrlAboutDialog(QWidget * parent, Qt::WindowFlags flags): 
  QDialog(parent, flags) {

  if (objectName().isEmpty())
    setObjectName(QStringLiteral("AboutDialog"));

  setWindowModality(Qt::ApplicationModal);
  QVBoxLayout* verticalLayout = new QVBoxLayout(this);
  verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

  QLabel* title_label = new QLabel(this);
  title_label->setObjectName(QStringLiteral("title_label"));
  QFont font;
  font.setPointSize(16);
  title_label->setFont(font);
  title_label->setAlignment(Qt::AlignCenter);

  verticalLayout->addWidget(title_label);

  QFormLayout* formLayout = new QFormLayout();
  formLayout->setObjectName(QStringLiteral("formLayout"));
  QLabel* qt_version_title_label = new QLabel(this);
  qt_version_title_label->setObjectName(QStringLiteral("qt_version_title_label"));
  QFont font1;
  font1.setPointSize(10);
  qt_version_title_label->setFont(font1);

  formLayout->setWidget(1, QFormLayout::LabelRole, qt_version_title_label);

  QLabel* qt_version_label = new QLabel(this);
  qt_version_label->setObjectName(QStringLiteral("qt_version_label"));
  qt_version_label->setFont(font1);

  formLayout->setWidget(1, QFormLayout::FieldRole, qt_version_label);

  QLabel* built_date_title_label = new QLabel(this);
  built_date_title_label->setObjectName(QStringLiteral("built_date_title_label"));
  built_date_title_label->setFont(font1);

  formLayout->setWidget(2, QFormLayout::LabelRole, built_date_title_label);

  QLabel* built_date_label = new QLabel(this);
  built_date_label->setObjectName(QStringLiteral("built_date_label"));
  built_date_label->setFont(font1);

  formLayout->setWidget(2, QFormLayout::FieldRole, built_date_label);

  verticalLayout->addLayout(formLayout);

  QLabel* author_label = new QLabel(this);
  author_label->setObjectName(QStringLiteral("author_label"));
  author_label->setFont(font1);
  author_label->setAlignment(Qt::AlignCenter);

  verticalLayout->addWidget(author_label);

  QPushButton *close_button = new QPushButton(this);
  close_button->setObjectName(QStringLiteral("close_button"));
  close_button->setFont(font1);

  verticalLayout->addWidget(close_button);

  setWindowTitle
   (QApplication::translate("AboutDialog", "WrlViewer1 2017", 0));
  title_label->setText
   (QApplication::translate("AboutDialog", "WrlViewer1 2017", 0));
  qt_version_title_label->setText
   (QApplication::translate("AboutDialog", "Qt Version:", 0));
  qt_version_label->setText(QApplication::translate
   ("AboutDialog", "qt ver", 0));
  built_date_title_label->setText(QApplication::translate
   ("AboutDialog", "Built on:", 0));
  built_date_label->setText(QApplication::translate
   ("AboutDialog", "build date time", 0));
  author_label->setText(QApplication::translate
   ("AboutDialog", "(c) 2017 Your Name", 0));
  close_button->setText(QApplication::translate
   ("AboutDialog", "Close", 0));

  QMetaObject::connectSlotsByName(this);
  
  qt_version_label->setText(qVersion());
  author_label->setText("NK");
  built_date_label->setText(QString("%1 %2").arg(__DATE__, __TIME__));
}

WrlAboutDialog::~WrlAboutDialog() {
}
