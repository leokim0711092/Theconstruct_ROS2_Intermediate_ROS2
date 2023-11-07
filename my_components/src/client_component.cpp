#include "my_components/client_component.hpp"

#include <cinttypes>
#include <iostream>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/empty.hpp"

using namespace std::chrono_literals;
using Empty = std_srvs::srv::Empty;
using ServiceResponseFuture =
    rclcpp::Client<std_srvs::srv::Empty>::SharedFuture;

namespace my_components {

Client::Client(const rclcpp::NodeOptions &options)
    : Node("move_circle_client", options) {
  client_ = create_client<Empty>("move_circle");
  timer_ = create_wall_timer(2s, std::bind(&Client::on_timer, this));
}

void Client::on_timer() {
  if (!client_->wait_for_service(1s)) {
    if (!rclcpp::ok()) {
      RCLCPP_ERROR(this->get_logger(),
                   "Interrupted while waiting for the service. Exiting.");
      return;
    }
    RCLCPP_INFO(this->get_logger(), "Service not available after waiting");
    return;
  }

  auto request = std::make_shared<Empty::Request>();

  auto response_received_callback = [this](ServiceResponseFuture future) {
    auto status = future.wait_for(1s);
    if (status == std::future_status::ready) {
      RCLCPP_INFO(this->get_logger(), "Result: success");
    } else {
      RCLCPP_INFO(this->get_logger(), "Service In-Progress...");
    }
  };
  auto future_result =
      client_->async_send_request(request, response_received_callback);
}

} // namespace my_components

#include "rclcpp_components/register_node_macro.hpp"

RCLCPP_COMPONENTS_REGISTER_NODE(my_components::Client)