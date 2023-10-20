#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <chrono>

using std::placeholders::_1;

class SubscriberQos : public rclcpp::Node {
public:
  SubscriberQos(int &argc, char **argv) : Node("subscriber_qos_obj") {

    deadline = std::stof(argv[2]);
    deadline_ms = deadline * 1000;
    rclcpp::QoS qos_profile_subscriber(1);
    qos_profile_subscriber.deadline(std::chrono::milliseconds(deadline_ms));

    rclcpp::SubscriptionOptions subscription_options;
    subscription_options.event_callbacks.incompatible_qos_callback =
        std::bind(&SubscriberQos::incompatible_qos_info_callback, this, _1);
    subscription_options.event_callbacks.deadline_callback =
        std::bind(&SubscriberQos::incompatible_deadline_callback, this);

    subscription_ = this->create_subscription<std_msgs::msg::String>(
        "qos_test", qos_profile_subscriber,
        std::bind(&SubscriberQos::listener_callback, this, _1),
        subscription_options);
  }

private:
  void incompatible_deadline_callback() {
    RCLCPP_ERROR(this->get_logger(), "PUBLISHER:::  Deadline Triggered!");
  }

  void incompatible_qos_info_callback(
      rclcpp::QOSRequestedIncompatibleQoSInfo &event) {
    RCLCPP_ERROR(this->get_logger(),
                 "SUBSCRIBER::: INCOMPATIBLE QoS Triggered!");
    RCLCPP_INFO(
        this->get_logger(),
        "Requested incompatible qos - total %d delta %d last_policy_kind: %d",
        event.total_count, event.total_count_change, event.last_policy_kind);
  }

  void listener_callback(const std_msgs::msg::String::SharedPtr msg) {
    RCLCPP_INFO(this->get_logger(), "Data Received = '%s'", msg->data.c_str());
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  float deadline;
  int deadline_ms;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SubscriberQos>(argc, argv));
  rclcpp::shutdown();
  return 0;
}