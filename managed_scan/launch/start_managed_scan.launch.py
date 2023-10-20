from launch import LaunchDescription
from launch_ros.actions import LifecycleNode, Node


def generate_launch_description():
    return LaunchDescription([
        LifecycleNode(package='managed_scan', executable='scan_publisher',
                      name='managed_scan_node', namespace='', output='screen'),
        Node(package='managed_scan', name='manager_client_node', 
             executable='lifecycle_manager', output='screen')
    ])