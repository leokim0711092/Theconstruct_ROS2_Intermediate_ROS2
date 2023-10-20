#include <cmath>
#include <rclcpp/rclcpp.hpp>
#include <chrono>
#include <functional>
#include <geometry_msgs/msg/twist.hpp>

using namespace std::chrono_literals;

class VelParam: public rclcpp::Node
{
  public:
    VelParam()
      : Node("param_vel_node")
    {
      auto param_desc = rcl_interfaces::msg::ParameterDescriptor{};
      param_desc.description = "Sets the velocity (in m/s) of the robot.";
      this->declare_parameter<std::double_t>("velocity", 0.0, param_desc);
      timer_ = this->create_wall_timer(
      1000ms, std::bind(&VelParam::timer_callback, this));
      publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    }
    void timer_callback()
    {
      this->get_parameter("velocity", vel_parameter_);
      RCLCPP_INFO(this->get_logger(), "Velocity parameter is: %f", vel_parameter_);
      auto message = geometry_msgs::msg::Twist();
      message.linear.x = vel_parameter_;
      publisher_->publish(message);
    }
  private:
    std::double_t vel_parameter_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
};

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<VelParam>());
  rclcpp::shutdown();
  return 0;
}