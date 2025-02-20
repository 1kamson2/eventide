#include "engine.hpp"

#include "agent.hpp"
#include "environment.hpp"

Engine::Engine() {
  this->width = 1280;
  this->height = 960;
  this->fps = 144;
  this->gravity = 9.80f;
  this->max_voxels_in_view = (int)(CAMERA_DEFAULT_RENDER_DISTANCE * CHUNK_SIZE *
                                   CAMERA_DEFAULT_RENDER_DISTANCE * CHUNK_SIZE *
                                   CAMERA_DEFAULT_RENDER_DEPTH * CHUNK_SIZE);

  this->AgentInstance = Agent();
  this->env_to_render = KDTree();
  this->env_buffer = Environment::CreateDefaultEnvironment();

  /*  This one is used for rendering */
  // this->env.FindNodesInRange(this->AgentInstance.camera->position,
  //                              CAMERA_DEFAULT_RENDER_DISTANCE *
  //                              CHUNK_SIZE);
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

bool Engine::VoxelHeuristic(const Voxel& voxel1, const Voxel& voxel2) const {
  PositionVectors vp1 = VoxelPositionVectors(0.0f, voxel1);
  PositionVectors vp2 = VoxelPositionVectors(0.0f, voxel2);
  /* TODO: Placeholder for delta */
  /* Check if BOTH voxels are in view */
  if (!(this->IsVoxelInView(0.0f, vp1) && this->IsVoxelInView(0.0f, vp2))) {
    return false;
  }

  double vd1 = this->VoxelDistanceVector(vp1);
  double vd2 = this->VoxelDistanceVector(vp2);
  return vd1 < vd2;
}

void Engine::FindVoxelsInView(const float& delta) {
  /* TODO: Bug where if we leave an area it crashes, also there are bugs in
   * finding the correct voxels */
  /* Probably I should start looking from the players position */
  /* If there are not sufficient voxels, put any. */
  /* Should implement some structure that keeps those voxels sorted in relation
   * to the agent */
  // std::sort(this->env.begin(), this->env.end(),
  //           [&](Voxel v1, Voxel v2) { return this->VoxelHeuristic(v1, v2);
  //           });

  int currently_found = 0;
  PositionVectors pos;
  for (int voxel = 0; voxel < MAX_OBJECTS_IN_AREA &&
                      currently_found < this->max_voxels_in_view;
       ++voxel) {
    pos = this->VoxelPositionVectors(delta, this->env[voxel]);
    if (this->IsVoxelInView(delta, pos)) {
      this->env_render_buffer[currently_found] = this->env[voxel];
      currently_found++;
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

double Engine::VoxelDistanceVector(PositionVectors voxel) const {
  double dx, dy, dz;
  std::tie(dx, dy, dz) = voxel;
  return std::sqrt(pow(dx, 2) + std::pow(dy, 2) + std::pow(dz, 2));
}

PositionVectors Engine::VoxelPositionVectors(const float& delta,
                                             Voxel voxel) const {
  /* Calculate absolute values of delta vectors */
  double dx_vector =
      std::abs(voxel.position.x - this->AgentInstance.camera->position.x);
  double dy_vector =
      std::abs(voxel.position.y - this->AgentInstance.camera->position.y);
  double dz_vector =
      std::abs(voxel.position.z - this->AgentInstance.camera->position.z);
  return std::make_tuple(dx_vector, dy_vector, dz_vector);
}

bool Engine::IsVoxelInView(const float& delta,
                           PositionVectors delta_vector) const {
  double dx, dy, dz;
  std::tie(dx, dy, dz) = delta_vector;

  return (dx < CAMERA_DEFAULT_RENDER_DISTANCE * CHUNK_SIZE &&
          dy < CAMERA_DEFAULT_RENDER_DEPTH * CHUNK_SIZE &&
          dz < CAMERA_DEFAULT_RENDER_DISTANCE * CHUNK_SIZE);
}

void Engine::ModifyEnvironment(const float& delta) {
  /* TODO: Fix the bug, destroying doesn't work, because we do not update, the
   * environment buffer */
  switch (this->recent_env_state) {
    case EnvironmentState::TRY_TO_DESTROY: {
      std::cout << "[INFO] Destroyed block" << std::endl;
      Voxel voxel_to_destroy = this->env_render_buffer[this->collision_where];
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

void Engine::LoadEnvironmentBuffer() {
  for (auto el : this->env_buffer) {
    this->env_to_render.UpdateRoot(el);
  }
}

void Engine::DebugInfo() {
  printf("[Current FPS]: fps=%d\n", GetFPS());
  printf("[Camera position]: x=%f y=%f z=%f\n",
         this->AgentInstance.camera->position.x,
         this->AgentInstance.camera->position.y,
         this->AgentInstance.camera->position.z);
  printf("[Camera target]: x=%f y=%f z=%f\n",
         this->AgentInstance.camera->target.x,
         this->AgentInstance.camera->target.y,
         this->AgentInstance.camera->target.z);
  printf("[Cursor position]: x=%f y=%f z=%f\n",
         this->AgentInstance.cursor.position.x,
         this->AgentInstance.cursor.position.y,
         this->AgentInstance.cursor.position.z);
  if (this->collision_where != -1) {
    printf("Cursor and object collision on: %d\n", this->collision_where);
  }
  printf("Mouse delta: x=%f y=%f\n", GetMouseDelta().x, GetMouseDelta().y);
}
