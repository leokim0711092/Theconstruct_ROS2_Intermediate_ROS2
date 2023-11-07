cmake_minimum_required(VERSION 3.8)
project(my_components)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(composition REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

include_directories(include)

add_library(moverobot_component SHARED src/moverobot_component.cpp)
target_compile_definitions(moverobot_component PRIVATE "COMPOSITION_BUILDING_DLL")
ament_target_dependencies(moverobot_component
  "rclcpp"
  "rclcpp_components"
  "geometry_msgs")
rclcpp_components_register_nodes(moverobot_component "my_components::MoveRobot")
set(node_plugins "${node_plugins}my_components::MoveRobot;$<TARGET_FILE:moverobot_component>\n")

add_library(readodom_component SHARED src/readodom_component.cpp)
target_compile_definitions(readodom_component PRIVATE "COMPOSITION_BUILDING_DLL")
ament_target_dependencies(readodom_component
  "rclcpp"
  "rclcpp_components"
  "nav_msgs")
rclcpp_components_register_nodes(readodom_component "my_components::OdomSubscriber")
set(node_plugins "${node_plugins}my_components::OdomSubscriber;$<TARGET_FILE:readodom_component>\n")


add_executable(manual_composition
  src/manual_composition.cpp)
target_link_libraries(manual_composition
  moverobot_component
  readodom_component)
ament_target_dependencies(manual_composition
  "rclcpp")

install(TARGETS
  manual_composition

  DESTINATION lib/${PROJECT_NAME})

install(TARGETS
  moverobot_component
  readodom_component

  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

# Install launch files.
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
