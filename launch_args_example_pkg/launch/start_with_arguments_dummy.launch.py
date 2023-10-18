import launch
# These log actions are: https://github.com/ros2/launch/blob/master/launch/launch/actions 
# Logging, for example: https://github.com/ros2/launch/blob/master/launch/launch/actions/log_info.py

def generate_launch_description():
    return launch.LaunchDescription([
        launch.actions.DeclareLaunchArgument('extra_msg', default_value='hello world'),
        launch.actions.DeclareLaunchArgument('important_msg'),
        launch.actions.LogInfo(msg=launch.substitutions.LaunchConfiguration('extra_msg')),
        launch.actions.LogInfo(msg=launch.substitutions.LaunchConfiguration('important_msg')),
    ])