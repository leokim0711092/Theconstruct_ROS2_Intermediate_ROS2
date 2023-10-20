#include "rclcpp/rclcpp.hpp"
#include "rmw/types.h"
#include "std_msgs/msg/string.hpp"
#include <chrono>
#include <thread>

using namespace std::chrono_literals;
using std::placeholders::_1;
using std::placeholders::_2;

class PublisherQoS : public rclcpp::Node {
public:
  PublisherQoS(int &argc, char **argv) : Node("publisher_qos_obj") {

    group_timer_publisher = this->create_callback_group(
        rclcpp::CallbackGroupType::MutuallyExclusive);
    group_alive_timer = this->create_callback_group(
        rclcpp::CallbackGroupType::MutuallyExclusive);
    group_events_clb = this->create_callback_group(
        rclcpp::CallbackGroupType::MutuallyExclusive);

    liveliness_lease_duration = std::stoi(argv[2]);
    rclcpp::QoS qos_profile_publisher(1);
    qos_profile_publisher.liveliness_lease_duration(
        std::chrono::milliseconds(liveliness_lease_duration));

    liveliness_policy = argv[10];
    if (liveliness_policy == "MANUAL_BY_TOPIC") {
      qos_profile_publisher.liveliness(
          RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC);
    } else {
      qos_profile_publisher.liveliness(RMW_QOS_POLICY_LIVELINESS_AUTOMATIC);
    }

    publish_period = std::stoi(argv[4]);
    topic_assert_period = std::stoi(argv[6]);
    publish_assert = argv[8];
    if (publish_assert == "yes") {
      publish_assert_bool = true;
    } else {
      publish_assert_bool = false;
    }

    rclcpp::PublisherOptions publisher_options;
    publisher_options.event_callbacks.incompatible_qos_callback =
        std::bind(&PublisherQoS::incompatible_qos_info_callback, this, _1);
    publisher_options.event_callbacks.liveliness_callback =
        std::bind(&PublisherQoS::incompatible_liveliness_callback, this);
    publisher_options.callback_group = group_events_clb;

    publisher_ = this->create_publisher<std_msgs::msg::String>(
        "/qos_test", qos_profile_publisher, publisher_options);

    swap_state_time = 5.0;
    time_pause = 5.0;
    counter = 0;

    timer1_ = this->create_wall_timer(
        std::chrono::milliseconds(publish_period),
        std::bind(&PublisherQoS::timer_callback, this), group_timer_publisher);
    timer2_ = this->create_wall_timer(
        std::chrono::milliseconds(topic_assert_period),
        std::bind(&PublisherQoS::alive_callback, this), group_alive_timer);
  }

  void incompatible_liveliness_callback() {
    RCLCPP_ERROR(this->get_logger(), "PUBLISHER::: Liveliness Triggered!");
  }

  void
  incompatible_qos_info_callback(rclcpp::QOSOfferedIncompatibleQoSInfo &event) {
    RCLCPP_ERROR(this->get_logger(),
                 "A subscriber is asking for an INCOMPATIBLE QoS Triggered!");
    RCLCPP_ERROR(
        this->get_logger(),
        "Offered incompatible qos - total %d delta %d last_policy_kind: %d",
        event.total_count, event.total_count_change, event.last_policy_kind);
  }

  void publish_one_message() {
    auto msg = std_msgs::msg::String();
    rclcpp::Time current_time = this->now();
    double seconds = current_time.seconds();
    double nanoseconds = current_time.nanoseconds();
    time_str = std::to_string(seconds) + "," + std::to_string(nanoseconds);
    msg.data = time_str;
    publisher_->publish(msg);
    RCLCPP_INFO(this->get_logger(), "Publishing: %s ", msg.data.c_str());
  }

  void timer_callback() {
    if (counter > int(swap_state_time / (publish_period / 1000))) {
      delta = 0.1;
      delta_ms = delta * 1000;
      range_steps = int(time_pause / delta);
      for (int i = 0; i < range_steps; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms));
        RCLCPP_INFO(this->get_logger(), "Paused = %f / %f ", (i * delta),
                    time_pause);
      }
      counter = 0;
    } else {
      publish_one_message();
      ++counter;
      RCLCPP_INFO(this->get_logger(), "Counter = %d", counter);
    }
  }

  void alive_callback() { i_am_alive(); }

  void i_am_alive() {
    if (publish_assert_bool) {
      if (publisher_->assert_liveliness()) {
        RCLCPP_INFO(this->get_logger(), "Publisher Alive...");
      } else {
        RCLCPP_INFO(this->get_logger(), "Publisher Dead...");
      }
    }
  }

private:
  int liveliness_lease_duration;
  std::string liveliness_policy;
  std::string publish_assert;
  bool publish_assert_bool;
  int publish_period;
  int topic_assert_period;
  float swap_state_time;
  float time_pause;
  float delta;
  int counter;
  int range_steps;
  int delta_ms;
  std::string time_str;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer1_;
  rclcpp::TimerBase::SharedPtr timer2_;
  rclcpp::CallbackGroup::SharedPtr group_timer_publisher;
  rclcpp::CallbackGroup::SharedPtr group_alive_timer;
  rclcpp::CallbackGroup::SharedPtr group_events_clb;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  std::shared_ptr<PublisherQoS> publisher_qos =
      std::make_shared<PublisherQoS>(argc, argv);
  rclcpp::executors::MultiThreadedExecutor executor(rclcpp::ExecutorOptions(),
                                                    3);
  executor.add_node(publisher_qos);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}