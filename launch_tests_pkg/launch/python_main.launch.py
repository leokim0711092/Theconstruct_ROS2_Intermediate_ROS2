from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.actions import IncludeLaunchDescription

from launch.actions import GroupAction

from launch.launch_description_sources import PythonLaunchDescriptionSource

from launch.substitutions import LaunchConfiguration
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare
from launch.substitutions import TextSubstitution


from launch_ros.actions import PushRosNamespace


def generate_launch_description():

    package_description = "launch_tests_pkg"

    # args that can be set from the command line or a default will be used
    # TextSubstitution(text="0.0") W ill only evaluate that in execution time.

    turning_speed_arg = DeclareLaunchArgument(
        "turning_speed", default_value=TextSubstitution(text="0.2")
    )
    forward_speed_arg = DeclareLaunchArgument(
        "forward_speed", default_value="0.1"
    )
    rviz_config_file_name_arg = DeclareLaunchArgument(
        "rviz_config_file_name", default_value=TextSubstitution(text="launch_part.rviz")
    )
    custom_namespace_arg = DeclareLaunchArgument(
        "custom_namespace", default_value=TextSubstitution(text="mazinger")
    )

    turning_speed_f = LaunchConfiguration('turning_speed')
    forward_speed_f = LaunchConfiguration('forward_speed')

    rviz_config_file_name_f = LaunchConfiguration('rviz_config_file_name')
    custom_namespace_f = LaunchConfiguration('custom_namespace')

    # include another launch file
    # use items because you need to pass a list with a key-value structure
    # [(key1,value_x),(key2,value_y),...]

    start_rviz_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([
                FindPackageShare(package_description),
                'launch',
                'start_rviz_with_arguments.launch.py'
            ])
        ]),
        launch_arguments={
            'rviz_config_file_name': rviz_config_file_name_f}.items()
    )

    # include another launch file in the chatter_ns namespace
    move_robot_with_namespace_launch = GroupAction(
        actions=[
            # push-ros-namespace to set namespace of included nodes
            PushRosNamespace(custom_namespace_f),
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource([
                    PathJoinSubstitution([
                        FindPackageShare(package_description),
                        'launch',
                        'move_with_arguments.launch.py'
                    ])
                ]),
                launch_arguments={'turning_speed': turning_speed_f,
                                  'forward_speed': forward_speed_f}.items()
            ),
        ],
    )

    return LaunchDescription([
        turning_speed_arg,
        forward_speed_arg,
        rviz_config_file_name_arg,
        custom_namespace_arg,
        start_rviz_launch,
        move_robot_with_namespace_launch,
    ])