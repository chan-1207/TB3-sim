################################################################################
# Set minimum required version of cmake, project name and compile options
################################################################################
cmake_minimum_required(VERSION 3.5)
project(turtlebot3_gazebo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(MSVC)
  add_compile_definitions(_USE_MATH_DEFINES)
endif()

################################################################################
# Find ament packages and libraries for ament and system dependencies
################################################################################
find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2 REQUIRED)
if($ENV{ROS_DISTRO} STREQUAL "jazzy")
  find_package(gz-sim8 REQUIRED)
  set(GZ_SIM_PKG gz-sim8)
elseif($ENV{ROS_DISTRO} STREQUAL "rolling")
  find_package(gz-sim7 REQUIRED)
  set(GZ_SIM_PKG gz-sim7)
elseif($ENV{ROS_DISTRO} STREQUAL "humble")
  find_package(gz-sim6 REQUIRED)
  set(GZ_SIM_PKG gz-sim6)

################################################################################
# Build
################################################################################
link_directories(
  ${GAZEBO_LIBRARY_DIRS}
)

include_directories(
  include
  ${GAZEBO_INCLUDE_DIRS}
)

set(dependencies
  geometry_msgs
  nav_msgs
  rclcpp
  sensor_msgs
  tf2
)

add_executable(turtlebot3_drive src/turtlebot3_drive.cpp)
ament_target_dependencies(turtlebot3_drive ${dependencies})

add_library(obstacles SHARED src/obstacles.cpp)
ament_target_dependencies(obstacles ${GZ_SIM_PKG})

add_library(obstacle1 SHARED src/obstacle1.cpp)
ament_target_dependencies(obstacle1 ${GZ_SIM_PKG})

add_library(obstacle2 SHARED src/obstacle2.cpp)
ament_target_dependencies(obstacle2 ${GZ_SIM_PKG})

################################################################################
# Install
################################################################################
install(TARGETS
turtlebot3_drive
  obstacles
  obstacle1
  obstacle2
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch models params rviz urdf worlds
  DESTINATION share/${PROJECT_NAME}/
)

install(DIRECTORY include/
  DESTINATION include/
)

################################################################################
# Macro for ament package
################################################################################
ament_export_include_directories(include)
ament_export_dependencies(gz-sim8)
ament_export_dependencies(geometry_msgs)
ament_export_dependencies(nav_msgs)
ament_export_dependencies(rclcpp)
ament_export_dependencies(sensor_msgs)
ament_export_dependencies(tf2)
ament_package()
