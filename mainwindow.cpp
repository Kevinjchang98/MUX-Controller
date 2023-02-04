//
// Created by Kevin Chang on 2/4/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <iostream>
#include "mainwindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // Window size
  setFixedSize(window_width, window_height);

  // Host input box
  auto hostInputBox = new QLineEdit(host, this);
  hostInputBox->setGeometry(window_margin, window_margin, window_width / 2 - 4 * window_margin, 30);

  // Port input box
  auto portInputBox = new QSpinBox(this);
  portInputBox->setGeometry(window_margin + window_width / 2 - 2 * window_margin,
                            window_margin,
                            window_width / 2 - 4 * window_margin,
                            30);
  portInputBox->setMinimum(0);
  portInputBox->setMaximum(65535);
  portInputBox->setValue(5000);

  // Update host and port on value change
  QObject::connect(hostInputBox, &QLineEdit::textEdited, [=](auto newHost){this->host = newHost;});
  QObject::connect(portInputBox, &QSpinBox::valueChanged, [=](auto newPort){this->port = newPort;});

  // Button to connect to MUX
  auto connectButton = new QPushButton("Test", this);
  connectButton->setGeometry(10, 100, 50, 30);

  QObject::connect(connectButton, &QPushButton::pressed, [=](){std::cout << this->host.toStdString() << std::endl;});
}

MainWindow::~MainWindow() {
  delete ui;
}
