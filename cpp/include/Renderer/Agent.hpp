#pragma once
#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>

#include <cassert>
#include <cmath>
#include <iostream>

#include "Utils/Enums.hpp"
#include "Utils/VecOperations.hpp"
using namespace agent_enums;
namespace agent {
constexpr float YAW = 0.0f;
constexpr float PITCH = 1.0f;
constexpr float MOVEMENT_SPEED = 5.0f;
constexpr float ROTATE_SPEED = 2.0f;
constexpr Vector3 DEFAULT_LOOK_AT = (Vector3){1.0f, 0.0f, 1.0f};
constexpr Vector3 DEFAULT_UP = (Vector3){0.0f, 1.0f, 0.0f};
constexpr float DEFAULT_FOV = 60.0f;
constexpr float DEFAULT_PROJ = CAMERA_PERSPECTIVE;
constexpr int DEFAULT_MODE = CAMERA_FIRST_PERSON;
constexpr float WIDTH = 640.0f, HEIGHT = 480.0f;
constexpr float ASPECT = WIDTH / HEIGHT;
class Agent {
 public:
  mutable Vector3 position;  // agent position
  mutable Vector3 front;     // camera's target
  mutable Vector3 up;        // axis that is main (0, 1, 0) is Y
  mutable Vector3 right;     // right angle
  mutable Vector3 world_up;  // what axis is main for world
  const float movement_speed;
  const float mouse_sens;
  mutable float yaw;
  mutable float pitch;
  mutable int cam_mode = DEFAULT_MODE;

  Camera cam;

  Agent(Vector3 position, Vector3 front, Vector3 up, Vector3 right,
        Vector3 world_up);
  Vector3 GetPosition() const;
  void Update(const float& x_dt, const float& z_dt, const float& dt);
  void CameraChangeProjection();
  void FetchState(AgentMovement state, const float& dt);
  Vector2 MouseDelta() const;
  void UpdateVectors();
};
}  // namespace agent
