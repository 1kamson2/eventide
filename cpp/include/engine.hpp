#pragma once
#include <math.h>
#include <raylib.h>

#include <memory>
#include <tuple>
#include <vector>

#include "agent.hpp"
#include "environment.hpp"
#include "kdtree.hpp"

class Engine {
 public:
  Engine();
  // Engine(params);
  Agent AgentInstance;
  int width, height, fps, collision_where;
  int render_distance;
  Mesh cube_mesh;
  unsigned char tickrate;
  double gravity;

  /* This one holds all generated voxels in the memory */
  mutable std::vector<std::shared_ptr<VoxelNode>> voxel_buffer;

  /* This one is an interface for voxels */
  std::unique_ptr<KDTree> kdtree;
  mutable EnvironmentState recent_env_state;

  /* TODO: SHOULD REFACTOR TO VOXEL NODE??? */
  void ProcessInput(const float& delta);
  void RenderVoxels(const float& delta);
  bool IsVoxelInView(const float& delta, PositionVectors delta_vector) const;
  void FindVoxelsInView(const float& delta);
  void Update(const float& delta);
  void DetectCollision(const float& delta);
  void ModifyEnvironment(const float& delta);
  void KDTreeLoad();
  PositionVectors VoxelPositionVectors(const float& delta, Voxel voxel) const;
  double VoxelDistanceVector(PositionVectors vector) const;
  void DebugInfo();
  ~Engine();
};
