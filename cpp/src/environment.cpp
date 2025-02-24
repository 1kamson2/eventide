#include "environment.hpp"

#include <raymath.h>

#include <cassert>

#include "SimplexNoise.h"
#include "resource_manager.hpp"

void Environment::GenerateWorld(
    std::vector<std::shared_ptr<VoxelNode>>& voxel_buffer) {
  /* TODO: Do chunk structure, to keep easily all Voxels */
  int currently_rendered = 0;
  constexpr float f = 0.5f;
  constexpr float norm = 1.75f;
  const float bound = CHUNK_SIZE * 3;
  for (float x = 0; x < bound; ++x) {
    for (float y = 0; y < bound; ++y) {
      float threshold = y / bound;
      for (float z = 0; z < bound; ++z) {
        float elevation =
            (SimplexNoise::noise(x, y, z) +
             f * SimplexNoise::noise(f * x, f * y, f * z) +
             f * f * SimplexNoise::noise(f * f * x, f * f * y, f * f * z)) /
            norm;
        Color color = elevation >= threshold ? DARKGREEN : BLANK;
        voxel_buffer.push_back(std::make_shared<VoxelNode>(
            Voxel(BLOCKING_ID::YES, IS_AGENT_IDENTIFIER::NO, EDGE_LENGTH,
                  (Vector3){x, -y, z}, color)));
      }
    }
  }
}

bool Environment::IsBlank(Color color) {
  return color.a == 0 && color.b == 0 && color.g == 0 && color.r == 0;
}

bool Environment::IsInsideAABB(const std::unique_ptr<VoxelNode>& cursor,
                               const std::shared_ptr<VoxelNode>& voxel) {
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
      voxel->data.BLOCK == BLOCKING_ID::YES);
}

VoxelNode Environment::ConstructVoxel(Vector3 position, float length) {
  Voxel voxel(BLOCKING_ID::NO, IS_AGENT_IDENTIFIER::YES, length, position, RED);
  return VoxelNode(voxel);
}

VoxelNode Environment::ConstructVoxel(BLOCKING_ID does_block,
                                      IS_AGENT_IDENTIFIER is_agent,
                                      float length, Vector3 position,
                                      Color color) {
  Voxel voxel_custom(does_block, is_agent, length, position, color);
  return VoxelNode(voxel_custom);
}
