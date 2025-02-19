#pragma once
#include <raylib.h>

#include <iostream>
#include <vector>

#include "agent.hpp"
#include "environment.hpp"
#include "resource_manager.hpp"
class Engine {
 public:
  Engine();
  // Engine(params);
  Agent AgentInstance;
  int width, height, fps, collision_where;
  int max_voxels_in_view;
  double gravity;
  mutable std::vector<EnvironmentObject> env;
  mutable std::vector<EnvironmentObject> env_render_buffer;
  mutable EnvironmentState recent_env_state;

  void ProcessInput(const float& delta);
  void RenderVoxels(const float& delta);
  bool IsVoxelInView(const float& delta, const int& which_voxel) const;
  void FindVoxelsInView(const float& delta);
  void Update(const float& delta);
  void DetectCollision(const float& delta);
  void ModifyEnvironment(const float& delta);
  std::vector<EnvironmentObject> LoadEnvironmentBuffer();
  void DebugInfo();
};
