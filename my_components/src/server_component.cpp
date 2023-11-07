#include "my_components/server_component.hpp"

#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/empty.hpp"

#include <memory>

using Empty = std_srvs::srv::Empty;
using std::placeholders::_1;
using std::placeholders::_2;

namespace my_components {

Server::Server(const rclcpp::NodeOptions &options)
    : Node("move_circle_service", options) {
  auto moving_callback =
      [this](const std::shared_ptr<Empty::Request> request,
             std::shared_ptr<Empty::Response> response) -> void {
    auto message = geometry_msgs::msg::Twist();
    message.linear.x = 0.2;
    message.angular.z = 0.2;
    publisher_->publish(message);
  };

  publisher_ = create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
  srv_ = create_service<Empty>("move_circle", moving_callback);
}

} // namespace my_components

#include "rclcpp_components/register_node_macro.hpp"

RCLCPP_COMPONENTS_REGISTER_NODE(my_components::Server)