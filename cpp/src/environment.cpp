#include "environment.hpp"

#include <cassert>

#include "resource_manager.hpp"

std::vector<std::shared_ptr<VoxelNode>>
Environment::CreateDefaultEnvironment() {
  /* Returns the vector of shared pointers to the Voxel Nodes */
  std::vector<std::shared_ptr<VoxelNode>> obstacles{};
  int currently_rendered = 0;
  /* The code down below renders 3 x 3 x 3 block of chunks */
  /* Constructs levels of the block */
  for (int row_level = 0; row_level < 3; ++row_level) {
    /* Constructs the block along Z axis */
    for (int row = 0; row < 3; ++row) {
      /* Constructs the block along X axis */
      for (int col = 0; col < 3; ++col) {
        /* Render a single chunk of size CHUNK_SIZE × CHUNK_SIZE × CHUNK_SIZE */
        for (int level = 0; level < CHUNK_SIZE; ++level) {
          for (int width = 0; width < CHUNK_SIZE; ++width) {
            for (int length = 0; length < CHUNK_SIZE; ++length) {
              Voxel voxel(
                  BLOCKING_ID::YES, IS_AGENT_IDENTIFIER::NO, EDGE_LENGTH,
                  (Vector3){EDGE_LENGTH * length + (col * CHUNK_SIZE),
                            -level * EDGE_LENGTH - (row_level * CHUNK_SIZE),
                            EDGE_LENGTH * width + (row * CHUNK_SIZE)},
                  (Color){(unsigned char)GetRandomValue(20, 255),
                          (unsigned char)GetRandomValue(10, 55), 30, 255});
              obstacles.push_back(
                  std::make_shared<VoxelNode>(VoxelNode(voxel)));
              ++currently_rendered;
            }
          }
        }
      }
    }
  }
  assert(currently_rendered < MAX_OBJECTS_IN_AREA);
  return obstacles;
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
