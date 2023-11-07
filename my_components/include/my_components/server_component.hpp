#ifndef COMPOSITION__SERVER_COMPONENT_HPP_
#define COMPOSITION__SERVER_COMPONENT_HPP_

#include "geometry_msgs/msg/twist.hpp"
#include "my_components/visibility_control.h"
#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/empty.hpp"

#include <memory>

namespace my_components {

class Server : public rclcpp::Node {
public:
  COMPOSITION_PUBLIC
  explicit Server(const rclcpp::NodeOptions &options);

private:
  rclcpp::Service<std_srvs::srv::Empty>::SharedPtr srv_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
};

} // namespace my_components

#endif // COMPOSITION__SERVER_COMPONENT_HPP_