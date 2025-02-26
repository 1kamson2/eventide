#pragma once
#include <math.h>
#include <raylib.h>
#include <raymath.h>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "Globals.hpp"
#include "ResourceManager.hpp"
#include "SimplexNoise.h"
struct ObjectInfo;
template <typename EnvoObj>
struct EnvironmentNode;
using Voxel = EnvironmentNode<ObjectInfo>;
using namespace engvars;
using namespace camvars;

struct ObjectInfo {
  BLOCK_ID BLOCK;
  AGENT_ID AGENT;
  float length;
  Vector3 position;
  Color color;
  ObjectInfo(BLOCK_ID block, AGENT_ID agent, float length, Vector3 position,
             Color color)
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
  static void GenerateWorld(std::vector<std::shared_ptr<Voxel>>& voxel_buffer);
  static bool IsInsideAABB(const std::unique_ptr<Voxel>& cursor,
                           const std::shared_ptr<Voxel>& voxel);
  static bool IsBlank(Color color);
  static float PerlinNoise(const float& x, const float& y);

  /* Constructors for Voxels */
  static Voxel ConstructVoxel(Vector3 position, float length);
  static Voxel ConstructVoxel(BLOCK_ID does_block, AGENT_ID is_agent,
                              float length, Vector3 position, Color color);

 private:
  Environment() = delete;
  virtual ~Environment() = delete;
};
