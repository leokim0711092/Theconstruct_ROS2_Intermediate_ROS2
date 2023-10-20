from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='dds_tests_pkg',
            executable='publisher_dds_exe',
            output='screen',
            emulate_tty=True),
    ])