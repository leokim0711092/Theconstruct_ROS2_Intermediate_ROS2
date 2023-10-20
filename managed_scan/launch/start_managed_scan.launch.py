from launch import LaunchDescription
from launch_ros.actions import LifecycleNode


def generate_launch_description():
    return LaunchDescription([
        LifecycleNode(package='managed_scan', executable='scan_publisher',
                      name='managed_scan_node', namespace='', output='screen')
    ])