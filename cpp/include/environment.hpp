#pragma once
#include <math.h>
#include <raylib.h>

#include <memory>
#include <vector>

#include "engine.hpp"
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
  EnvironmentObject(BLOCKING_ID block, IS_AGENT_IDENTIFIER agent, float length,
                    Vector3 position, Color color)
      : BLOCK(block),
        AGENT(agent),
        length(length),
        position(position),
        color(color) {}
};

template <typename EnvObj>
struct EnvironmentNode {
  EnvObj data;
  std::shared_ptr<EnvironmentNode> left = nullptr;
  std::shared_ptr<EnvironmentNode> right = nullptr;
  EnvironmentNode(EnvObj data) : data(data) {}
};

class Environment {
 public:
  static std::vector<std::shared_ptr<VoxelNode>> CreateDefaultEnvironment();
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
