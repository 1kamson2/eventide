#pragma once
#include <math.h>
#include <raylib.h>

#include <tuple>
#include <vector>

#include "environment.hpp"

class KDTree {
 public:
  std::shared_ptr<VoxelNode> root;
  std::shared_ptr<VoxelNode> best_voxel_cand;
  std::vector<std::shared_ptr<VoxelNode>> voxels_to_render;
  mutable double best_distance_cand;
  mutable int rendered = 0;
  KDTree();
  std::shared_ptr<VoxelNode> InsertVoxel(
      std::shared_ptr<VoxelNode>& last_free_voxel,
      std::shared_ptr<VoxelNode>& new_voxel, int depth);

  bool CompareVoxels(const Vector3& vec1, const Vector3& vec2) const;
  bool FindVoxel(const std::shared_ptr<VoxelNode>& node,
                 const std::shared_ptr<VoxelNode>& voxel_to_find,
                 int depth) const;
  std::shared_ptr<VoxelNode> FindNearestVoxel(
      const std::shared_ptr<VoxelNode> target);
  void FindVoxelsInRange(const Vector3& position, const float& radius);
  void SearchForNearestVoxel(std::shared_ptr<VoxelNode>& voxel,
                             const Vector3& target_position, int depth,
                             std::shared_ptr<VoxelNode>& best_voxel,
                             double& best_distance);
  void LoadRenderBuffer(std::shared_ptr<VoxelNode>& voxel,
                        std::shared_ptr<VoxelNode>& temp_voxel, float radius,
                        int depth);
  float GetDistance(const Vector3& vec1, const Vector3& vec2) const;
  void UpdateRoot(std::shared_ptr<VoxelNode>& voxel);
};
