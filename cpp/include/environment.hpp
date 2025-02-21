#pragma once
#include <math.h>
#include <raylib.h>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "resource_manager.hpp"
struct EnvironmentObject;
template <typename EnvoObj>
struct EnvironmentNode;
using Voxel = EnvironmentObject;
using VoxelNode = EnvironmentNode<Voxel>;
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
  float GetDimValue(const int& what_dim) {
    float dim_value = 0;
    switch (what_dim) {
      case 0:
        dim_value = this->data.position.x;
        break;
      case 1:
        dim_value = this->data.position.y;
        break;
      case 2:
        dim_value = this->data.position.z;
        break;
      default:
        std::cerr << "[ERROR] User has given " << what_dim << " as a parameter."
                  << std::endl;
        throw std::invalid_argument("[ERROR] This dimension doesn't exist");
        break;
    }
    return dim_value;
  }
};

class Environment {
 public:
  static std::vector<std::shared_ptr<VoxelNode>> CreateDefaultEnvironment();
  static bool IsInsideAABB(const std::unique_ptr<VoxelNode>& cursor,
                           const std::shared_ptr<VoxelNode>& voxel);
  static bool IsBlank(Color color);

  /* Constructors for Voxels */
  static VoxelNode ConstructVoxel(Vector3 position, float length);
  static VoxelNode ConstructVoxel(BLOCKING_ID does_block,
                                  IS_AGENT_IDENTIFIER is_agent, float length,
                                  Vector3 position, Color color);

 private:
  Environment() = delete;
  virtual ~Environment() = delete;
};
