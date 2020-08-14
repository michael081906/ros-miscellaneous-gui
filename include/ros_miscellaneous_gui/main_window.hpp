#ifndef ros_miscellaneous_gui_MAIN_WINDOW_H
#define ros_miscellaneous_gui_MAIN_WINDOW_H

#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"

namespace ros_miscellaneous_gui {

struct offset_group
{
  double x_offset = 0;
  double y_offset = 0;
  double z_offset = 0;
};

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(int argc, char** argv, QWidget *parent = 0);
	~MainWindow();

	void ReadSettings(); // Load up qt program settings at startup
	void WriteSettings(); // Save qt program settings when closing

	void closeEvent(QCloseEvent *event); // Overloaded function
	void showNoMasterMessage();

public Q_SLOTS:
	void on_actionAbout_triggered();
	void on_button_connect_clicked(bool check );
  void on_button_load_clicked(bool check);
	void on_checkbox_use_environment_stateChanged(int state);

  void slider_x_moved(int value);
  void slider_y_moved(int value);
  void slider_z_moved(int value);
  void spinBox_changed(int index_);

    void updateLoggingView(); // no idea why this can't connect automatically
Q_SIGNALS:
    void load_offest(int index_, double x_off, double y_off, double z_off);

private:
  int    index_private = 0;
  double x_off_private;
  double y_off_private;
  double z_off_private;
  double threshold = 0.00000000001;

  int slider_x_off_position;
  int slider_y_off_position;
  int slider_z_off_position;

  double scale = 0.01/100;
  void set_num_at_label(double x, double y, double z);

	Ui::MainWindowDesign ui;
	QNode qnode;
  std::array<offset_group, 20 > offset_group_;

};

}  // namespace ros_miscellaneous_gui

#endif // ros_miscellaneous_gui_MAIN_WINDOW_H
