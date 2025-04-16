#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Chunk/Chunk.hpp"
#include "Utils/Enums.hpp"
#include "Voxel/Voxel.hpp"
using namespace chunk;
namespace environment {
constexpr float Y_BOUND = SIZE * 2;
constexpr float X_BOUND = SIZE * 16;
constexpr float Z_BOUND = SIZE * 16;
constexpr float FREQ = 1.0f;
constexpr float NORM = 1.75f;
constexpr float MAX_CHUNKS_ALLOWED = 128.0f;
class Environment {
 public:
  uint64_t max_voxels_rendered = 1 << 63;
  uint64_t max_chunks_rendered = 1 << 63;
  Environment();
  void PerlinNoise();
  void WorldInit(std::vector<Chunk>& chunks_to_render);

  bool Moisture(const float& x, const float& y, const float& z,
                const float& norm, const float& f);
  float GetElevation(const float& x, const float& y, const float& z,
                     const float& f, const float& norm);

  Color AssignColor(const float& elevation, const bool& moisture,
                    const float& y) const;
  void ChunkRender(std::vector<Chunk>& chunks_to_render, const float& f,
                   const float& norm, const int& x, const int& y, const int& z);
  void IsInAABB(const std::unique_ptr<Voxel>& cursor,
                const std::shared_ptr<Voxel>& voxel);
};
}  // namespace environment
