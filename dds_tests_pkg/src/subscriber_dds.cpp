#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;

class SubscriberDDS : public rclcpp::Node {
public:
  SubscriberDDS() : Node("simple_subscriber") {
    rclcpp::QoS qos_profile(10);
    qos_profile.reliability(RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT);
    subscription_ = this->create_subscription<std_msgs::msg::String>(
        "dds_test", qos_profile,
        std::bind(&SubscriberDDS::listener_callback, this, _1));
  }

private:
  void listener_callback(const std_msgs::msg::String::SharedPtr msg) {
    process_dds_test_data(msg->data);
  }

  void process_dds_test_data(std::string data) {
    // The data has the format ID:Seconds,NanoSeconds
    // return:
    // ID
    // TIME Object
    delimiter = ":";
    last = 0;
    next = 0;
    while ((next = data.find(delimiter, last)) != std::string::npos) {
      id = std::stoi(data.substr(last, next - last));
      last = next + 1;
    }
    time = data.substr(last);

    if (id == 0) {
      id_next = 0;
    }
    if (id == id_next) {
      RCLCPP_WARN(this->get_logger(),
                  "MESSAGE OK: ID=%d, Next ID=%d, TOTAL MSG LOST=%d", id,
                  id_next, lost_messages_counter);
    } else {
      if (id_next != -1) {
        delta_messages_lost = id - id_next;
        lost_messages_counter = lost_messages_counter + delta_messages_lost;
        RCLCPP_ERROR(this->get_logger(),
                     "MESSAGE LOST: ID=%d, Next ID=%d, DELTA MSG LOST=%d, "
                     "TOTAL MSG LOST=%d",
                     id, id_next, delta_messages_lost, lost_messages_counter);
      } else {
      }
    }

    id_next = id + 1;
    delimiter = ",";
    last = 0;
    next = 0;
    while ((next = time.find(delimiter, last)) != std::string::npos) {
      seconds = std::stod(time.substr(last, next - last));
      last = next + 1;
    }
    nanoseconds = std::stod(time.substr(last));

    rclcpp::Time time_obj(seconds, nanoseconds);
    rclcpp::Time time_now_obj = this->now();
    delta = time_obj.nanoseconds() - time_now_obj.nanoseconds();
    RCLCPP_INFO(this->get_logger(),
                "DELTA Between PUB and SUB = %lf nanoseconds", delta);
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  int id;
  int id_next;
  int lost_messages_counter;
  int delta_messages_lost;
  std::string time;
  std::string delimiter;
  double seconds;
  double nanoseconds;
  size_t last;
  size_t next;
  double delta;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SubscriberDDS>());
  rclcpp::shutdown();
  return 0;
}