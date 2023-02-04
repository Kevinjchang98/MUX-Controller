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

  // MUX Connection Label
  auto connectionInputLabel = new QLabel(this);
  connectionInputLabel->setText("MUX Connection");
  connectionInputLabel->setGeometry(window_margin, window_margin, window_width, 30);

  // Host input box
  auto hostInputBox = new QLineEdit(host, this);
  hostInputBox->setGeometry(window_margin,
                            window_margin + item_offset_vertical * 1,
                            window_width / 2 - 4 * window_margin,
                            30);

  // Port input box
  auto portInputBox = new QSpinBox(this);
  portInputBox->setGeometry(window_margin + window_width / 2 - 2 * window_margin,
                            window_margin + item_offset_vertical * 1,
                            window_width / 2 - 4 * window_margin,
                            30);
  portInputBox->setMinimum(0);
  portInputBox->setMaximum(65535);
  portInputBox->setValue(5000);

  // Update host and port on value change
  QObject::connect(hostInputBox, &QLineEdit::textEdited, [=](auto newHost) { this->host = newHost; });
  QObject::connect(portInputBox, &QSpinBox::valueChanged, [=](auto newPort) { this->port = newPort; });

  // Button to connect to MUX
  auto connectButton = new QPushButton("Connect", this);
  connectButton->setGeometry(10, window_margin + item_offset_vertical * 2, 80, 30);

  // Run connectionHandler() on button press
  QObject::connect(connectButton, &QPushButton::pressed, [=]() { connectionHandler(); });

  // MUX Connection Label
  auto inputRoutingLabel = new QLabel(this);
  inputRoutingLabel->setText("Display Routing");
  inputRoutingLabel->setGeometry(window_margin, window_margin + item_offset_vertical * 3, window_width, 30);

  // Input routing boxes
  auto fromInputBox = new QSpinBox(this);
  fromInputBox->setGeometry(window_margin,
                            window_margin + item_offset_vertical * 4,
                            window_width / 4 - 4 * window_margin,
                            30);
  fromInputBox->setMinimum(1);
  fromInputBox->setMaximum(4);
  fromInputBox->setValue(1);

  QObject::connect(fromInputBox, &QSpinBox::valueChanged, [=](auto newFrom) { this->from = newFrom; });

  auto toInputBox = new QSpinBox(this);
  toInputBox->setGeometry(window_margin + 2 * (window_width / 4 - 4 * window_margin),
                          window_margin + item_offset_vertical * 4,
                          window_width / 4 - 4 * window_margin,
                          30);
  toInputBox->setMinimum(1);
  toInputBox->setMaximum(4);
  toInputBox->setValue(1);

  QObject::connect(toInputBox, &QSpinBox::valueChanged, [=](auto newTo) { this->to = newTo; });

  // Button to set new route
  auto setRouteButton = new QPushButton("Set", this);
  setRouteButton->setGeometry(window_margin + 4 * (window_width / 4 - 4 * window_margin),
                              window_margin + item_offset_vertical * 4,
                              50,
                              30);

  // Run connectionHandler() on button press
  QObject::connect(setRouteButton, &QPushButton::pressed, [=]() { setRoute(); });
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::connectionHandler() {
  std::cout << this->host.toStdString() << std::endl;
}

void MainWindow::setRoute() {
  std::cout << this->from << std::endl;
  std::cout << this->to << std::endl;
}
