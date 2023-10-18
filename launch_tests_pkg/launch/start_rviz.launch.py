import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription

from launch_ros.actions import Node
import launch


def generate_launch_description():

    # RVIZ Configuration
    rviz_config_dir = os.path.join(get_package_share_directory(
        'launch_tests_pkg'), 'rviz_config', 'launch_part.rviz')

    # This is to publish messages inside Launch files.
    message_info = launch.actions.LogInfo(
        msg=str(rviz_config_dir))

    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        output='screen',
        name='rviz_node',
        parameters=[{'use_sim_time': True}],
        arguments=['-d', rviz_config_dir])

    # create and return launch description object
    return LaunchDescription(
        [
            rviz_node,
            message_info
        ]
    )