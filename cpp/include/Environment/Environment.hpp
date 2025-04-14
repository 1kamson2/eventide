#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Chunk/Chunk.hpp"
#include "Utils/Globals.hpp"
#include "Voxel/Voxel.hpp"
class Environment {
 public:
  uint64_t max_voxels_rendered = 1 << 63;
  uint64_t max_chunks_rendered = 1 << 63;
  Environment();
  void PerlinNoise();
  void WorldInit(std::vector<Chunk>& chunks_to_render);
  float GetElevation(const float& x, const float& y, const float& z,
                     const float& f, const float& norm);
  Color AssignColor(const float& elevation, const float& y) const;
  void ChunkRender(std::vector<Chunk>& chunks_to_render, const float& f,
                   const float& norm, const int& x, const int& y, const int& z);
  void IsInAABB(const std::unique_ptr<Voxel>& cursor,
                const std::shared_ptr<Voxel>& voxel);
};
