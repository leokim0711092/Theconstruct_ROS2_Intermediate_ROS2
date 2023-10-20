#include "rclcpp/rclcpp.hpp"
#include "rmw/types.h"
#include "std_msgs/msg/string.hpp"
#include <chrono>
#include <math.h>

using std::placeholders::_1;

class SubscriberQos : public rclcpp::Node {
public:
  SubscriberQos(int &argc, char **argv) : Node("subscriber_qos_obj") {

    liveliness_lease_duration = std::stoi(argv[2]);
    rclcpp::QoS qos_profile_subscriber(1);
    qos_profile_subscriber.liveliness_lease_duration(
        std::chrono::milliseconds(liveliness_lease_duration));

    liveliness_policy = argv[4];
    if (liveliness_policy == "MANUAL_BY_TOPIC") {
      qos_profile_subscriber.liveliness(
          RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC);
    } else {
      qos_profile_subscriber.liveliness(RMW_QOS_POLICY_LIVELINESS_AUTOMATIC);
    }

    rclcpp::SubscriptionOptions subscription_options;
    subscription_options.event_callbacks.incompatible_qos_callback =
        std::bind(&SubscriberQos::incompatible_qos_info_callback, this, _1);
    subscription_options.event_callbacks.liveliness_callback =
        std::bind(&SubscriberQos::incompatible_liveliness_callback, this, _1);

    subscription_ = this->create_subscription<std_msgs::msg::String>(
        "qos_test", qos_profile_subscriber,
        std::bind(&SubscriberQos::listener_callback, this, _1),
        subscription_options);
  }

private:
  void
  incompatible_liveliness_callback(rclcpp::QOSLivelinessChangedInfo &event) {
    RCLCPP_ERROR(this->get_logger(), "SUBSCRIBER::: Liveliness Triggered!");
    RCLCPP_ERROR(this->get_logger(), "alive_count = %d", event.alive_count);
    RCLCPP_ERROR(this->get_logger(), "not_alive_count = %d",
                 event.not_alive_count);
    RCLCPP_ERROR(this->get_logger(), "alive_count_change = %d",
                 event.alive_count_change);
    RCLCPP_ERROR(this->get_logger(), "not_alive_count_change = %d",
                 event.not_alive_count_change);
    RCLCPP_ERROR(this->get_logger(), "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
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
  int liveliness_lease_duration;
  std::string liveliness_policy;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SubscriberQos>(argc, argv));
  rclcpp::shutdown();
  return 0;
}