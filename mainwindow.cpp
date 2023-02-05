//
// Created by Kevin Chang on 2/4/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <iostream>
#include <QWidget>
#ifdef __WIN32__
#include <winsock2.h>
#endif
#ifdef __MACH__
#include <arpa/inet.h>
#endif
#include "mainwindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connectionForm();
  routeForm();
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::routeForm() {
  // MUX Connection Label
  auto inputRoutingLabel = new QLabel(this);
  inputRoutingLabel->setText("Display Routing");
  inputRoutingLabel->setGeometry(window_margin, window_margin + item_offset_vertical * 3, window_width, item_height);

  // Input routing boxes
  auto fromInputBox = new QSpinBox(this);
  fromInputBox->setGeometry(window_margin,
                            window_margin + item_offset_vertical * 4,
                            window_width / 4 - 4 * window_margin,
                            item_height);
  fromInputBox->setMinimum(1);
  fromInputBox->setMaximum(4);
  fromInputBox->setValue(1);

  QObject::connect(fromInputBox, &QSpinBox::valueChanged, [this](auto newFrom) { from = newFrom; });

  auto toInputBox = new QSpinBox(this);
  toInputBox->setGeometry(window_margin + 2 * (window_width / 4 - 4 * window_margin),
                          window_margin + item_offset_vertical * 4,
                          window_width / 4 - 4 * window_margin,
                          item_height);
  toInputBox->setMinimum(1);
  toInputBox->setMaximum(4);
  toInputBox->setValue(1);

  QObject::connect(toInputBox, &QSpinBox::valueChanged, [this](auto newTo) { to = newTo; });

  // Button to set new route
  auto setRouteButton = new QPushButton("Set", this);
  setRouteButton->setGeometry(window_margin + 4 * (window_width / 4 - 4 * window_margin),
                              window_margin + item_offset_vertical * 4,
                              50,
                              item_height);

  // Run setRoute() on button press
  QObject::connect(setRouteButton, &QPushButton::pressed, [this]() { setRoute(); });
}

void MainWindow::connectionForm() {
  // Window size
  setFixedSize(window_width, window_height);

  // MUX Connection Label
  auto connectionInputLabel = new QLabel(this);
  connectionInputLabel->setText("MUX Connection");
  connectionInputLabel->setGeometry(window_margin, window_margin, window_width, item_height);

  // Host input box
  auto hostInputBox = new QLineEdit(host, this);
  hostInputBox->setGeometry(window_margin,
                            window_margin + item_offset_vertical * 1,
                            window_width / 2 - 4 * window_margin,
                            item_height);

  // Port input box
  auto portInputBox = new QSpinBox(this);
  portInputBox->setGeometry(window_margin + window_width / 2 - 2 * window_margin,
                            window_margin + item_offset_vertical * 1,
                            window_width / 2 - 4 * window_margin,
                            item_height);
  portInputBox->setMinimum(0);
  portInputBox->setMaximum(65535);
  portInputBox->setValue(5000);

  // Update host and port on value change
  QObject::connect(hostInputBox, &QLineEdit::textEdited, [this](auto newHost) { host = newHost; });
  QObject::connect(portInputBox, &QSpinBox::valueChanged, [this](auto newPort) { port = newPort; });

  // Button to connect to MUX
  auto connectButton = new QPushButton("Connect", this);
  connectButton->setGeometry(10, window_margin + item_offset_vertical * 2, 80, item_height);

  // Run connect() on button press
  QObject::connect(connectButton, &QPushButton::pressed, [this]() { connect(); });
}

void MainWindow::connect() {
  sendSockAddr = {};
  sendSockAddr.sin_family = AF_INET;
  sendSockAddr.sin_addr.s_addr = inet_addr(host.toStdString().c_str());
  sendSockAddr.sin_port = htons(port);
  clientSd = socket(AF_INET, SOCK_STREAM, 0);

  int attemptNumber = 0;
  while (::connect(clientSd, (sockaddr *) &sendSockAddr, sizeof(sendSockAddr)) < 0 && attemptNumber++ < 10) {
    std::cout << attemptNumber << " - Error connecting to socket" << std::endl;

    clientSd = socket(AF_INET, SOCK_STREAM, 0);
  }
}

void MainWindow::setRoute() {
  // TODO: Confirm constructing message correctly
  routeString[6] = '0' + this->from;
  routeString[7] = '0' + this->to;

  for (char i : routeString)
    std::cout << i;
  std::cout << std::endl;

  send(clientSd, routeString, strlen(routeString) , 0);
}
