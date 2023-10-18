from launch import LaunchDescription

from launch_ros.actions import Node


def generate_launch_description():

    move_robot_node = Node(
        package='launch_tests_pkg',
        executable='move_robot_exe',
        output='screen',
        name='move_robot_node',
        parameters=[{'use_sim_time': True}])

    # create and return launch description object
    return LaunchDescription(
        [
            move_robot_node
        ]
    )