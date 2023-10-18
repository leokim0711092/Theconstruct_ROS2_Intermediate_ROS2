#!/usr/bin/python3
# -*- coding: utf-8 -*-
import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():

    pkg_box_bot_gazebo = get_package_share_directory('launch_tests_pkg')

    move_robot = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_box_bot_gazebo, 'launch',
                         'move.launch.py'),
        )
    )

    start_rviz = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_box_bot_gazebo, 'launch', 'start_rviz.launch.py'),
        )
    )

    return LaunchDescription([
        move_robot,
        start_rviz,

    ])