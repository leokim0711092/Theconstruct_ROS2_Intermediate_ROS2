#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

class MoveRobot : public rclcpp::Node {
public:
  MoveRobot() : Node("move_robot_node") {

    publisher_ =
        this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
    
    timer_ = this->create_wall_timer(
        1000ms, std::bind(&MoveRobot::timer_callback, this));
  }

  void timer_callback() {
    this->timer_->cancel();
    this->main_logic(); 
  }

  void turn() {
    RCLCPP_INFO(this->get_logger(), "TURNING....");
    twist.linear.x = 0.5;
    twist.angular.z = 0.5;
    publisher_->publish(twist);
  }

  void main_logic() {
    turn();
  }

private:
  geometry_msgs::msg::Twist twist;

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MoveRobot>());
  rclcpp::shutdown();
  return 0;
}