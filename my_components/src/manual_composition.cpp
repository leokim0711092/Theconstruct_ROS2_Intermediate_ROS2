#include <memory>

#include "my_components/moverobot_component.hpp"
#include "my_components/readodom_component.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char *argv[]) {
  // Force flush of the stdout buffer.
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  rclcpp::init(argc, argv);

  rclcpp::executors::SingleThreadedExecutor exec;
  rclcpp::NodeOptions options;

  // Add some nodes to the executor which provide work for the executor during
  // its "spin" function. An example of available work is executing a
  // subscription callback, or a timer callback.
  auto moverobot = std::make_shared<my_components::MoveRobot>(options);
  exec.add_node(moverobot);
  auto readodom = std::make_shared<my_components::OdomSubscriber>(options);
  exec.add_node(readodom);

  // spin will block until work comes in, execute work as it becomes available,
  // and keep blocking. It will only be interrupted by Ctrl-C.
  exec.spin();

  rclcpp::shutdown();

  return 0;
}