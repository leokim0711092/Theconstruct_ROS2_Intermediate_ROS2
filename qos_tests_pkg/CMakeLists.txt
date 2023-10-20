cmake_minimum_required(VERSION 3.8)
project(qos_tests_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(publisher_custom_minimal_qos_exe src/publisher_custom_minimal_qos.cpp)
ament_target_dependencies(publisher_custom_minimal_qos_exe rclcpp)
add_executable(subscriber_custom_minimal_qos_exe src/subscriber_custom_minimal_qos.cpp)
ament_target_dependencies(subscriber_custom_minimal_qos_exe rclcpp)

add_executable(subscriber_durability_exe src/subscriber_durability.cpp)
ament_target_dependencies(subscriber_durability_exe rclcpp)
add_executable(publisher_durability_exe src/publisher_durability.cpp)
ament_target_dependencies(publisher_durability_exe rclcpp)

add_executable(subscriber_deadline_exe src/subscriber_deadline.cpp)
ament_target_dependencies(subscriber_deadline_exe rclcpp)
add_executable(publisher_deadline_exe src/publisher_deadline.cpp)
ament_target_dependencies(publisher_deadline_exe rclcpp)

add_executable(subscriber_lifespan_exe src/subscriber_lifespan.cpp)
ament_target_dependencies(subscriber_lifespan_exe rclcpp)
add_executable(publisher_lifespan_exe src/publisher_lifespan.cpp)
ament_target_dependencies(publisher_lifespan_exe rclcpp)

install(TARGETS
   publisher_custom_minimal_qos_exe
   subscriber_custom_minimal_qos_exe
   
   subscriber_durability_exe
   publisher_durability_exe

   subscriber_deadline_exe
   publisher_deadline_exe

   subscriber_lifespan_exe
   publisher_lifespan_exe

   DESTINATION lib/${PROJECT_NAME}
 )

ament_package()