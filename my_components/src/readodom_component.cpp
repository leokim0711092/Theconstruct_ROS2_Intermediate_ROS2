#include "my_components/readodom_component.hpp"

#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"

using std::placeholders::_1;

namespace my_components
{
  OdomSubscriber::OdomSubscriber(const rclcpp::NodeOptions & options)
  : Node("odom_subscriber", options)
  {
    auto callback =
    [this](const typename nav_msgs::msg::Odometry::SharedPtr msg) -> void
    {
      // Print X position of the robot
      RCLCPP_INFO(this->get_logger(), "I heard: '%f'", msg->pose.pose.position.x);
      std::flush(std::cout);
    };
    
    subscription_ = create_subscription<nav_msgs::msg::Odometry>(
      "odom", 10, callback);
  }
  
}

#include "rclcpp_components/register_node_macro.hpp"

RCLCPP_COMPONENTS_REGISTER_NODE(my_components::OdomSubscriber)