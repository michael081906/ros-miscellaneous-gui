#ifndef ros_miscellaneous_gui_QNODE_HPP_
#define ros_miscellaneous_gui_QNODE_HPP_

#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <string>
#include <QThread>
#include <QStringListModel>
#include <std_msgs/Float64MultiArray.h>

namespace ros_miscellaneous_gui {

class QNode : public QThread {
    Q_OBJECT
public:
	QNode(int argc, char** argv );
	virtual ~QNode();
	bool init();
	bool init(const std::string &master_url, const std::string &host_url);
	void run();

	enum LogLevel {
	         Debug,
	         Info,
	         Warn,
	         Error,
	         Fatal
	 };

	QStringListModel* loggingModel() { return &logging_model; }
	void log( const LogLevel &level, const std::string &msg);

Q_SIGNALS:
    void loggingUpdated();
    void rosShutdown();

public Q_SLOTS:
    void Load_point(int point_, double x_off, double y_off, double z_off);


private:
	int init_argc;
	char** init_argv;
	ros::Publisher chatter_publisher;
    QStringListModel logging_model;
  ros::Publisher point_offset_publisher;
  std_msgs::Float64MultiArray offset;

};

}  // namespace ros_miscellaneous_gui

#endif /* ros_miscellaneous_gui_QNODE_HPP_ */
