#include "Engine/Engine.hpp"

#include <cmath>
#include <memory>

#include "Chunk/Chunk.hpp"
#include "Environment/Environment.hpp"
#include "Renderer/Agent.hpp"
#include "Utils/Enums.hpp"

using namespace agent;
using namespace chunk;
using namespace environment;
using namespace renderer;
using namespace chunk_enums;
using namespace agent_enums;
Engine::Engine()
    : env(),
      agt((Vector3){0, 16, 0}, DEFAULT_LOOK_AT, DEFAULT_UP,
          (Vector3){0.0f, 1.0f, 0.0f},
          Vector3CrossProduct(DEFAULT_LOOK_AT, DEFAULT_UP)),
      ren(agt) {
  std::cout << "[INFO] Loading chunks." << std::endl;
  env.WorldInit(chunks);
}

void Engine::ProcessInput(const float& dt) {
  if (IsKeyPressed(KEY_COMMA)) {
    debugging = !debugging;
    if (debugging) {
      std::cout << "[INFO] Debugging is ON" << std::endl;
    } else {
      std::cout << "[INFO] Debugging is OFF" << std::endl;
    }
  }
  if (IsKeyPressed(KEY_P)) {
    agt.FetchState(AgentMovement::PROJECT, dt);
  }
  if (IsKeyDown(KEY_W)) {
    agt.FetchState(AgentMovement::FORWARD, dt);
  }
  if (IsKeyDown(KEY_S)) {
    agt.FetchState(AgentMovement::BACKWARD, dt);
  }
  if (IsKeyDown(KEY_A)) {
    agt.FetchState(AgentMovement::RIGHT, dt);
  }
  if (IsKeyDown(KEY_D)) {
    agt.FetchState(AgentMovement::LEFT, dt);
  } else {
    agt.FetchState(AgentMovement::IDLE, dt);
  }
}

// bool Engine::PeekAtNextChunk(const Chunk& chunk) {}

void Engine::GetChunksToRender() {
  /*
   * Parameters:
   *  None
   * Function:
   *  Loads and unloads chunks from the render buffer.
   */
  Vector3 position = agt.GetPosition();
  for (size_t idx = 0; idx < MAX_CHUNKS_ALLOWED; ++idx) {
    bool CHUNK_IS_SEEN =
        chunks[idx].InView(position, DEFAULT_DISTANCE) ? true : false;

    if (CHUNK_IS_SEEN) {
      if (!chunks[idx].IsVisible()) {
        chunks[idx].MarkAs(Visibility::VISIBLE);

        /* If debugging is ON, then display information */
        if (debugging) {
          std::cout << "[INFO] Loaded chunk: " << idx << std::endl;
          /* Jump to the next iteration */
          continue;
        }
      }
    }

    if (!CHUNK_IS_SEEN) {
      if (chunks[idx].IsVisible()) {
        chunks[idx].MarkAs(Visibility::INVISIBLE);

        /* If debugging is ON, then display information */
        if (debugging) {
          std::cout << "[INFO] Unloaded chunk: " << idx << std::endl;
          continue;
        }
      }
    }
  }
}

void Engine::GameLoop(const float& dt) {
  ProcessInput(dt);
  // DetectCollision(dt);
  GetChunksToRender();
  ren.TraverseChunks(chunks);
}
