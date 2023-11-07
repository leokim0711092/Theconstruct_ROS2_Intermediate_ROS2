#ifndef COMPOSITION__CLIENT_COMPONENT_HPP_
#define COMPOSITION__CLIENT_COMPONENT_HPP_

#include "my_components/visibility_control.h"
#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/empty.hpp"

namespace my_components {

class Client : public rclcpp::Node {
public:
  COMPOSITION_PUBLIC
  explicit Client(const rclcpp::NodeOptions &options);

protected:
  void on_timer();

private:
  rclcpp::Client<std_srvs::srv::Empty>::SharedPtr client_;
  rclcpp::TimerBase::SharedPtr timer_;
};

} // namespace composition

#endif // COMPOSITION__CLIENT_COMPONENT_HPP_