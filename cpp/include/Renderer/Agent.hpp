#pragma once
#include <raylib.h>
#include <rcamera.h>

#include <iostream>

#include "Utils/Enums.hpp"

namespace agent {
constexpr Vector3 DEFAULT_TGT = (Vector3){0.0f, 2.0f, 0.0f};
constexpr Vector3 DEFAULT_ROT = (Vector3){0.0f, 1.0f, 0.0f};
constexpr float DEFAULT_FOV = 60.0f;
constexpr float DEFAULT_PROJ = CAMERA_PERSPECTIVE;
constexpr int DEFAULT_MODE = CAMERA_FIRST_PERSON;

class Agent {
 public:
  Vector3 agent_pos;
  mutable float yaw = 0.0f, pitch = 1.0f, roll = 0.0f;
  mutable float movement_spd = 0.1f, rot_spd = 2.0f;
  mutable int cam_mode = DEFAULT_MODE;
  Camera cam;
  Agent(Vector3 agent_pos);
  Vector3 GetPosition() const;
  void CameraChangeProjection();
  void AgentUpdate(AgentAction state);
};
}  // namespace agent
