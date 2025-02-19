#pragma once
#include <math.h>
#include <raylib.h>

#include "resource_manager.hpp"

#define MAX_OBJECTS_IN_AREA 1 << 18
#define CHUNK_SIZE 16
#define MAX_ROW 3
#define MAX_ROW_LEVEL 3
#define EDGE_LENGTH 1.0f
#define CURSOR_LENGTH 0.1f

struct EnvironmentObject {
  BLOCKING_ID BLOCK;
  IS_AGENT_IDENTIFIER AGENT;
  float length;
  Vector3 position;
  Color color;
};

class Environment {
 public:
  static EnvironmentObject* CreateDefaultEnvironment();
  static bool IsInsideAABB(const EnvironmentObject& cursor,
                           const EnvironmentObject& voxel);
  static bool IsBlank(Color color);

  /* Constructors for Voxels */
  static EnvironmentObject ConstructVoxel(Vector3 position, float length);
  static EnvironmentObject ConstructVoxel(BLOCKING_ID does_block,
                                          IS_AGENT_IDENTIFIER is_agent,
                                          float length, Vector3 position,
                                          Color color);

 private:
  Environment() = delete;
  virtual ~Environment() = delete;
};
