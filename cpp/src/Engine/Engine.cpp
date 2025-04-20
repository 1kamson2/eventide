#include "Engine/Engine.hpp"

#include <cmath>
#include <memory>

#include "Chunk/Chunk.hpp"
#include "Environment/Environment.hpp"
#include "Renderer/Agent.hpp"

using namespace agent;
using namespace chunk;
using namespace environment;
using namespace renderer;
Engine::Engine()
    : env(),
      agt((Vector3){0, 16, 0}, DEFAULT_LOOK_AT, DEFAULT_UP,
          (Vector3){0.0f, 1.0f, 0.0f},
          Vector3CrossProduct(DEFAULT_LOOK_AT, DEFAULT_UP)),
      ren(agt),
      loaded_chunks_position_buf(MAX_CHUNKS_ALLOWED, -1) {
  std::cout << "[INFO] Loading chunks." << std::endl;
  env.WorldInit(chunks);
}

void Engine::ProcessInput(const float& dt) {
  if (IsKeyDown(KEY_P)) {
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

bool Engine::CheckIfChunkInBuffer(const size_t& idx) {
  return loaded_chunks_position_buf[idx] != -1;
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
    /* We always assume, that the chunk on idx is not seen */
    size_t pos_in_loaded_buffer = -1;
    bool SHOULD_BE_UNLOADED = false;
    bool SHOULD_BE_PUSHED = false;

    if (CheckIfChunkInBuffer(idx)) {
      /* We check if the chunk has been previously loaded and is still in the
       * buffer */
      pos_in_loaded_buffer = loaded_chunks_position_buf[idx];
    }

    if (chunks[idx].InView(position, DEFAULT_DISTANCE)) {
      /* The chunk is in view, but might be not in the chunk render buffer */
      if (pos_in_loaded_buffer == -1) {
        /* The chunk wasn't in render buffer, so we load it */
        loaded_chunks_position_buf[idx] = ++chunks_to_render_last_pos;
        pos_in_loaded_buffer = loaded_chunks_position_buf[idx];
        SHOULD_BE_PUSHED = true;
      }
    } else {
      /* The chunk should be unloaded */
      /* Keep pos_in_loaded_buffer as reference to unload */
      if (pos_in_loaded_buffer != -1) {
        loaded_chunks_position_buf[idx] = -1;
        SHOULD_BE_UNLOADED = true;
      }
    }

    if (SHOULD_BE_UNLOADED) {
      std::cout << "[INFO] Unloaded the chunk: " << idx << std::endl;
      if (chunks_to_render.size() > 0) {
        chunks_to_render.erase(chunks_to_render.begin() + pos_in_loaded_buffer);
        --chunks_to_render_last_pos;
      }
      if (pos_in_loaded_buffer + 1 < MAX_CHUNKS_ALLOWED) {
        for (int idx = pos_in_loaded_buffer + 1; idx < MAX_CHUNKS_ALLOWED;
             ++idx) {
          if (loaded_chunks_position_buf[idx] != -1) {
            loaded_chunks_position_buf[idx] -= 1;
          }
        }
      }
    }
    if (SHOULD_BE_PUSHED) {
      chunks_to_render.push_back(chunks[idx]);
    }
  }
}

void Engine::GameLoop(const float& dt) {
  ProcessInput(dt);
  // DetectCollision(dt);
  GetChunksToRender();
  ren.TraverseChunks(chunks_to_render);
}
