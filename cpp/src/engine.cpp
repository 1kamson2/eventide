#include "engine.hpp"
Engine::Engine() {
  this->width = 1280;
  this->height = 960;
  this->fps = 1000;
  this->gravity = 9.80f;
  this->max_voxels_in_view = (int)(CAMERA_DEFAULT_RENDER_DISTANCE * CHUNK_SIZE *
                                   CAMERA_DEFAULT_RENDER_DISTANCE * CHUNK_SIZE *
                                   CAMERA_DEFAULT_RENDER_DEPTH * CHUNK_SIZE);

  this->AgentInstance = Agent();
  this->env_buffer = Environment::CreateDefaultEnvironment();
  /* Initialize KDTree */
  this->env_to_render = std::make_unique<KDTree>();
  this->TEMP_RENDER_BUFFER = std::vector<std::shared_ptr<VoxelNode>>();
  this->LoadEnvironmentBuffer();

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

void Engine::LoadEnvironmentBuffer() {
  for (auto el : this->env_buffer) {
    this->env_to_render->UpdateRoot(el);
  }
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
  //
  //
  //
  //
  //
  /*  This one is used for rendering */
  // this->env.FindNodesInRange(this->AgentInstance.camera->position,
  //                              CAMERA_DEFAULT_RENDER_DISTANCE *
  //                              CHUNK_SIZE);

  /*for (int voxel = 0; voxel < this->max_voxels_in_view; ++voxel) {*/
  /*}*/
  this->TEMP_RENDER_BUFFER = this->env_to_render->FindNodesInRange(
      this->AgentInstance.camera.position,
      CAMERA_DEFAULT_RENDER_DISTANCE * CHUNK_SIZE);
}

void Engine::DetectCollision(const float& delta) {
  /* Return the first collision */
  this->collision_where = -1;
  // for (int i = 0; i < this->max_voxels_in_view; ++i) {
  for (int i = 0; i < this->TEMP_RENDER_BUFFER.size(); ++i) {
    if (Environment::IsInsideAABB(this->AgentInstance.cursor,
                                  this->TEMP_RENDER_BUFFER[i])) {
      this->collision_where = i;
      return;
    }
  }
  return;
}

void Engine::RenderVoxels(const float& delta) {
  for (int i = 0; i < this->max_voxels_in_view; i++) {
    if (!Environment::IsBlank(this->TEMP_RENDER_BUFFER[i]->data.color)) {
      DrawCube(this->TEMP_RENDER_BUFFER[i]->data.position,
               this->TEMP_RENDER_BUFFER[i]->data.length,
               this->TEMP_RENDER_BUFFER[i]->data.length,
               this->TEMP_RENDER_BUFFER[i]->data.length,
               this->TEMP_RENDER_BUFFER[i]->data.color);
    }
    if (this->collision_where != -1) {
      int j = this->collision_where;
      DrawCubeWires(this->TEMP_RENDER_BUFFER[j]->data.position,
                    this->TEMP_RENDER_BUFFER[j]->data.length,
                    this->TEMP_RENDER_BUFFER[j]->data.length,
                    this->TEMP_RENDER_BUFFER[j]->data.length, BLACK);
    }
  }
}

void Engine::ModifyEnvironment(const float& delta) {
  /* TODO: Fix the bug, destroying doesn't work, because we do not update, the
   * environment buffer */
  switch (this->recent_env_state) {
    case EnvironmentState::TRY_TO_DESTROY: {
      std::cout << "[INFO] Destroyed block" << std::endl;
      std::shared_ptr<VoxelNode> voxel_to_destroy =
          this->TEMP_RENDER_BUFFER[this->collision_where];
      this->TEMP_RENDER_BUFFER[this->collision_where] =
          std::make_shared<VoxelNode>(Environment::ConstructVoxel(
              voxel_to_destroy->data.position, voxel_to_destroy->data.length));
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

void Engine::DebugInfo() {
  printf("[Current FPS]: fps=%d\n", GetFPS());
  printf("[Buffer size]: temp_buffer_size=%lu\n",
         sizeof this->TEMP_RENDER_BUFFER);
  printf("[VoxelNode size]: VoxelNode=%lu\n",
         sizeof this->TEMP_RENDER_BUFFER[0]);
  printf("[Camera position]: x=%f y=%f z=%f\n",
         this->AgentInstance.camera.position.x,
         this->AgentInstance.camera.position.y,
         this->AgentInstance.camera.position.z);
  printf(
      "[Camera target]: x=%f y=%f z=%f\n", this->AgentInstance.camera.target.x,
      this->AgentInstance.camera.target.y, this->AgentInstance.camera.target.z);
  printf("[Cursor position]: x=%f y=%f z=%f\n",
         this->AgentInstance.cursor->data.position.x,
         this->AgentInstance.cursor->data.position.y,
         this->AgentInstance.cursor->data.position.z);
  if (this->collision_where != -1) {
    printf("Cursor and object collision on: %d\n", this->collision_where);
  }
  printf("Mouse delta: x=%f y=%f\n", GetMouseDelta().x, GetMouseDelta().y);
}
