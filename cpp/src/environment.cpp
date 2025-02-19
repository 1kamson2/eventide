#include "environment.hpp"

#include "resource_manager.hpp"

EnvironmentObject* Environment::CreateDefaultEnvironment() {
  EnvironmentObject* obstacles = new EnvironmentObject[MAX_OBJECTS_IN_AREA];
  for (int i = 0; i < MAX_OBJECTS_IN_AREA; ++i) {
    obstacles[i].BLOCK = BLOCKING_ID::YES;
    obstacles[i].AGENT = IS_AGENT_IDENTIFIER::NO;
    obstacles[i].width = EDGE_LENGTH;
    obstacles[i].height = EDGE_LENGTH;
    obstacles[i].length = EDGE_LENGTH;
    obstacles[i].position =
        (Vector3){(float)GetRandomValue(-15, 15), obstacles[i].height / 2.0f,
                  (float)GetRandomValue(-15, 15)};
    obstacles[i].color =
        (Color){(unsigned char)GetRandomValue(20, 255),
                (unsigned char)GetRandomValue(10, 55), 30, 255};
  }
  return obstacles;
}

bool Environment::IsInsideAABB(const EnvironmentObject& cursor,
                               const EnvironmentObject& voxel) {
  /* If the cursor is small enough it behaves as if point */
  /* TODO: There is a shift in a wrong direction */
  return ((cursor.position.x + CURSOR_LENGTH) >= voxel.position.x &&
          cursor.position.x <= (voxel.position.x + EDGE_LENGTH) &&
          (cursor.position.y + CURSOR_LENGTH) >= voxel.position.y &&
          cursor.position.y <= (voxel.position.y + EDGE_LENGTH) &&
          (cursor.position.z + CURSOR_LENGTH) >= voxel.position.z &&
          cursor.position.z <= (voxel.position.z + EDGE_LENGTH) &&
          voxel.BLOCK == BLOCKING_ID::YES);
}
