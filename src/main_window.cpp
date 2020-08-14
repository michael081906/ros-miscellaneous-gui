#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include <math.h>
#include "../include/ros_miscellaneous_gui/main_window.hpp"

namespace ros_miscellaneous_gui {

using namespace Qt;

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
	: QMainWindow(parent)
	, qnode(argc,argv)
{
	ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.
    QObject::connect(ui.actionAbout_Qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt())); // qApp is a global variable for the application
    ReadSettings();
	setWindowIcon(QIcon(":/images/icon.png"));
  ui.tab_manager->setCurrentIndex(0); // ensure the first tab is showing - qt-designer should have this already hardwired, but often loses it (settings?).
    QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));
    QObject::connect(this, SIGNAL(load_offest(int, double, double, double)), &qnode, SLOT(Load_point(int, double, double, double)));
    QObject::connect(ui.slide_x, SIGNAL(sliderMoved(int)), this, SLOT(slider_x_moved(int)));
    QObject::connect(ui.slide_y, SIGNAL(sliderMoved(int)), this, SLOT(slider_y_moved(int)));
    QObject::connect(ui.slide_z, SIGNAL(sliderMoved(int)), this, SLOT(slider_z_moved(int)));
    QObject::connect(ui.spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBox_changed(int)));

    ui.view_logging->setModel(qnode.loggingModel());
    QObject::connect(&qnode, SIGNAL(loggingUpdated()), this, SLOT(updateLoggingView()));

    if ( ui.checkbox_remember_settings->isChecked() ) {
        on_button_connect_clicked(true);
    }
}

MainWindow::~MainWindow() {}


void MainWindow::showNoMasterMessage() {
	QMessageBox msgBox;
	msgBox.setText("Couldn't find the ros master.");
	msgBox.exec();
    close();
}


void MainWindow::on_button_connect_clicked(bool check ) {
	if ( ui.checkbox_use_environment->isChecked() ) {
		if ( !qnode.init() ) {
			showNoMasterMessage();
		} else {
			ui.button_connect->setEnabled(false);
		}
	} else {
		if ( ! qnode.init(ui.line_edit_master->text().toStdString(),
				   ui.line_edit_host->text().toStdString()) ) {
			showNoMasterMessage();
		} else {
			ui.button_connect->setEnabled(false);
			ui.line_edit_master->setReadOnly(true);
			ui.line_edit_host->setReadOnly(true);
			ui.line_edit_topic->setReadOnly(true);
		}
	}
}


void MainWindow::on_checkbox_use_environment_stateChanged(int state) {
	bool enabled;
	if ( state == 0 ) {
		enabled = true;
	} else {
		enabled = false;
	}
	ui.line_edit_master->setEnabled(enabled);
	ui.line_edit_host->setEnabled(enabled);
	//ui.line_edit_topic->setEnabled(enabled);
}

void MainWindow::updateLoggingView() {
        ui.view_logging->scrollToBottom();
}

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, tr("About ..."),tr("<h2>PACKAGE_NAME Test Program 0.10</h2><p>Copyright Yujin Robot</p><p>This package needs an about description.</p>"));
}

void MainWindow::ReadSettings() {
    QSettings settings("Qt-Ros Package", "ros_miscellaneous_gui");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    QString master_url = settings.value("master_url",QString("http://192.168.1.2:11311/")).toString();
    QString host_url = settings.value("host_url", QString("192.168.1.3")).toString();
    //QString topic_name = settings.value("topic_name", QString("/chatter")).toString();
    ui.line_edit_master->setText(master_url);
    ui.line_edit_host->setText(host_url);
    //ui.line_edit_topic->setText(topic_name);
    bool remember = settings.value("remember_settings", false).toBool();
    ui.checkbox_remember_settings->setChecked(remember);
    bool checked = settings.value("use_environment_variables", false).toBool();
    ui.checkbox_use_environment->setChecked(checked);
    if ( checked ) {
    	ui.line_edit_master->setEnabled(false);
    	ui.line_edit_host->setEnabled(false);
    	//ui.line_edit_topic->setEnabled(false);
    }
}

void MainWindow::WriteSettings() {
    QSettings settings("Qt-Ros Package", "ros_miscellaneous_gui");
    settings.setValue("master_url",ui.line_edit_master->text());
    settings.setValue("host_url",ui.line_edit_host->text());
    //settings.setValue("topic_name",ui.line_edit_topic->text());
    settings.setValue("use_environment_variables",QVariant(ui.checkbox_use_environment->isChecked()));
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("remember_settings",QVariant(ui.checkbox_remember_settings->isChecked()));

}

void MainWindow::closeEvent(QCloseEvent *event)
{
	WriteSettings();
	QMainWindow::closeEvent(event);
}

void MainWindow::on_button_load_clicked(bool check)
{

}

void MainWindow::slider_x_moved(int value)
{
  ROS_INFO_STREAM("value: " << value);
  x_off_private =(value - 100)* scale;
  ROS_INFO_STREAM("x_off_private: " << x_off_private);

  offset_group_[index_private].x_offset = x_off_private;
  Q_EMIT load_offest(index_private,
                     x_off_private,
                     y_off_private,
                     z_off_private);

  set_num_at_label(x_off_private, y_off_private, z_off_private);

}

void MainWindow::slider_y_moved(int value)
{
  ROS_INFO_STREAM("value: " << value);
  y_off_private =(value - 100)* scale;
  ROS_INFO_STREAM("y_off_private: " << y_off_private);
  offset_group_[index_private].y_offset = y_off_private;
  Q_EMIT load_offest(index_private,
                     x_off_private,
                     y_off_private,
                     z_off_private);

  set_num_at_label(x_off_private, y_off_private, z_off_private);
}

void MainWindow::slider_z_moved(int value)
{
  ROS_INFO_STREAM("value: " << value);
  z_off_private =(value - 100)* scale;
  ROS_INFO_STREAM("z_off_private: " << z_off_private);
  offset_group_[index_private].z_offset = z_off_private;
  Q_EMIT load_offest(index_private,
                     x_off_private,
                     y_off_private,
                     z_off_private);
  set_num_at_label(x_off_private, y_off_private, z_off_private);
}

void MainWindow::spinBox_changed(int index_)
{
  std::cout << "you clicked: " << index_ << std::endl;
  index_private = index_;
  x_off_private = offset_group_[index_].x_offset;
  y_off_private = offset_group_[index_].y_offset;
  z_off_private = offset_group_[index_].z_offset;
  ROS_INFO_STREAM("x_off_private: " << x_off_private);
  ROS_INFO_STREAM("y_off_private: " << y_off_private);
  ROS_INFO_STREAM("z_off_private: " << z_off_private);
  // set back to the slider
  slider_x_off_position = x_off_private/scale + 100;
  slider_y_off_position = y_off_private/scale + 100;
  slider_z_off_position = z_off_private/scale + 100;
  ROS_INFO_STREAM("slider_x_off_position: " << slider_x_off_position);
  ROS_INFO_STREAM("slider_y_off_position: " << slider_y_off_position);
  ROS_INFO_STREAM("slider_z_off_position: " << slider_z_off_position);
  ui.slide_x->setSliderPosition(slider_x_off_position);
  ui.slide_y->setSliderPosition(slider_y_off_position);
  ui.slide_z->setSliderPosition(slider_z_off_position);

  Q_EMIT load_offest(index_private,
                     x_off_private,
                     y_off_private,
                     z_off_private);

  set_num_at_label(x_off_private, y_off_private, z_off_private);
}

 void MainWindow::set_num_at_label(double x, double y, double z)
 {
   if(fabs(x) < threshold) ui.value_x->setNum(0.0);
   else              ui.value_x->setNum(x);
   if(fabs(y) < threshold) ui.value_y->setNum(0.0);
   else              ui.value_y->setNum(y);
   if(fabs(z) < threshold) ui.value_z->setNum(0.0);
   else              ui.value_z->setNum(z);
 }


}  // namespace ros_miscellaneous_gui

