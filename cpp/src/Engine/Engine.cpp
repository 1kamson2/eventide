#include "Engine/Engine.hpp"

#include "Utils/Globals.hpp"

using namespace cam_definitions;
Engine::Engine() : env(), ren(), agt((Vector3){0, 2, 4}) {
  std::cout << "[INFO] Loading chunks." << std::endl;
  env.WorldInit(chunks);
}

void Engine::ProcessInput(const float& dt) {
  if (IsKeyDown(KEY_P)) {
    agt.AgentUpdate(Action::PROJECTION);
  }
  if (IsKeyDown(KEY_W)) {
    agt.AgentUpdate(Action::MOVE_X_NORTH);
  }
  if (IsKeyDown(KEY_S)) {
    agt.AgentUpdate(Action::MOVE_X_SOUTH);
  }
  if (IsKeyDown(KEY_A)) {
    agt.AgentUpdate(Action::MOVE_X_EAST);
  }
  if (IsKeyDown(KEY_D)) {
    agt.AgentUpdate(Action::MOVE_X_WEST);
  }
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    Action act = Action::TRY_TO_CREATE;
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    Action act = Action::TRY_TO_CREATE;
  } else {
    agt.AgentUpdate(Action::IDLE);
  }
}

void Engine::GetChunksToRender() {
  Vector3 agent_pos = agt.GetPosition();
  // TODO: CHANGE IT LATER FOR MORE INTELLIGENT APPROACH
  // TODO: HARDCODED VALUE
  // TODO: MORE INTELLIGENT APPROACH (EARLY)
  // TODO: CHECK IF ALWAYS CAN ESCAPE EARLY
  voxels_to_render.clear();
  float max_y = 3.0f;
  for (Chunk& chunk : chunks) {
    if (chunk.InView(agent_pos, DEFAULT_RENDER_DISTANCE)) {
      chunk.LoadVoxelsToY(max_y, voxels_to_render, chunk.root_voxel);
    }
  }
}

void Engine::GameLoop(const float& dt) {
  ProcessInput(dt);
  // DetectCollision(dt);
  GetChunksToRender();
  ren.RenderVoxels(voxels_to_render);
}
