#pragma once
#include "raylib.h"
#include "resource_manager.hpp"
#define MAX_OBJECTS_IN_AREA 10
#define EDGE_LENGTH 1.0f
#define CURSOR_LENGTH 0.1f

struct EnvironmentObject {
  BLOCKING_ID BLOCK;
  IS_AGENT_IDENTIFIER AGENT;
  float width, height, length;
  Vector3 position;
  Color color;
};

class Environment {
 public:
  static EnvironmentObject* CreateDefaultEnvironment();
  static bool IsInsideAABB(const EnvironmentObject& cursor,
                           const EnvironmentObject& voxel);

 private:
  Environment() = delete;
  virtual ~Environment() = delete;
};
