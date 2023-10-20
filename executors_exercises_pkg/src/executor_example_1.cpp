#include <rclcpp/rclcpp.hpp>

int main(int argc, char *argv[]) {
  // Some initialization.
  rclcpp::init(argc, argv);

  // Instantiate a node.
  rclcpp::Node::SharedPtr node =
      rclcpp::Node::make_shared("executor_example_1_node");

  // This is the same as a print in ROS
  RCLCPP_INFO(node->get_logger(), "Bacon pancakes, makin bacon pancakes");

  // Start and spin executor SingleThreadded
  //   rclcpp::spin(node);

  // Same code, but in steps
  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node);
  executor.spin();

  // Shut down and exit.
  rclcpp::shutdown();
  return 0;
}