#include "Renderer/Agent.hpp"
using namespace agent;

Agent::Agent(Vector3 agent_pos) : agent_pos(agent_pos) {
  cam = {0};
  cam.position = agent_pos;
  cam.target = DEFAULT_TGT;
  cam.up = DEFAULT_ROT;
  cam.fovy = DEFAULT_FOV;
  cam.projection = DEFAULT_PROJ;
}

Vector3 Agent::GetPosition() const { return agent_pos; }

void Agent::CameraChangeProjection() {
  switch (cam.projection) {
    case CAMERA_PERSPECTIVE:
      cam_mode = CAMERA_THIRD_PERSON;
      cam.position = (Vector3){0.0f, 2.0f, -100.0f};
      cam.target = (Vector3){0.0f, 2.0f, 0.0f};
      cam.up = (Vector3){0.0f, 1.0f, 0.0f};
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

void Agent::AgentUpdate(AgentAction state) {
  float dt_x = 0.0f, dt_y = 0.0f, dt_vert = 0.0f, dt_horiz = 0.0f;
  switch (state) {
    case AgentAction::MOVE_X_NORTH:
      dt_x = this->movement_spd;
      agent_pos.x += dt_x;
      break;
    case AgentAction::MOVE_X_SOUTH:
      dt_x = -this->movement_spd;
      agent_pos.x += dt_x;
      break;
    case AgentAction::MOVE_X_EAST:
      dt_y = -this->movement_spd;
      agent_pos.y += dt_y;
      break;
    case AgentAction::MOVE_X_WEST:
      dt_y = this->movement_spd;
      agent_pos.y += dt_y;
      break;
    case AgentAction::ROLL:
      // for now no rolling
      roll += 0.0f;
      break;
    case AgentAction::PROJECTION:
      CameraChangeProjection();
      break;
    case AgentAction::IDLE:
      /* Default state */
      break;
    default:
      std::cout << "[WARNING] Unhandled state in AgentUpdate" << std::endl;
  }
  /* TODO: Put a cap, consider negative values */
  dt_vert = GetMouseDelta().x * this->rot_spd;
  dt_horiz = GetMouseDelta().y * this->rot_spd;
  cam.up = (Vector3){this->yaw, this->pitch, this->roll};
  UpdateCameraPro(&this->cam, (Vector3){dt_x, dt_y, 0.0f},
                  (Vector3){dt_vert, dt_horiz, 0.0f},
                  GetMouseWheelMove() * 2.0f);
  // this->cursor->data.position = this->cam.target;
}
