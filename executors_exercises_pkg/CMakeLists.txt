cmake_minimum_required(VERSION 3.5)
project(executors_exercises_pkg)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter, which checks for copyrights
  # uncomment the line when copyright and license are not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set (ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

# Build 
add_executable(executor_example_1_node src/executor_example_1.cpp)
ament_target_dependencies(executor_example_1_node rclcpp)
add_executable(executor_example_2_node src/executor_example_2.cpp)
ament_target_dependencies(executor_example_2_node rclcpp nav_msgs)
add_executable(executor_example_3_node src/executor_example_3.cpp)
ament_target_dependencies(executor_example_3_node rclcpp nav_msgs)
add_executable(executor_example_4_node src/executor_example_4.cpp)
ament_target_dependencies(executor_example_4_node rclcpp nav_msgs)
add_executable(executor_example_4_singlethreaded_node src/executor_example_4_singlethreaded.cpp)
ament_target_dependencies(executor_example_4_singlethreaded_node rclcpp nav_msgs)

# Install
install(TARGETS
   executor_example_1_node
   executor_example_2_node
   executor_example_3_node
   executor_example_4_node
   executor_example_4_singlethreaded_node

   DESTINATION lib/${PROJECT_NAME}
 )

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)