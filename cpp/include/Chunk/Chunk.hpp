#pragma once
#include <iostream>
#include <vector>

#include "Voxel/Voxel.hpp"
using namespace voxel;
namespace chunk {
constexpr float SIZE = 16.0f;
constexpr float DEFAULT_DISTANCE = 2 * chunk::SIZE;

class Chunk {
 public:
  std::shared_ptr<Voxel> root_voxel = nullptr;
  Vector3 mid_point;

  Chunk(const Vector3& mid_point);
  void Update(std::shared_ptr<Voxel>& voxel);
  float GetDistanceSquared(const std::shared_ptr<Voxel>& vox1,
                           const std::shared_ptr<Voxel>& vox2);

  float GetDistanceSquared(Vector3& vec1, Vector3& vec2);
  Vector3 GetMidPoint() const;

  // std::shared_ptr<Voxel>& GetVoxelFromLayer(const float& y);
  bool IsVoxelInChunk(const std::shared_ptr<Voxel>& voxel);
  bool TheSameChunk(const Chunk& rhs);

  void LoadVoxelsLTY(float& max_y, std::shared_ptr<Voxel>& curr_voxel);
  bool InView(Vector3& vec_pos, const float& max_allowed_dist);
  bool InAxisView(const float& first_scalar_point_value,
                  const float& second_scalar_point_value,
                  const float& max_allowed_dist) const;
  std::shared_ptr<Voxel> Insert(std::shared_ptr<Voxel>& last_free_voxel,
                                std::shared_ptr<Voxel>& new_voxel, int depth);
  friend bool operator==(const Voxel& lhs, const Voxel& rhs);
};
}  // namespace chunk
