#pragma once

#include <unordered_map>
#include <vector>

#include "Chunk/Chunk.hpp"
#include "Environment/Environment.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Renderer.hpp"
class Engine {
 public:
  static constexpr int width = 640, height = 480;
  Environment env;
  Renderer ren;
  Agent agt;
  std::vector<Chunk> chunks;
  std::vector<std::shared_ptr<Voxel>> voxels_to_render;
  std::unordered_map<size_t, bool> loaded_chunks;

  Engine();
  bool CheckIfChunkInBuffer(const size_t& idx);
  bool PeekAtNextChunk(const Chunk& chunk);
  void ProcessInput(const float& dt);
  void DetectCollision(const float& dt);
  void GetChunksToRender();

  void GameLoop(const float& dt);
};
