// Copyright 2012 Open Source Robotics Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: Ryan Shim, ChanHyeong Lee

#include "turtlebot3_gazebo/obstacle1.hpp"

#include <gz/math/Quaternion.hh>
#include <gz/math/Vector3.hh>
#include <gz/plugin/Register.hh>
#include <gz/sim/EntityComponentManager.hh>
#include <gz/sim/EventManager.hh>
#include <gz/sim/Model.hh>
#include <gz/sim/System.hh>
#include <gz/sim/Types.hh>
#include <sdf/Element.hh>
#include <chrono>
#include <cmath>
#include <memory>

using gz::math::Pose3d;
using gz::math::Quaterniond;
using gz::math::Vector3d;
using gz::sim::Entity;
using gz::sim::EntityComponentManager;
using gz::sim::EventManager;
using gz::sim::ISystemConfigure;
using gz::sim::ISystemPreUpdate;
using gz::sim::Model;
using gz::sim::System;
using gz::sim::UpdateInfo;

namespace turtlebot3_gazebo
{

void Obstacle1Plugin::Configure(
  const gz::sim::Entity & entity,
  const std::shared_ptr<const sdf::Element> &,
  gz::sim::EntityComponentManager &,
  gz::sim::EventManager &)
{
  this->model = gz::sim::Model(entity);
  this->startTime = std::chrono::steady_clock::now();
}

void Obstacle1Plugin::PreUpdate(
  const gz::sim::UpdateInfo &,
  gz::sim::EntityComponentManager & ecm)
{
  if (!this->model.Valid(ecm)) {
    return;
  }

  auto now = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = now - this->startTime;
  double cycle = 150.0;
  double t = std::fmod(elapsed.count(), cycle);

  gz::math::Vector3d start, end;
  double localT = 0.0;
  double duration = 1.0;

  if (t <= 10.0) {
    start = {2.0, 2.0, 0.25};
    end = {1.5, 1.0, 0.25};
    duration = 10.0;
    localT = t;
  } else if (t <= 40.0) {
    start = {1.5, 1.0, 0.25};
    end = {-1.5, 1.0, 0.25};
    duration = 30.0;
    localT = t - 10.0;
  } else if (t <= 60.0) {
    start = {-1.5, 1.0, 0.25};
    end = {-1.7, -1.0, 0.25};
    duration = 20.0;
    localT = t - 40.0;
  } else if (t <= 80.0) {
    start = {-1.7, -1.0, 0.25};
    end = {-1.5, 1.0, 0.25};
    duration = 20.0;
    localT = t - 60.0;
  } else if (t <= 120.0) {
    start = {-1.5, 1.0, 0.25};
    end = {1.5, 1.0, 0.25};
    duration = 40.0;
    localT = t - 80.0;
  } else if (t <= 130.0) {
    start = {1.5, 1.0, 0.25};
    end = {2.0, 2.0, 0.25};
    duration = 10.0;
    localT = t - 120.0;
  } else {
    start = {2.0, 2.0, 0.25};
    end = start;
    localT = 0.0;
  }

  double alpha = std::min(localT / duration, 1.0);
  gz::math::Vector3d currentPos = start + (end - start) * alpha;
  gz::math::Pose3d pose(currentPos, gz::math::Quaterniond::Identity);

  this->model.SetWorldPoseCmd(ecm, pose);
}

}  // namespace turtlebot3_gazebo

GZ_ADD_PLUGIN(
  turtlebot3_gazebo::Obstacle1Plugin,
  gz::sim::System,
  gz::sim::ISystemConfigure,
  gz::sim::ISystemPreUpdate)

GZ_ADD_PLUGIN_ALIAS(
  turtlebot3_gazebo::Obstacle1Plugin,
  "turtlebot3_gazebo::Obstacle1Plugin")
