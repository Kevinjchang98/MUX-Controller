//
// Created by Kevin Chang on 2/4/23.
//

#ifndef MUX_CONTROLLER__MAINWINDOW_H_
#define MUX_CONTROLLER__MAINWINDOW_H_

#include <QWidget>
#include <QLabel>
#ifdef __WIN32__
#include <winsock2.h>
#endif
#ifdef __MACH__
#include <arpa/inet.h>
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget {
 Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

 private:
  Ui::MainWindow *ui;

  static constexpr int window_width = 350;
  static constexpr int window_height = 500;
  static constexpr int window_margin = 10;
  static constexpr int item_offset_vertical = 40;
  static constexpr int item_height = 30;

  sockaddr_in sendSockAddr{};
  unsigned long long clientSd{};

  QLabel *statusText;

  QString host = "192.168.150.25";
  int port = 5000;
  bool isConnected = false;

  int from = 1;
  int to = 1;
  char routeString[11] = "MT00SW00NT";

  auto connect() -> bool;
  auto setRoute() -> bool;
  void connectionForm();
  void routeForm();
  void statusBar();
};

#endif //MUX_CONTROLLER__MAINWINDOW_H_
