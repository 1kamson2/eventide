#include "Renderer/Agent.hpp"

using namespace agent;
using namespace vec_operations;

Agent::Agent(Vector3 position, Vector3 front, Vector3 up, Vector3 right,
             Vector3 world_up)
    : position(position),
      front(front),
      up(up),
      right(right),
      world_up(world_up),
      movement_speed(MOVEMENT_SPEED),
      mouse_sens(ROTATE_SPEED),
      yaw(YAW),
      pitch(PITCH) {
  /*
   * cam.position = camera's position;
   * cam.target = camera is looking at;
   * cam.up = up; the upward axis for THE CAMERA;
   * world_up = the upward axis for THE WORLD;
   * right = perpendicular to the up axis, points to the right of the camera
   *
   */
  cam = {0};
  cam.position = position;
  cam.target = DEFAULT_LOOK_AT;
  cam.up = DEFAULT_UP;
  cam.fovy = DEFAULT_FOV;
  cam.projection = DEFAULT_PROJ;
}

Vector3 Agent::GetPosition() const { return position; }

void Agent::CameraChangeProjection() {
  switch (cam.projection) {
    case CAMERA_PERSPECTIVE:
      cam_mode = CAMERA_THIRD_PERSON;
      cam.position = (Vector3){0.0f, 2.0f, -100.0f};
      cam.target = (Vector3){0.0f, 2.0f, 0.0f};
      cam.up = DEFAULT_UP;
      cam.projection = CAMERA_ORTHOGRAPHIC;
      cam.fovy = 20.0f;  // near plane width in CAMERA_ORTHOGRAPHIC
      CameraYaw(&cam, -135 * DEG2RAD, true);
      CameraPitch(&cam, -45 * DEG2RAD, true, true, false);
      break;
    case CAMERA_ORTHOGRAPHIC:
      cam_mode = CAMERA_THIRD_PERSON;
      cam.position = (Vector3){0.0f, 2.0f, 10.0f};
      cam.target = (Vector3){0.0f, 2.0f, 0.0f};
      cam.up = (Vector3){0.0f, 1.0f, 0.0f};
      cam.projection = CAMERA_PERSPECTIVE;
      cam.fovy = 60.0f;
      break;
  }
}

void Agent::FetchState(AgentMovement state, const float& dt) {
  float x_dt = 0.0f, z_dt = 0.0f;
  float velocity = movement_speed * dt;
  switch (state) {
    case AgentMovement::FORWARD:
      position.x += velocity;
      x_dt = velocity;
      break;
    case AgentMovement::BACKWARD:
      position.x += -velocity;
      x_dt = -velocity;
      break;
    case AgentMovement::RIGHT:
      position.y += -velocity;
      z_dt = -velocity;
      break;
    case AgentMovement::LEFT:
      position.y += velocity;
      z_dt = velocity;
      break;
    case AgentMovement::PROJECT:
      CameraChangeProjection();
      break;
    case AgentMovement::IDLE:
      /* Default state */
      break;
    default:
      std::cout << "[WARNING] Unhandled state in AgentUpdate" << std::endl;
  }
  /* TODO: Put a cap, consider negative values */
  Update(x_dt, z_dt, dt);
}

Vector2 Agent::MouseDelta() const { return GetMouseDelta(); }

void Agent::UpdateVectors() {
  Vector3 front_updated{-1, -1, -1};
  // TODO: put an array of this values, so we optimize it
  front_updated.x = cos(DEG2RAD * yaw) * cos(DEG2RAD * pitch);
  front_updated.y = sin(DEG2RAD * pitch);
  front_updated.z = sin(DEG2RAD * yaw) * cos(DEG2RAD * pitch);
  assert(front_updated.x != -1 || front_updated.y != -1 ||
         front_updated.z != -1);

  Normalize(front_updated);
  front = front_updated;

  right = Vector3CrossProduct(world_up, front);
  Normalize(right);
  up = Vector3CrossProduct(front, right);
  Normalize(up);
}

void Agent::Update(const float& x_dt, const float& z_dt, const float& dt) {
  /*
   * Parameters:
   *  x_dt: Displacement on the X-axis
   *  z_dt: Displacement on the Z-axis
   *  dt  : Delta time
   *  Function:
   *    Update the entire agent class.
   * */
  Vector2 mouse_dt = GetMouseDelta();
  float mouse_x = mouse_dt.x * mouse_sens;
  float mouse_y = mouse_dt.y * mouse_sens;

  yaw += mouse_x;
  pitch += mouse_y;

  if (yaw > 89.0f) {
    yaw = 88.0f;
  } else if (yaw < -89.0f) {
    yaw = -88.0f;
  }
  if (pitch > 89.0f) {
    pitch = 89.0f;
  } else if (pitch < -89.0f) {
    pitch = -89.0f;
  }
  // pitch = pitch < 1.0f ? pitch + mouse_y : 89.0f;
  UpdateVectors();
  // cam.up = up;
  // cam.target = position + front;
  UpdateCameraPro(&this->cam, (Vector3){x_dt, z_dt, 0},
                  (Vector3){mouse_x, mouse_y, 0},
                  GetMouseWheelMove() * mouse_sens);

  // this->cursor->data.position = this->cam.target;
}
