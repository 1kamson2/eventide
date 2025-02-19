#include "environment.hpp"

#include "resource_manager.hpp"

EnvironmentObject* Environment::CreateDefaultEnvironment() {
  EnvironmentObject* obstacles = new EnvironmentObject[MAX_OBJECTS_IN_AREA];
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
              obstacles[currently_rendered].BLOCK = BLOCKING_ID::YES;
              obstacles[currently_rendered].AGENT = IS_AGENT_IDENTIFIER::NO;
              obstacles[currently_rendered].length = EDGE_LENGTH;
              obstacles[currently_rendered].position =
                  (Vector3){EDGE_LENGTH * length + (col * CHUNK_SIZE),
                            -level * EDGE_LENGTH - (row_level * CHUNK_SIZE),
                            EDGE_LENGTH * width + (row * CHUNK_SIZE)};
              obstacles[currently_rendered].color =
                  (Color){(unsigned char)GetRandomValue(20, 255),
                          (unsigned char)GetRandomValue(10, 55), 30, 255};
              ++currently_rendered;
            }
          }
        }
      }
    }
  }

  return obstacles;
}

bool Environment::IsBlank(Color color) {
  return color.a == 0 && color.b == 0 && color.g == 0 && color.r == 0;
}

bool Environment::IsInsideAABB(const EnvironmentObject& cursor,
                               const EnvironmentObject& voxel) {
  /* If the cursor is small enough it behaves as if point */
  return ((cursor.position.x + CURSOR_LENGTH) >=
              (voxel.position.x - EDGE_LENGTH / 2) &&
          cursor.position.x <= (voxel.position.x + EDGE_LENGTH / 2) &&
          (cursor.position.y + CURSOR_LENGTH) >=
              (voxel.position.y - EDGE_LENGTH / 2) &&
          cursor.position.y <= (voxel.position.y + EDGE_LENGTH / 2) &&
          (cursor.position.z + CURSOR_LENGTH) >=
              (voxel.position.z - EDGE_LENGTH / 2) &&
          cursor.position.z <= (voxel.position.z + EDGE_LENGTH / 2) &&
          voxel.BLOCK == BLOCKING_ID::YES);
}

EnvironmentObject Environment::ConstructVoxel(Vector3 position, float length) {
  EnvironmentObject voxel{};
  voxel.BLOCK = BLOCKING_ID::NO;
  voxel.AGENT = IS_AGENT_IDENTIFIER::YES;
  voxel.length = length;
  voxel.position = position;
  voxel.color = BLANK;
  return voxel;
}

EnvironmentObject Environment::ConstructVoxel(BLOCKING_ID does_block,
                                              IS_AGENT_IDENTIFIER is_agent,
                                              float length, Vector3 position,
                                              Color color) {
  EnvironmentObject voxel_custom{};
  voxel_custom.BLOCK = does_block;
  voxel_custom.AGENT = is_agent;
  voxel_custom.length = length;
  voxel_custom.position = position;
  voxel_custom.color = color;
  return voxel_custom;
}
