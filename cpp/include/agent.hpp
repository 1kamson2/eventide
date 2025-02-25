#pragma once
#include <iostream>
#include <memory>

#include "Voxels.hpp"
#include "raylib.h"
#include "rcamera.h"

class Cursor : public ObjectData {
 public:
  Vector3 position;
  Cursor(BLOCK_ID block, AGENT_ID agent, const uint& color,
         const Vector3& position);
};

class Agent {
 public:
  mutable float x, y, z, yaw, pitch, roll, movement_speed, rotate_speed;
  Agent();
  // Agent(params);
  void AgentUpdate(EnvironmentState state);
  Cursor cursor;
  int cameraMode;
  Camera camera;
  Camera CameraInit();
  void CameraChangeProjection();
};
