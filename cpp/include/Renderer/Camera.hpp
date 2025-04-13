#pragma once
#include <raylib.h>
#include <rcamera.h>

#include "Utils/Globals.hpp"
using namespace cam_definitions;
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
  void AgentUpdate(Action state);
};
