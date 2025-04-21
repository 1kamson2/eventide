#pragma once
#include <iostream>
#include <vector>

#include "Utils/Enums.hpp"
#include "Voxel/Voxel.hpp"
using namespace voxel;
using namespace chunk_enums;
namespace chunk {
constexpr float SIZE = 16.0f;
constexpr float DEFAULT_DISTANCE = 4 * chunk::SIZE;

class Chunk {
 public:
  std::shared_ptr<Voxel> root_voxel = nullptr;
  Vector3 mid_point;
  mutable bool visible = false;

  Chunk(const Vector3& mid_point);
  void Update(VoxelRef voxel);
  float GetDistanceSquared(VoxelCRef vox1, VoxelCRef vox2);

  float GetDistanceSquared(Vector3& vec1, Vector3& vec2);
  Vector3 GetMidPoint() const;

  void MarkAs(const Visibility& status) const;
  bool IsVisible() const;
  bool IsVoxelInChunk(VoxelCRef voxel);
  bool TheSameChunk(const Chunk& rhs);

  void LoadVoxelsLTY(float& max_y, VoxelRef curr_voxel);
  bool InView(Vector3& vec_pos, const float& max_allowed_dist);
  bool InAxisView(const float& first_scalar_point_value,
                  const float& second_scalar_point_value,
                  const float& max_allowed_dist) const;
  std::shared_ptr<Voxel> Insert(VoxelRef last_free_voxel, VoxelRef new_voxel,
                                int depth);
  friend bool operator==(VoxelCRef lhs, VoxelCRef rhs);
};
}  // namespace chunk
