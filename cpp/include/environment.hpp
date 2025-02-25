#pragma once
#include <math.h>
#include <raylib.h>
#include <raymath.h>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "Chunk.hpp"
#include "SimplexNoise.h"
#include "Voxels.hpp"
#include "resource_manager.hpp"

class Environment {
 public:
  static void TestNoise(std::vector<Voxel<ObjectData>>& voxel_buffer);
  static void GenerateWorld(std::vector<Chunk<ObjectData>>& voxel_buffer);
  static Chunk<ObjectData> GenerateChunk(const int& id,
                                         const Vector2& min_coords);
  static bool IsInsideAABB(const Cursor& cursor,
                           const std::unique_ptr<Voxel<ObjectData>>& voxel);
  static bool IsBlank(Color color);
  static float PerlinNoise(const float& x, const float& y);

  /* Constructors for Voxels */
  static Voxel<ObjectData> ConstructVoxel(Vector3 position, float length);
  static Voxel<ObjectData> ConstructVoxel(BLOCK_ID does_block,
                                          AGENT_ID is_agent, float length,
                                          Vector3 position, Color color);

 private:
  Environment() = delete;
  virtual ~Environment() = delete;
};
