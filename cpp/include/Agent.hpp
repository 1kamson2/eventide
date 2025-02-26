#pragma once
#include <iostream>
#include <memory>

#include "Environment.hpp"
#include "Globals.hpp"
#include "raylib.h"
#include "rcamera.h"
using namespace camvars;
using namespace engvars;
class Agent {
 public:
  mutable float x, y, z, yaw, pitch, roll, movement_speed, rotate_speed;
  Agent();
  // Agent(params);
  void AgentUpdate(EnvironmentState state);
  std::unique_ptr<Voxel> cursor;
  int cameraMode;
  Camera camera;
  Camera CameraInit();
  void CameraChangeProjection();
};
