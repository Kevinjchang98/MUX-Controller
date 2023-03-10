//
// Created by Kevin Chang on 2/4/23.
//

#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QWidget>

#ifdef __WIN32__
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

#include "mainwindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  connectionForm();
  routeForm();
  statusBar();
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::routeForm() {
  // MUX Connection Label
  auto inputRoutingLabel = new QLabel(this);
  inputRoutingLabel->setText("Display Routing");
  inputRoutingLabel->setGeometry(window_margin,
                                 window_margin + item_offset_vertical * 3,
                                 window_width,
                                 item_height);

  // Input routing boxes
  auto fromInputBox = new QSpinBox(this);
  fromInputBox->setGeometry(window_margin,
                            window_margin + item_offset_vertical * 4,
                            window_width / 4 - 4 * window_margin,
                            item_height);
  fromInputBox->setMinimum(1);
  fromInputBox->setMaximum(4);
  fromInputBox->setValue(1);

  QObject::connect(fromInputBox,
                   &QSpinBox::valueChanged,
                   [this](auto newFrom) { from = newFrom; });

  auto toInputBox = new QSpinBox(this);
  toInputBox->setGeometry(
      window_margin + 2 * (window_width / 4 - 4 * window_margin),
      window_margin + item_offset_vertical * 4,
      window_width / 4 - 4 * window_margin,
      item_height);
  toInputBox->setMinimum(1);
  toInputBox->setMaximum(4);
  toInputBox->setValue(1);

  QObject::connect(toInputBox,
                   &QSpinBox::valueChanged,
                   [this](auto newTo) { to = newTo; });

  // Button to set new route
  auto setRouteButton = new QPushButton("Set", this);
  setRouteButton->setGeometry(
      window_margin + 4 * (window_width / 4 - 4 * window_margin),
      window_margin + item_offset_vertical * 4,
      50,
      item_height);

  // Run setRoute() on button press
  QObject::connect(setRouteButton, &QPushButton::pressed, [this]() {
    if (setRoute())
      statusText->setText(QString::fromStdString(
          "Route updated to " + std::to_string(from) + " -> "
              + std::to_string(to)));
    else
      statusText->setText("Error updating route");
  });

  // Button to get current route
  auto getRouteButton = new QPushButton("Get current route", this);
  getRouteButton->setGeometry(window_margin,
                              window_margin + item_offset_vertical * 5,
                              150,
                              item_height);

  // Run getRoute() on button press
  QObject::connect(getRouteButton, &QPushButton::pressed, [this]() {
    auto res = getRoute();
    if (!res.empty())
      statusText->setText(QString::fromStdString(res));
    else
      statusText->setText("Error getting route");
  });
}

void MainWindow::connectionForm() {
  // Window size
  setFixedSize(window_width, window_height);

  // MUX Connection Label
  auto connectionInputLabel = new QLabel(this);
  connectionInputLabel->setText("MUX Connection");
  connectionInputLabel->setGeometry(window_margin,
                                    window_margin,
                                    window_width,
                                    item_height);

  // Host input box
  auto hostInputBox = new QLineEdit(host, this);
  hostInputBox->setGeometry(window_margin,
                            window_margin + item_offset_vertical * 1,
                            window_width / 2 - 4 * window_margin,
                            item_height);

  // Port input box
  auto portInputBox = new QSpinBox(this);
  portInputBox->setGeometry(
      window_margin + window_width / 2 - 2 * window_margin,
      window_margin + item_offset_vertical * 1,
      window_width / 2 - 4 * window_margin,
      item_height);
  portInputBox->setMinimum(0);
  portInputBox->setMaximum(65535);
  portInputBox->setValue(5000);

  // Update host and port on value change
  QObject::connect(hostInputBox,
                   &QLineEdit::textEdited,
                   [this](auto newHost) { host = newHost; });
  QObject::connect(portInputBox,
                   &QSpinBox::valueChanged,
                   [this](auto newPort) { port = newPort; });

  // Button to connect to MUX
  auto connectButton = new QPushButton("Connect", this);
  connectButton->setGeometry(10,
                             window_margin + item_offset_vertical * 2,
                             80,
                             item_height);

  // Run connect() on button press
  QObject::connect(connectButton, &QPushButton::pressed, [this]() {
    if (connect()) statusText->setText("Connected successfully");
    else statusText->setText("Failed to connect");
  });
}

void MainWindow::statusBar() {
  statusText = new QLabel(this);
  statusText->setText("Not connected");
  statusText->setGeometry(window_margin,
                          window_height - (window_margin + item_height),
                          window_width - window_margin * 2,
                          item_height);
}

auto MainWindow::connect() -> bool {
  sendSockAddr = {};
  sendSockAddr.sin_family = AF_INET;
  sendSockAddr.sin_addr.s_addr = inet_addr(host.toStdString().c_str());
  sendSockAddr.sin_port = htons(port);
  muxSocket = socket(AF_INET, SOCK_STREAM, 0);

  // Run WSAStartup for Winsock
#ifdef _WIN32
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

  int attemptNumber = 0;

  while (::connect(muxSocket, (sockaddr *) &sendSockAddr, sizeof(sendSockAddr))
      < 0) {
    muxSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (++attemptNumber == 11)
      return false;
  }

  isConnected = true;
  return true;
}

auto MainWindow::setRoute() const -> bool {
  if (!isConnected) return false;

  char routeString[13] = "MT00SW0000NT";
  routeString[7] = this->from + '0';
  routeString[9] = this->to + '0';

  send(muxSocket, routeString, 12, 0);

  return true;
}

auto MainWindow::getRoute() const -> std::string {
  if (!isConnected) return "";

  char buffer[32] = "";
  std::string res;

  send(muxSocket, "MT00RD0000NT", 12, 0);

  // Receive until we've gotten all 28 bytes
  int n = 0;
  while (res.size() < 28) {
    n = recv(muxSocket, buffer, sizeof(buffer), 0);
    res.append(buffer, n);
  }

  return std::string() + "Current routing: " + res[8] + "->" + res[6] + "  "
      + res[13] + "->" + res[11] + "  " + res[18] + "->" + res[16] + "  "
      + res[23] + "->" + res[21];
}
