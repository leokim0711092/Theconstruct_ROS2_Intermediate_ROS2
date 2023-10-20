#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp/wait_result_kind.hpp"
#include "std_msgs/msg/string.hpp"

// To use 3s values, for example,
using namespace std::chrono_literals;

int32_t main(const int32_t argc, char **const argv) {
  rclcpp::init(argc, argv);

  auto node = std::make_shared<rclcpp::Node>("wait_set_listener");
  auto do_nothing = [](std_msgs::msg::String::UniquePtr) { assert(false); };

  auto sub1 = node->create_subscription<std_msgs::msg::String>(
      "/reached_goal/box_bot_1", 1, do_nothing);
  auto sub2 = node->create_subscription<std_msgs::msg::String>(
      "/reached_goal/box_bot_2", 1, do_nothing);
  auto sub3 = node->create_subscription<std_msgs::msg::String>(
      "/reached_goal/box_bot_3", 1, do_nothing);

  auto guard_condition1 = std::make_shared<rclcpp::GuardCondition>();

  rclcpp::WaitSet wait_set({}, {});

  wait_set.add_subscription(sub1);
  wait_set.add_subscription(sub2);
  wait_set.add_subscription(sub3);
  wait_set.add_guard_condition(guard_condition1);

  int timeout_counter = 0;

  while (rclcpp::ok()) {
    const auto wait_result = wait_set.wait(3s);
    if (wait_result.kind() == rclcpp::WaitResultKind::Ready) {
      bool sub1_has_data =
          wait_result.get_wait_set().get_rcl_wait_set().subscriptions[0U];
      bool sub2_has_data =
          wait_result.get_wait_set().get_rcl_wait_set().subscriptions[1U];
      bool sub3_has_data =
          wait_result.get_wait_set().get_rcl_wait_set().subscriptions[2U];

      size_t guard_index = 0;
      bool condition1_triggered = wait_result.get_wait_set()
                                      .get_rcl_wait_set()
                                      .guard_conditions[guard_index];

      if (condition1_triggered) {
        RCLCPP_INFO(node->get_logger(),
                    "TOO MANY TIMEOUTS CONDITION TRIGGERED...ENDING NODE");
        break;
      }

      if (sub1_has_data) {
        std_msgs::msg::String topic_msg;
        rclcpp::MessageInfo msg_info;

        sub1->take(topic_msg, msg_info);
        RCLCPP_INFO(node->get_logger(), "I heard: '%s'",
                    topic_msg.data.c_str());
      }

      if (sub2_has_data) {
        std_msgs::msg::String topic_msg;
        rclcpp::MessageInfo msg_info;

        sub2->take(topic_msg, msg_info);
        RCLCPP_INFO(node->get_logger(), "I heard: '%s'",
                    topic_msg.data.c_str());
      }

      if (sub3_has_data) {
        std_msgs::msg::String topic_msg;
        rclcpp::MessageInfo msg_info;

        sub3->take(topic_msg, msg_info);
        RCLCPP_INFO(node->get_logger(), "I heard: '%s'",
                    topic_msg.data.c_str());
      }

    } else if (wait_result.kind() == rclcpp::WaitResultKind::Timeout) {
      if (rclcpp::ok()) {
        RCLCPP_ERROR(node->get_logger(), "Wait-set failed with timeout");
        timeout_counter += 1;
        if (timeout_counter > 3) {
          RCLCPP_INFO(node->get_logger(), "Action: Trigger Guard condition 1");
          guard_condition1->trigger();
        }
      }
    }
  }

  rclcpp::shutdown();
  return 0;
}