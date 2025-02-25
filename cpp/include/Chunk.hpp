#pragma once
#include "Voxels.hpp"
#include "agent.hpp"

template <typename Data>
class Chunk {
 public:
  /* Denotes the position of the back-top-left voxel, the boundaries can be
   * calculated */
  int id;
  Vector3 start_of_chunk;
  std::unique_ptr<Voxel<Data>> root;
  std::unique_ptr<Voxel<Data>> best_voxel_cand;
  mutable double best_distance_cand;
  Chunk(const int& id, const float& x, const float& y, const float& z);
  std::unique_ptr<Voxel<Data>> InsertVoxel(
      std::unique_ptr<Voxel<Data>>& last_free_voxel,
      std::unique_ptr<Voxel<Data>>& new_voxel, int depth, int8_t x, int8_t y,
      int8_t z);
  bool CompareVoxels(const std::unique_ptr<Voxel<Data>>& voxel1,
                     const std::unique_ptr<Voxel<Data>>& voxel2) const;
  bool FindVoxel(const std::unique_ptr<Voxel<Data>>& node,
                 const std::unique_ptr<Voxel<Data>>& voxel_to_find,
                 int depth) const;
  std::unique_ptr<Voxel<Data>> FindNearestVoxel(
      const std::unique_ptr<Voxel<Data>> target);
  void FindVoxelsInRange(const Vector3& position, const float& radius);
  void SearchForNearestVoxel(std::unique_ptr<Voxel<Data>>& voxel,
                             const Vector3& target_position, int depth,
                             std::unique_ptr<Voxel<Data>>& best_voxel,
                             double& best_distance);

  int8_t GetDistance(const std::unique_ptr<Voxel<Data>>& voxel1,
                     const std::unique_ptr<Voxel<Data>>& voxel2) const;
  void UpdateRoot(std::unique_ptr<Voxel<Data>>& voxel);
  bool ShouldRenderChunk(const Cursor& cursor) const;
};
