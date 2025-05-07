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
if(DEFINED ENV{ROS_DISTRO})
  message(STATUS "ROS_DISTRO: $ENV{ROS_DISTRO}")
  if($ENV{ROS_DISTRO} STREQUAL "jazzy")
    find_package(gz-sim8 REQUIRED)
    set(GZ_SIM gz-sim8)
  elseif($ENV{ROS_DISTRO} STREQUAL "rolling")
    find_package(gz-sim7 REQUIRED)
    set(GZ_SIM gz-sim7)
  elseif($ENV{ROS_DISTRO} STREQUAL "humble")
    find_package(gz-sim7 REQUIRED)
    set(GZ_SIM gz-sim7)
  endif()
endif()
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

set(OBSTACLE_LIBS
  obstacles
  obstacle1
  obstacle2
)

foreach(lib ${OBSTACLE_LIBS})
  add_library(${lib} SHARED src/${lib}.cpp)
  ament_target_dependencies(${lib} GZ_SIM)
endforeach()

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
ament_export_dependencies(GZ_SIM)
ament_export_dependencies(geometry_msgs)
ament_export_dependencies(nav_msgs)
ament_export_dependencies(rclcpp)
ament_export_dependencies(sensor_msgs)
ament_export_dependencies(tf2)
ament_package()
