from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='parameter_tests',
            executable='param_vel_node',
            name='param_vel_node',
            output='screen',
            emulate_tty=True,
            parameters=[
                {'velocity': 0.2}
            ]
        )
    ])