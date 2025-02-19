#include "engine.hpp"

#include "agent.hpp"
#include "environment.hpp"

Engine::Engine() {
  this->width = 640;
  this->height = 480;
  this->fps = 60;
  this->gravity = 9.80f;
  this->env = Environment::CreateDefaultEnvironment();
  this->AgentInstance = Agent();
  this->collision_where = -1;
}

void Engine::ProcessInput(const float& delta) {
  if (IsKeyDown(KEY_P)) {
    AgentInstance.AgentUpdate(CameraUpdateState::PROJECTION);
  }
  if (IsKeyDown(KEY_W)) {
    AgentInstance.AgentUpdate(CameraUpdateState::MOVE_X_NORTH);
  }
  if (IsKeyDown(KEY_S)) {
    AgentInstance.AgentUpdate(CameraUpdateState::MOVE_X_SOUTH);
  }
  if (IsKeyDown(KEY_A)) {
    AgentInstance.AgentUpdate(CameraUpdateState::MOVE_X_EAST);
  }
  if (IsKeyDown(KEY_D)) {
    AgentInstance.AgentUpdate(CameraUpdateState::MOVE_X_WEST);
  } else {
    AgentInstance.AgentUpdate(CameraUpdateState::IDLE);
  }
}

void Engine::Update(const float& delta) {
  /* Keep in mind, that the processing the input should be first */
  this->ProcessInput(delta);
  this->RenderPrimitives(delta);
  this->DetectCollision(delta);
  this->DebugInfo();
}

void Engine::DetectCollision(const float& delta) {
  /* Return the first collision */
  this->collision_where = -1;
  for (int i = 0; i < MAX_OBJECTS_IN_AREA; ++i) {
    if (Environment::IsInsideAABB(this->AgentInstance.cursor, this->env[i])) {
      this->collision_where = i;
      return;
    }
  }
  return;
}

void Engine::RenderPrimitives(const float& delta) {
  for (int i = 0; i < MAX_OBJECTS_IN_AREA; i++) {
    DrawCube(env[i].position, env[i].width, env[i].height, env[i].length,
             env[i].color);
    DrawCubeWires(env[i].position, env[i].width, env[i].height, env[i].length,
                  BLACK);
  }
}

void Engine::DebugInfo() {
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
