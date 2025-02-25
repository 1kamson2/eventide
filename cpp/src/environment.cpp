#include "environment.hpp"

#include <raymath.h>

#include <cassert>

#include "SimplexNoise.h"
#include "Voxels.hpp"
#include "resource_manager.hpp"

void Environment::GenerateWorld(std::vector<Chunk<ObjectData>>& voxel_buffer) {
  /* TODO: Do chunk structure, to keep easily all Voxels */
  int currently_rendered = 0;
  const float bound = CHUNK_SIZE * 8;
  for (int y = 0; y < 8; ++y) {
    for (int x = 0; x < 8; ++x) {
      voxel_buffer.push_back(
          GenerateChunk(currently_rendered, (Vector2){(float)x, (float)y}));
    }
  }
}

Chunk<ObjectData> Environment::GenerateChunk(const int& id,
                                             const Vector2& min_coords) {
  constexpr float f = 0.5f;
  constexpr float norm = 1.75f;
  Chunk<ObjectData> chunk(id, min_coords.x, 0.0f, min_coords.y);
  for (float x = 0; x < CHUNK_SIZE; ++x) {
    for (float y = 0; y < CHUNK_SIZE; ++y) {
      float threshold = y / CHUNK_SIZE;
      for (float z = 0; z < CHUNK_SIZE; ++z) {
        float elevation =
            (SimplexNoise::noise(x, y, z) +
             f * SimplexNoise::noise(f * x, f * y, f * z) +
             f * f * SimplexNoise::noise(f * f * x, f * f * y, f * f * z)) /
            norm;
        uint color = elevation >= threshold ? 0xFFFFFFFF : 0x0;
        ObjectData obj(BLOCK_ID::YES, AGENT_ID::NO, color);
        std::unique_ptr<Voxel<ObjectData>> voxel =
            std::make_unique<Voxel<ObjectData>>(obj, (int)x, (int)y, (int)z);
        chunk.UpdateRoot(voxel);
      }
    }
  }
  return chunk;
}

bool Environment::IsBlank(Color color) {
  return color.a == 0 && color.b == 0 && color.g == 0 && color.r == 0;
}
