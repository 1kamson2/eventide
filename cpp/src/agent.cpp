#include "agent.hpp"

#include <raylib.h>

#include "environment.hpp"

Agent::Agent() {
  this->x = CAMERA_DEFAULT_POSITION_X;
  this->y = CAMERA_DEFAULT_POSITION_Y;
  this->z = CAMERA_DEFAULT_POSITION_Z;
  this->yaw = 0.0f;
  this->pitch = 1.0f;
  this->roll = 0.0f;
  this->movement_speed = CAMERA_DEFAULT_X_FACTOR;
  this->rotate_speed = CAMERA_DEFAULT_ROTATE_FACTOR;
  this->cameraMode = CAMERA_DEFAULT_MODE;
  this->camera = this->CameraInit();
  this->cursor = this->CursorInit();
}

EnvironmentObject Agent::CursorInit() {
  EnvironmentObject cursorobj{};
  cursorobj.BLOCK = BLOCKING_ID::NO;
  cursorobj.AGENT = IS_AGENT_IDENTIFIER::YES;
  cursorobj.width = CURSOR_LENGTH;
  cursorobj.height = CURSOR_LENGTH;
  cursorobj.length = CURSOR_LENGTH;
  cursorobj.position = this->camera.target;
  cursorobj.color = RED;
  return cursorobj;
}

Camera Agent::CameraInit() {
  Camera camera = {0};
  camera.position = (Vector3){this->x, this->y, this->z};
  camera.target = CAMERA_DEFAULT_TARGET;
  camera.up = CAMERA_DEFAULT_ROTATION;
  camera.fovy = CAMERA_DEFAULT_FOV;
  camera.projection = CAMERA_DEFAULT_PERSPECTIVE;
  return camera;
}

void Agent::AgentUpdate(CameraUpdateState state) {
  float deltax = 0.0f, deltay = 0.0f, deltavert = 0.0f, deltahoriz = 0.0f;
  switch (state) {
    case CameraUpdateState::MOVE_X_NORTH:
      deltax = this->movement_speed;
      this->x += deltax;
      break;
    case CameraUpdateState::MOVE_X_SOUTH:
      deltax = -this->movement_speed;
      this->x -= deltax;
      break;
    case CameraUpdateState::MOVE_X_EAST:
      deltay = -this->movement_speed;
      this->y -= deltay;
      break;
    case CameraUpdateState::MOVE_X_WEST:
      deltay = this->movement_speed;
      this->y += deltay;
      break;
    case CameraUpdateState::ROLL:
      // for now no rolling
      this->roll += 0.0f;
      break;
    case CameraUpdateState::PROJECTION:
      this->CameraChangeProjection();
      break;
    case CameraUpdateState::IDLE:
      /* Default state */
      break;
    default:
      std::cout << "[WARNING] Unhandled state in AgentUpdate" << std::endl;
  }
  /* TODO: Put a cap, consider negative values */
  deltavert = GetMouseDelta().x * this->rotate_speed;
  deltahoriz = GetMouseDelta().y * this->rotate_speed;
  this->camera.up = (Vector3){this->yaw, this->pitch, this->roll};
  UpdateCameraPro(&this->camera, (Vector3){deltax, deltay, 0.0f},
                  (Vector3){deltavert, deltahoriz, 0.0f},
                  GetMouseWheelMove() * 0.0f);
  this->cursor.position = this->camera.target;
}

void Agent::CameraChangeProjection() {
  switch (camera.projection) {
    case CAMERA_PERSPECTIVE:
      this->cameraMode = CAMERA_THIRD_PERSON;
      this->camera.position = (Vector3){0.0f, 2.0f, -100.0f};
      this->camera.target = (Vector3){0.0f, 2.0f, 0.0f};
      this->camera.up = (Vector3){0.0f, 1.0f, 0.0f};
      this->camera.projection = CAMERA_ORTHOGRAPHIC;
      this->camera.fovy = 20.0f;  // near plane width in CAMERA_ORTHOGRAPHIC
      CameraYaw(&camera, -135 * DEG2RAD, true);
      CameraPitch(&camera, -45 * DEG2RAD, true, true, false);
      break;
    case CAMERA_ORTHOGRAPHIC:
      this->cameraMode = CAMERA_THIRD_PERSON;
      this->camera.position = (Vector3){0.0f, 2.0f, 10.0f};
      this->camera.target = (Vector3){0.0f, 2.0f, 0.0f};
      this->camera.up = (Vector3){0.0f, 1.0f, 0.0f};
      this->camera.projection = CAMERA_PERSPECTIVE;
      this->camera.fovy = 60.0f;
      break;
  }
}
