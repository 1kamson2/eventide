#pragma once
#include <iostream>
#include <memory>

#include "environment.hpp"
#include "raylib.h"
#include "rcamera.h"

class Agent {
 public:
  mutable float x, y, z, yaw, pitch, roll, movement_speed, rotate_speed;
  Agent();
  // Agent(params);
  void AgentUpdate(EnvironmentState state);
  std::unique_ptr<VoxelNode> cursor;
  int cameraMode;
  Camera camera;
  Camera CameraInit();
  void CameraChangeProjection();
};
