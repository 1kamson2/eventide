#pragma once
#include <math.h>
#include <raylib.h>

#include <tuple>
#include <vector>

#include "agent.hpp"
#include "environment.hpp"
#include "kdtree.hpp"
using PositionVectors = std::tuple<double, double, double>;
using Voxel = EnvironmentObject;
using VoxelNode = EnvironmentNode<EnvironmentObject>;

class Engine {
 public:
  Engine();
  // Engine(params);
  Agent AgentInstance;
  int width, height, fps, collision_where;
  int max_voxels_in_view;
  double gravity;

  /* This one holds all generated voxels in the memory */
  mutable std::vector<std::shared_ptr<VoxelNode>> env_buffer;
  /* This one is temporary */
  mutable std::vector<std::shared_ptr<VoxelNode>> TEMP_RENDER_BUFFER;

  /* This one is an interface for voxels */
  std::unique_ptr<KDTree> env_to_render;
  mutable EnvironmentState recent_env_state;

  /* TODO: SHOULD REFACTOR TO VOXEL NODE??? */
  void ProcessInput(const float& delta);
  void RenderVoxels(const float& delta);
  bool IsVoxelInView(const float& delta, PositionVectors delta_vector) const;
  void FindVoxelsInView(const float& delta);
  void Update(const float& delta);
  void DetectCollision(const float& delta);
  void ModifyEnvironment(const float& delta);
  void LoadEnvironmentBuffer();
  PositionVectors VoxelPositionVectors(const float& delta, Voxel voxel) const;
  double VoxelDistanceVector(PositionVectors vector) const;
  void DebugInfo();
};
