//
// Created by Kevin Chang on 2/4/23.
//

#ifndef MUX_CONTROLLER__MAINWINDOW_H_
#define MUX_CONTROLLER__MAINWINDOW_H_

#include <QWidget>

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

  static constexpr int window_width = 400;
  static constexpr int window_height = 400;
  static constexpr int window_margin = 10;
  static constexpr int item_offset_vertical = 40;
  static constexpr int item_height = 30;

  QString host = "192.168.150.25";
  int port = 5000;

  int from = 1;
  int to = 1;

  void connectionHandler();
  void setRoute();
};

#endif //MUX_CONTROLLER__MAINWINDOW_H_
