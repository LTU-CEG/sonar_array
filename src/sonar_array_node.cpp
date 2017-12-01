//          Copyright Emil Fresk 2015-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <ros/ros.h>
#include <sensor_msgs/Range.h>
#include <serialpipe.hpp>
#include <string>
#include <algorithm>

ros::Publisher sonar_pub;

void serial_callback(const std::vector< uint8_t > &data)
{
  std::string str(data.begin(), data.end());

  if (str.back() == '\n' && std::count(str.begin(), str.end(), ',') == 1)
  {
    str.pop_back();

    auto comma_pos = str.find_first_of(',');
    if (comma_pos > str.size())
      return;

    auto id = str.substr(0, comma_pos);
    auto dist = std::stoi(str.substr(comma_pos+1));

    if (dist == 0)
      return;

    sensor_msgs::Range msg;

    msg.header.stamp = ros::Time::now();
    msg.header.frame_id = id;
    msg.radiation_type = sensor_msgs::Range::ULTRASOUND;
    msg.field_of_view = 30 / 2 * 3.1415 / 180;
    msg.min_range = 0.01;
    msg.max_range = 5;
    msg.range = static_cast<double>(dist) / 1000.0;

    sonar_pub.publish(msg);
  }
}

int main(int argc, char *argv[])
{
  /*
   * Initializing ROS
   */
  ROS_INFO("Initializing Sonar Array...");
  ros::init(argc, argv, "sonar_array");

  ros::NodeHandle nh("~");

  std::string port;
  if (!nh.getParam("port", port))
  {
    ROS_WARN("No serial port defined defaulting to /dev/ttyUSB0.");
    port = "/dev/ttyUSB0";
  }

  int baud;
  if (!nh.getParam("baudrate", baud))
  {
    ROS_INFO("No baudrate defined defaulting to 57600.");
    baud = 57600;
  }

  sonar_pub = nh.advertise< sensor_msgs::Range >("sonars", 1);

  ROS_INFO_STREAM("Starting Sonar Array, listening on port " << port << " with baudrate " << baud);

  // Open serial
  serialpipe::bridge sp(port, baud, 50, true);
  sp.registerCallback(serial_callback);
  sp.openPort();

  /* Let ROS run. */
  ros::spin();

  sp.closePort();

  return 0;
}
