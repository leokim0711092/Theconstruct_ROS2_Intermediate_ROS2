#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <chrono>
#include <thread>

using namespace std::chrono_literals;
using std::placeholders::_1;

class PublisherQoS : public rclcpp::Node {
public:
  PublisherQoS(int &argc, char **argv) : Node("publisher_qos_obj") {

    deadline = std::stof(argv[2]);
    deadline_ms = deadline * 1000;
    rclcpp::QoS qos_profile_publisher(1);
    qos_profile_publisher.deadline(std::chrono::milliseconds(deadline_ms));

    rclcpp::PublisherOptions publisher_options;
    publisher_options.event_callbacks.incompatible_qos_callback =
        std::bind(&PublisherQoS::incompatible_qos_info_callback, this, _1);
    publisher_options.event_callbacks.deadline_callback =
        std::bind(&PublisherQoS::incompatible_deadline_callback, this);

    publisher_ = this->create_publisher<std_msgs::msg::String>(
        "/qos_test", qos_profile_publisher, publisher_options);

    timer_ = this->create_wall_timer(
        1000ms, std::bind(&PublisherQoS::timer_callback, this));

    msgs_id = 0;
    timer_period = 1.0;
    swap_state_time = 5.0;
    time_pause = 2.0;
    counter = 0;
  }

  void incompatible_deadline_callback() {
    RCLCPP_ERROR(this->get_logger(), "PUBLISHER:::  Deadline Triggered!");
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
    auto current_time = this->now();
    auto current_time_s = current_time.seconds();
    auto current_time_ns = current_time.nanoseconds();
    time_str =
        std::to_string(current_time_s) + "," + std::to_string(current_time_ns);
    dds_msg_str = std::to_string(msgs_id) + ":" + time_str;
    msg.data = dds_msg_str;
    publisher_->publish(msg);
    RCLCPP_INFO(this->get_logger(), "Publishing: %s ", msg.data.c_str());
  }

  void timer_callback() {
    if (counter > int(swap_state_time / timer_period)) {
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

private:
  float deadline;
  float timer_period;
  float swap_state_time;
  float time_pause;
  float delta;
  int counter;
  int range_steps;
  int delta_ms;
  int deadline_ms;
  int msgs_id;
  std::string time_str;
  std::string dds_msg_str;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PublisherQoS>(argc, argv));
  rclcpp::shutdown();
  return 0;
}