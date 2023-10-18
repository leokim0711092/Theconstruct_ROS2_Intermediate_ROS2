from launch import LaunchDescription
from launch.substitutions import LaunchConfiguration
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

from launch_ros.actions import Node

from launch.actions import DeclareLaunchArgument
from launch.actions import LogInfo


def generate_launch_description():

    package_description = "launch_tests_pkg"

    rviz_config_file_name = LaunchConfiguration('rviz_config_file_name')

    rviz_config_file_name_arg = DeclareLaunchArgument(
        'rviz_config_file_name', default_value='launch_part.rviz')

    global_path_to_rviz_file = PathJoinSubstitution([
        FindPackageShare(package_description),
        'rviz_config',
        rviz_config_file_name
    ])

    message_path = LogInfo(
        msg=global_path_to_rviz_file)

    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        output='screen',
        name='rviz_node',
        parameters=[{'use_sim_time': True}],
        arguments=['-d', global_path_to_rviz_file])

    # create and return the launch description object
    return LaunchDescription(
        [
            rviz_config_file_name_arg,
            rviz_node,
            message_path
        ]
    )