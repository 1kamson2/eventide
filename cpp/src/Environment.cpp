#include "Environment.hpp"

#include <raymath.h>

#include <cassert>

#include "ResourceManager.hpp"
#include "SimplexNoise.h"

void Environment::GenerateWorld(
    std::vector<std::shared_ptr<Voxel>>& voxel_buffer) {
  /* TODO: Do chunk structure, to keep easily all Voxels */
  int currently_rendered = 0;
  constexpr float f = 0.5f;
  constexpr float norm = 1.75f;
  const float bound = CHUNK_SIZE * 8;
  for (float x = 0; x < bound; ++x) {
    for (float y = 0; y < 3; ++y) {
      float threshold = y / 3;
      for (float z = 0; z < bound; ++z) {
        float elevation =
            (SimplexNoise::noise(x, y, z) +
             f * SimplexNoise::noise(f * x, f * y, f * z) +
             f * f * SimplexNoise::noise(f * f * x, f * f * y, f * f * z)) /
            norm;
        std::cout << elevation << std::endl;
        Color color = ((elevation >= threshold || elevation <= -threshold))
                          ? DARKGREEN
                          : BLANK;
        voxel_buffer.push_back(std::make_shared<Voxel>(
            ObjectInfo(BLOCK_ID::YES, AGENT_ID::NO, EDGE_LENGTH,
                       (Vector3){x, -y, z}, color)));
      }
    }
  }
}

bool Environment::IsBlank(Color color) {
  return color.a == 0 && color.b == 0 && color.g == 0 && color.r == 0;
}

bool Environment::IsInsideAABB(const std::unique_ptr<Voxel>& cursor,
                               const std::shared_ptr<Voxel>& voxel) {
  /* If the cursor is small enough it behaves as if point */
  return (
      (cursor->data.position.x + CURSOR_LENGTH) >=
          (voxel->data.position.x - EDGE_LENGTH / 2) &&
      cursor->data.position.x <= (voxel->data.position.x + EDGE_LENGTH / 2) &&
      (cursor->data.position.y + CURSOR_LENGTH) >=
          (voxel->data.position.y - EDGE_LENGTH / 2) &&
      cursor->data.position.y <= (voxel->data.position.y + EDGE_LENGTH / 2) &&
      (cursor->data.position.z + CURSOR_LENGTH) >=
          (voxel->data.position.z - EDGE_LENGTH / 2) &&
      cursor->data.position.z <= (voxel->data.position.z + EDGE_LENGTH / 2) &&
      voxel->data.BLOCK == BLOCK_ID::YES);
}

Voxel Environment::ConstructVoxel(Vector3 position, float length) {
  ObjectInfo voxel(BLOCK_ID::NO, AGENT_ID::YES, length, position, RED);
  return Voxel(voxel);
}

Voxel Environment::ConstructVoxel(BLOCK_ID does_block, AGENT_ID is_agent,
                                  float length, Vector3 position, Color color) {
  ObjectInfo voxel_custom(does_block, is_agent, length, position, color);
  return Voxel(voxel_custom);
}
