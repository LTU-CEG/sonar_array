# A ROS node for parsing a Sonar Array

## Contributors

* [Emil Fresk](https://www.github.com/korken89)

---

## License

Licensed under the Boost Software License 1.0, see LICENSE file for details.

---

## Functionality

Reads a serial stream from [this Arduino sketch](https://github.com/LTU-CEG/sonar_array_sketch) and publishes `sensor_msgs::Range`.
The `frame_id` has the ID of the sonar.
