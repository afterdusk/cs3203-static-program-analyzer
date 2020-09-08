/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include "codeeditor.h"
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow {
public:
  QWidget *centralwidget;
  QVBoxLayout *verticalLayout;
  QGroupBox *groupBox_2;
  QHBoxLayout *horizontalLayout_2;
  CodeEditor *txtCodeEditor;
  QGroupBox *groupBox;
  QHBoxLayout *horizontalLayout;
  QTextEdit *txtQuery;
  QGroupBox *groupBox_3;
  QHBoxLayout *horizontalLayout_3;
  QTextEdit *txtResult;
  QPushButton *btnLoadQuery;
  QPushButton *btnRunQuery;
  QMenuBar *menubar;
  QStatusBar *statusbar;

  void setupUi(QMainWindow *MainWindow) {
    if (MainWindow->objectName().isEmpty())
      MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(852, 751);
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    verticalLayout = new QVBoxLayout(centralwidget);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    groupBox_2 = new QGroupBox(centralwidget);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    horizontalLayout_2 = new QHBoxLayout(groupBox_2);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    txtCodeEditor = new CodeEditor(groupBox_2);
    txtCodeEditor->setObjectName(QString::fromUtf8("txtCodeEditor"));

    horizontalLayout_2->addWidget(txtCodeEditor);

    verticalLayout->addWidget(groupBox_2);

    groupBox = new QGroupBox(centralwidget);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setMaximumSize(QSize(16777215, 80));
    groupBox->setCheckable(false);
    horizontalLayout = new QHBoxLayout(groupBox);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    txtQuery = new QTextEdit(groupBox);
    txtQuery->setObjectName(QString::fromUtf8("txtQuery"));
    txtQuery->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);

    horizontalLayout->addWidget(txtQuery);

    verticalLayout->addWidget(groupBox);

    groupBox_3 = new QGroupBox(centralwidget);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    groupBox_3->setMaximumSize(QSize(16777215, 100));
    horizontalLayout_3 = new QHBoxLayout(groupBox_3);
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    txtResult = new QTextEdit(groupBox_3);
    txtResult->setObjectName(QString::fromUtf8("txtResult"));

    horizontalLayout_3->addWidget(txtResult);

    verticalLayout->addWidget(groupBox_3);

    btnLoadQuery = new QPushButton(centralwidget);
    btnLoadQuery->setObjectName(QString::fromUtf8("btnLoadQuery"));

    verticalLayout->addWidget(btnLoadQuery);

    btnRunQuery = new QPushButton(centralwidget);
    btnRunQuery->setObjectName(QString::fromUtf8("btnRunQuery"));

    verticalLayout->addWidget(btnRunQuery);

    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 852, 21));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindow->setStatusBar(statusbar);

    retranslateUi(MainWindow);
    QObject::connect(btnLoadQuery, SIGNAL(clicked()), MainWindow,
                     SLOT(btnLoad_clicked()));
    QObject::connect(btnRunQuery, SIGNAL(clicked()), MainWindow,
                     SLOT(btnRun_clicked()));

    QMetaObject::connectSlotsByName(MainWindow);
  } // setupUi

  void retranslateUi(QMainWindow *MainWindow) {
    MainWindow->setWindowTitle(
        QCoreApplication::translate("MainWindow", "SPA GUI", nullptr));
    groupBox_2->setTitle(QCoreApplication::translate(
        "MainWindow", "SIMPLE Source Code Editor", nullptr));
    groupBox->setTitle(QCoreApplication::translate(
        "MainWindow", "PQL Query - input one query at a time", nullptr));
    groupBox_3->setTitle(
        QCoreApplication::translate("MainWindow", "Query Results", nullptr));
    btnLoadQuery->setText(QCoreApplication::translate(
        "MainWindow", "Load SIMPLE source code from file", nullptr));
    btnRunQuery->setText(QCoreApplication::translate(
        "MainWindow", "Evaluate PQL qeury", nullptr));
  } // retranslateUi
};

namespace Ui {
class MainWindow : public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
