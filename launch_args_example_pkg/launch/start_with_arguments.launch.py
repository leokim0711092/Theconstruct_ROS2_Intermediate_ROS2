import launch
from launch_ros.actions import Node

# How to use Example:
# ros2 launch execution_and_callbacks_examples start_with_arguments.launch.py timer_period:=0.5


def generate_launch_description():
    return launch.LaunchDescription([
        launch.actions.DeclareLaunchArgument('msg_A', default_value='Tick'),
        launch.actions.DeclareLaunchArgument('msg_B', default_value='Tack'),
        launch.actions.DeclareLaunchArgument('timer_period'),
        launch.actions.LogInfo(
            msg=launch.substitutions.LaunchConfiguration('msg_A')),
        launch.actions.LogInfo(
            msg=launch.substitutions.LaunchConfiguration('msg_B')),
        launch.actions.LogInfo(
            msg=launch.substitutions.LaunchConfiguration('timer_period')),
        # All the arguments have to be strings. Floats will give an error of NonItreable.
        Node(
            package='launch_args_example_pkg',
            executable='arguments_examples_demo',
            output='screen',
            emulate_tty=True,
            arguments=["-timer_period_message", launch.substitutions.LaunchConfiguration(
                'msg_A'),
                launch.substitutions.LaunchConfiguration('msg_B'),
                "-timer_period", launch.substitutions.LaunchConfiguration(
                'timer_period')
            ]
        ),
    ])