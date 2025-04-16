#include "Engine/Engine.hpp"

#include <cmath>
#include <memory>

#include "Utils/Globals.hpp"

using namespace cam_definitions;
using namespace chunk_definitions;
using namespace env_definitions;
Engine::Engine()
    : env(), ren(), agt((Vector3){0, 2, 4}), loaded_chunks(max_chunks, false) {
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

bool Engine::CheckIfChunkInBuffer(const size_t& idx) {
  return loaded_chunks[idx];
}

// bool Engine::PeekAtNextChunk(const Chunk& chunk) {}

void Engine::GetChunksToRender() {
  Vector3 agent_pos = agt.GetPosition();
  // TODO: CHANGE IT LATER FOR MORE INTELLIGENT APPROACH
  // TODO: HARDCODED VALUE
  // TODO: MORE INTELLIGENT APPROACH (EARLY)
  // TODO: CHECK IF ALWAYS CAN ESCAPE EARLY
  // voxels_to_render.clear();
  float max_y = 3.0f;
  /* The chunks are stored in the following manner:
   * Chunk 0:
   * [0, SIZE * SIZE * max_y - 1],
   * Chunk 1:
   * [SIZE * SIZE * max_y, ... -1]
   */

  for (size_t idx = 0; idx < chunks.size(); ++idx) {
    bool PREV_STATE = false;

    if (CheckIfChunkInBuffer(idx)) {
      /* If in buffer fetch the state, if not then the PREV_STATE = false is
       * still a valid value */
      PREV_STATE = loaded_chunks[idx];
    }

    if (chunks[idx].InView(agent_pos, DEFAULT_RENDER_DISTANCE)) {
      /* The chunk possibly not seen */
      loaded_chunks[idx] = true;
    } else {
      /* The chunk possibly seen */
      loaded_chunks[idx] = false;
    }

    if ((PREV_STATE == false) && (loaded_chunks[idx] == true)) {
      /* Should load */
      std::cout << "[INFO] Loaded the chunk: " << idx << std::endl;
      chunks_to_render.push_back(chunks[idx]);
    } else if ((PREV_STATE == true) && (loaded_chunks[idx] == false)) {
      /* Should unload */
      if (chunks_to_render.size() == 0) {
        break;
      }
      for (auto it = chunks_to_render.begin(); it != chunks_to_render.end();
           ++it) {
        if (it->TheSameChunk(chunks[idx])) {
          chunks_to_render.erase(it);
          std::cout << "[INFO] Unloaded the chunk: " << idx << std::endl;
          break;
        }
      }
    }
  }
}

void Engine::GameLoop(const float& dt) {
  ProcessInput(dt);
  // DetectCollision(dt);
  GetChunksToRender();
  ren.TraverseChunks(chunks_to_render);
}
