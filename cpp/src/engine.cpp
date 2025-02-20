#include "engine.hpp"

#include <raylib.h>

#include "agent.hpp"
#include "environment.hpp"

Engine::Engine() {
  this->width = 1280;
  this->height = 960;
  this->fps = 144;
  this->gravity = 9.80f;
  this->max_voxels_in_view =
      (int)(2 * CAMERA_DEFAULT_RENDER_DISTANCE * CHUNK_SIZE *
            CAMERA_DEFAULT_RENDER_DISTANCE * CHUNK_SIZE *
            CAMERA_DEFAULT_RENDER_DEPTH * CHUNK_SIZE);
  this->env = Environment::CreateDefaultEnvironment();
  this->env_render_buffer = this->LoadEnvironmentBuffer();
  this->AgentInstance = Agent();
  this->collision_where = -1;
  this->recent_env_state = EnvironmentState::IDLE;
}

void Engine::ProcessInput(const float& delta) {
  if (IsKeyDown(KEY_P)) {
    this->recent_env_state = EnvironmentState::PROJECTION;
    AgentInstance.AgentUpdate(this->recent_env_state);
  }
  if (IsKeyDown(KEY_W)) {
    this->recent_env_state = EnvironmentState::MOVE_X_NORTH;
    AgentInstance.AgentUpdate(this->recent_env_state);
  }
  if (IsKeyDown(KEY_S)) {
    this->recent_env_state = EnvironmentState::MOVE_X_SOUTH;
    AgentInstance.AgentUpdate(this->recent_env_state);
  }
  if (IsKeyDown(KEY_A)) {
    this->recent_env_state = EnvironmentState::MOVE_X_EAST;
    AgentInstance.AgentUpdate(this->recent_env_state);
  }
  if (IsKeyDown(KEY_D)) {
    this->recent_env_state = EnvironmentState::MOVE_X_WEST;
    AgentInstance.AgentUpdate(this->recent_env_state);
  }
  /* Here we won't execute the ModifyEnvironment(...) function, because
   * the state of variable collision_where is not known at this time.
   * Function is invoked in Update(...) function.
   */
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    this->recent_env_state = EnvironmentState::TRY_TO_DESTROY;
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    this->recent_env_state = EnvironmentState::TRY_TO_CREATE;
  } else {
    this->recent_env_state = EnvironmentState::IDLE;
    AgentInstance.AgentUpdate(this->recent_env_state);
  }
}

void Engine::Update(const float& delta) {
  /* Keep in mind, that the processing the input should be first */
  this->ProcessInput(delta);
  this->FindVoxelsInView(delta);
  this->DetectCollision(delta);
  /* this->collision_where is updated after DetectCollision(...) */
  if (this->collision_where != -1 &&
      (this->recent_env_state == EnvironmentState::TRY_TO_DESTROY ||
       this->recent_env_state == EnvironmentState::TRY_TO_CREATE)) {
    this->ModifyEnvironment(delta);
  }
  this->RenderVoxels(delta);
  this->DebugInfo();
}

void Engine::FindVoxelsInView(const float& delta) {
  /* TODO: Bug where if we leave an area it crashes, also there are bugs in
   * finding the correct voxels */
  /* Probably I should start looking from the players position */
  int currently_found = 0;
  for (int voxel = 0; voxel < MAX_OBJECTS_IN_AREA &&
                      currently_found < this->max_voxels_in_view;
       ++voxel) {
    if (this->IsVoxelInView(delta, voxel)) {
      this->env_render_buffer[currently_found] = this->env[voxel];
      ++currently_found;
    }
  }
}

void Engine::DetectCollision(const float& delta) {
  /* Return the first collision */
  this->collision_where = -1;
  for (int i = 0; i < this->max_voxels_in_view; ++i) {
    if (Environment::IsInsideAABB(this->AgentInstance.cursor,
                                  this->env_render_buffer[i])) {
      this->collision_where = i;
      return;
    }
  }
  return;
}

void Engine::RenderVoxels(const float& delta) {
  for (int i = 0; i < this->max_voxels_in_view; i++) {
    if (!Environment::IsBlank(this->env_render_buffer[i].color)) {
      DrawCube(
          this->env_render_buffer[i].position,
          this->env_render_buffer[i].length, this->env_render_buffer[i].length,
          this->env_render_buffer[i].length, this->env_render_buffer[i].color);
    }
    if (this->collision_where != -1) {
      int j = this->collision_where;
      DrawCubeWires(this->env_render_buffer[j].position,
                    this->env_render_buffer[j].length,
                    this->env_render_buffer[j].length,
                    this->env_render_buffer[j].length, BLACK);
    }
  }
}

bool Engine::IsVoxelInView(const float& delta, const int& which_voxel) const {
  EnvironmentObject voxel = this->env[which_voxel];
  double delta_x_vector =
      std::abs(voxel.position.x - this->AgentInstance.camera.position.x);
  double delta_y_vector =
      std::abs(voxel.position.y - this->AgentInstance.camera.position.y);
  double delta_z_vector =
      std::abs(voxel.position.z - this->AgentInstance.camera.position.z);
  return (delta_x_vector < CAMERA_DEFAULT_RENDER_DISTANCE * CHUNK_SIZE &&
          delta_y_vector < CAMERA_DEFAULT_RENDER_DEPTH * CHUNK_SIZE &&
          delta_z_vector < CAMERA_DEFAULT_RENDER_DISTANCE * CHUNK_SIZE);
}

void Engine::ModifyEnvironment(const float& delta) {
  /* TODO: Fix the bug, destroying doesn't work, because we do not update, the
   * environment buffer */
  switch (this->recent_env_state) {
    case EnvironmentState::TRY_TO_DESTROY: {
      std::cout << "[INFO] Destroyed block" << std::endl;
      EnvironmentObject voxel_to_destroy =
          this->env_render_buffer[this->collision_where];
      this->env_render_buffer[this->collision_where] =
          Environment::ConstructVoxel(voxel_to_destroy.position,
                                      voxel_to_destroy.length);
      break;
    }
    case EnvironmentState::TRY_TO_CREATE:
      std::cout << "[WARNING] Not implemented, nothing changes. " << std::endl;
      break;
    default:
      std::cout << "[WARNING] Unhandled state in ModifyEnvironment(). "
                << std::endl;
      break;
  }
}

std::vector<EnvironmentObject> Engine::LoadEnvironmentBuffer() {
  std::vector<EnvironmentObject> buffer;
  for (int voxel = 0; voxel < this->max_voxels_in_view; ++voxel) {
    buffer.push_back(this->env[voxel]);
  }
  return buffer;
}

void Engine::DebugInfo() {
  printf("[Current FPS]: fps=%d\n", GetFPS());
  printf("[Camera position]: x=%f y=%f z=%f\n",
         this->AgentInstance.camera.position.x,
         this->AgentInstance.camera.position.y,
         this->AgentInstance.camera.position.z);
  printf(
      "[Camera target]: x=%f y=%f z=%f\n", this->AgentInstance.camera.target.x,
      this->AgentInstance.camera.target.y, this->AgentInstance.camera.target.z);
  printf("[Cursor position]: x=%f y=%f z=%f\n",
         this->AgentInstance.cursor.position.x,
         this->AgentInstance.cursor.position.y,
         this->AgentInstance.cursor.position.z);
  if (this->collision_where != -1) {
    printf("Cursor and object collision on: %d\n", this->collision_where);
  }
  printf("Mouse delta: x=%f y=%f\n", GetMouseDelta().x, GetMouseDelta().y);
}
