#include <nav_msgs/msg/odometry.hpp>
#include <rclcpp/rclcpp.hpp>

class OdomSubscriber : public rclcpp::Node {
public:
  OdomSubscriber(std::string odom_topic_name) : Node("simple_subscriber") {

    subscription_ = this->create_subscription<nav_msgs::msg::Odometry>(
        odom_topic_name, 10,
        std::bind(&OdomSubscriber::topic_callback, this,
                  std::placeholders::_1));
  }

private:
  void topic_callback(const nav_msgs::msg::Odometry::SharedPtr msg) {
    RCLCPP_INFO(this->get_logger(), "Odometry=['%f','%f','%f']",
                msg->pose.pose.position.x, msg->pose.pose.position.y,
                msg->pose.pose.position.z);
  }
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscription_;
};

int main(int argc, char *argv[]) {
  // Some initialization.
  rclcpp::init(argc, argv);

  // Instantiate a node.
  std::shared_ptr<OdomSubscriber> node =
      std::make_shared<OdomSubscriber>("/box_bot_1/odom");

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