#pragma once
#include <math.h>
#include <raylib.h>
#include <raymath.h>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "SimplexNoise.h"
#include "resource_manager.hpp"

class Environment {
 public:
  static void TestNoise(std::vector<std::shared_ptr<VoxelNode>>& voxel_buffer);
  static void GenerateWorld(
      std::vector<std::shared_ptr<VoxelNode>>& voxel_buffer);
  static bool IsInsideAABB(const std::unique_ptr<VoxelNode>& cursor,
                           const std::shared_ptr<VoxelNode>& voxel);
  static bool IsBlank(Color color);
  static float PerlinNoise(const float& x, const float& y);

  /* Constructors for Voxels */
  static VoxelNode ConstructVoxel(Vector3 position, float length);
  static VoxelNode ConstructVoxel(BLOCKING_ID does_block,
                                  IS_AGENT_IDENTIFIER is_agent, float length,
                                  Vector3 position, Color color);

 private:
  Environment() = delete;
  virtual ~Environment() = delete;
};
