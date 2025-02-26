#pragma once
#include <math.h>
#include <raylib.h>

#include <tuple>
#include <vector>

#include "Environment.hpp"
using PositionVectors = std::tuple<double, double, double>;
using Voxel = EnvironmentNode<ObjectInfo>;

class Chunk {
 public:
  std::shared_ptr<Voxel> root;
  std::shared_ptr<Voxel> best_voxel_cand;
  std::vector<std::shared_ptr<Voxel>> voxels_to_render;
  mutable double best_distance_cand;
  mutable int rendered = 0;
  Chunk();
  std::shared_ptr<Voxel> InsertVoxel(std::shared_ptr<Voxel>& last_free_voxel,
                                     std::shared_ptr<Voxel>& new_voxel,
                                     int depth);

  bool CompareVoxels(const Vector3& vec1, const Vector3& vec2) const;
  bool FindVoxel(const std::shared_ptr<Voxel>& node,
                 const std::shared_ptr<Voxel>& voxel_to_find, int depth) const;
  std::shared_ptr<Voxel> FindNearestVoxel(const std::shared_ptr<Voxel> target);
  void FindVoxelsInRange(const Vector3& position, const float& radius);
  void SearchForNearestVoxel(std::shared_ptr<Voxel>& voxel,
                             const Vector3& target_position, int depth,
                             std::shared_ptr<Voxel>& best_voxel,
                             double& best_distance);
  void LoadRenderBuffer(std::shared_ptr<Voxel>& voxel,
                        std::shared_ptr<Voxel>& temp_voxel, float radius,
                        int depth);
  float GetDistance(const Vector3& vec1, const Vector3& vec2) const;
  void UpdateRoot(std::shared_ptr<Voxel>& voxel);
};
