#pragma once
#include <iostream>
#include <memory>

#include "environment.hpp"
#include "raylib.h"
#include "rcamera.h"
#define CAMERA_DEFAULT_POSITION_X 0.0f
#define CAMERA_DEFAULT_POSITION_Y 2.0f
#define CAMERA_DEFAULT_POSITION_Z 4.0f
#define CAMERA_DEFAULT_TARGET (Vector3){0.0f, 2.0f, 0.0f}
#define CAMERA_DEFAULT_ROTATION (Vector3){0.0f, 1.0f, 0.0f}
#define CAMERA_DEFAULT_FOV 90.0f
#define CAMERA_DEFAULT_PERSPECTIVE CAMERA_PERSPECTIVE
#define CAMERA_DEFAULT_MODE CAMERA_FIRST_PERSON
#define CAMERA_DEFAULT_X_FACTOR 0.1f
#define CAMERA_DEFAULT_ROTATE_FACTOR 0.05f
#define CAMERA_DEFAULT_MAX_SPEED 1.0f

enum class EnvironmentState {
  MOVE_X_NORTH = 0,
  MOVE_X_SOUTH = 1,
  MOVE_X_EAST = 2,
  MOVE_X_WEST = 3,
  ROLL = 4,
  PROJECTION = 5,
  IDLE = 6,
  TRY_TO_DESTROY = 100,
  TRY_TO_CREATE = 101,
};

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
